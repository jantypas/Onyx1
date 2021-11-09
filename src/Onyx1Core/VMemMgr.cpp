//
// Created by jantypas on 10/20/21.
//

#include "VMemMgr.h"
#include "CPUError.h"
#include <fstream>

/******************************************************************************************************
 ******************************************************************************************************
 * Utility and support functions
 *
 * These functions are used all over this source code.  These functions create and destroy key objects and
 * manage things like various state flags
 *
 * WARNING: THESE FUNCTIONS, THOUGH PUBLIC, ARE INTERNAL FUNCTIOHNS.  THEY DO NOT PERFORM THE NORMAL
 * PUBLIC ERROR CHECKING WE USE IN THE PUBLIC FUNCTIONS.  USE THESE WITH CARE.  THEY ARE ONLY PUBLIC FOR
 * EDUCATIONAL PURPOSES.
 *
 ******************************************************************************************************
 ******************************************************************************************************/

uint32_t    addrToPage(uint64_t addr) { return addr / MAX_PAGE_SIZE; };
uint32_t    addrToOffset(uint64_t addr) { return addr % MAX_PAGE_SIZE; };

/**
 * Int_isPageSwappable -- Is this page swappable?
 * @param page  -- The page we want to swap
 * @return      -- Yes or no
 */
bool VMemMgr::Int_isPageSwappable(uint32_t page) {
    auto pp = virtualPageTable[page];
    if (!pp.state.inUse)    { return false; };
    if (pp.state.isDirty)   { return false; };
    if (pp.state.isLocked)  { return false; };
    if (pp.state.isOnDisk)  { return false; };
    return true;
}

/**
 * Int_isPageSwappedOut - Is this page swapped out
 * @param page  -- The page we want to test
 * @return      -- Yes/No
 */
bool VMemMgr::Int_isPageSwappedOut(uint32_t page) {
    return virtualPageTable[page].state.isOnDisk;
}

/**
 * Int_isPageInUse  -- Is this page in use?
 * @param page  -- The page we want to test
 * @return      -- Yes/No
 */
bool VMemMgr::Int_isPageInUse(uint32_t page) {
    return virtualPageTable[page].state.inUse;
}

/**
 * Int_isPageLocked -- Is this page locked in memory?
 * @param page  -- The page we want to test
 * @return      -- Yes/No
 */
bool VMemMgr::Int_isPageLocked(uint32_t page) {
    return virtualPageTable[page].state.isLocked;
}

/**
 * Int_isPageSwappedIn  -- Is the page swapped into memory
 * @param page  -- The page we want to test
 * @return      -- Yes/No
 */
bool VMemMgr::Int_isPageSwappedIn(uint32_t page) {
    return (!virtualPageTable[page].state.isOnDisk);
}

/**
 * Int_getOldestVirtualPages -- Get the list of virtual pages sorted by age
 *
 * @param foundPages    -- The list of pages
 */
void VMemMgr::Int_getOldestVirtualPages(std::vector<uint32_t> *foundPages) {
    std::map<uint64_t, uint32_t> sortedMap;
    /* Walk the virtual page table and place all swappable pages in the sorted map */
    for (uint32_t ix = 0; ix < requestedVirtualPages; ix++) {
        auto vp = &virtualPageTable[ix];
        if (Int_isPageSwappable(ix)) {
            sortedMap.emplace(vp->lastUsed, ix);
        }
    };
}

/**
 * Int_swapOutPage  -- Swap a given page out to disk
 *
 * @param pageid    -- The page to swap out
 * @return          -- Result code
 */
uint32_t VMemMgr::Int_swapOutPage(uint32_t pageid) {
    /* Make sure our page is valid, in use, and swappable */
    if (pageid >= requestedVirtualPages)        { return CPUError_InvalidPage; };
    if (!Int_isPageInUse(pageid))               { return CPUError_PageIsFree; };
    if (!Int_isPageSwappable(pageid))           { return CPUError_InvalidPage; };
    /*
     * Seek to the offset for this block of memory and write it out
     * Don't forget to sync to make sure this page is actually written to disk
     */
    Int_writeOutPageToDisk(pageid);
    /*
     * Now, let's put the physical page we had back in the free pool and remove it from the used pool
     * First remove the physical page from the used pool, then add it to the free pool
     */
    physicalBitmap.set(virtualPageTable[pageid].physicalPage, false)
    /*
     * Now, update our virtual page to reflect that it has no physical page and is swapped out
     */
    virtualPageTable[pageid].diskBlock         = pageid;
    virtualPageTable[pageid].state.isOnDisk    = true;
    return CPUError_None;
}

/**
 * Int_swapOutNPages    -- Swap out a page set
 * @param pagelist  -- The list of pages
 * @return          == Result code
 */
uint32_t VMemMgr::Int_swapOutNPages(std::vector<uint32_t> *pagelist) {
    for (auto ix : *pagelist {
        Int_swapOutPage(ix);
    };
    return CPUError_None;
}

/**
 * Int_swapInPage   -- Swap a page in
 *
 * @param pageid    -- Page to swap in
 * @return          -- Result code
 */
int32_t VMemMgr::Int_swapInPage(uint32_t pageid) {
    if (pageid >= requestedVirtualPages) { return CPUError_InvalidPage; };
    if (!Int_isPageSwappedOut(pageid)) { return CPUError_PageSwapError; };
    uint32_t newPage = 0;

    /* Get a reference to our page */
    auto vp = &virtualPageTable[pageid];
    /* If we're out of free physical pages, try to make some */
    std::vector<uint32_t> freePagesList, oldPagesList;
    Int_getOldestVirtualPages(&freePagesList);

    if (freePagesList.empty()) {
        Int_getOldestVirtualPages(&oldPagesList);
        Int_swapOutPage((oldPagesList[9]));
    };
    /* Now that we have a physical page, let's swap ours in */
    std::vector<uint32_t> newPages;
    Int_findFreePhysicalPages(&newPages);
    auto newFreePage = newPages[0];
    Int_readInPageFromDisk(newFreePage);
    /* Finally, bind that physical page to the virtual one */
    virtualPageTable[pageid].physicalPage = newPage;
    virtualPageTable[pageid].state.isOnDisk = false;
    virtualPageTable[pageid].state.inUse = true;
    return CPUError_None;
}

/**
 * Int_findSwappablePages   -- Get a list of swappable pages
 *
 * @param foundPages    -- The list of pages we found
 */
void VMemMgr::Int_findSwappablePages(std::vector<uint32_t> *foundPages) {
    for (uint32_t ix = 0; ix < requestedVirtualPages; ix++) {
        if (Int_isPageSwappable(ix)) { foundPages->push_back(ix) };
    }
}

/**
 * Int_writeOutPage -- Write a page out to disk
 *
 * @param page  -- The page to write
 * @return      -- The result code
 */
uint32_t VMemMgr::Int_writeOutPageToDisk(uint32_t page) {
    if (!swapper.is_open()) {
        return CPUError_CantSwap;
    };
    auto vp = physicalPageTable[virtualPageTable[page].physicalPage].buffer.byte_array;
    swapper.seekp(MAX_PAGE_SIZE*page, std::ios::beg);
    swapper.write((char *)vp, MAX_PAGE_SIZE);
    swapper.sync();
    return CPUError_None;
}

/**
 * Int_readPageFromDisk -- Read a page from disk
 *
 * @param page  -- The page to read
 * @return      -- Result code
 */
uint32_t VMemMgr::Int_readInPageFromDisk(uint32_t page) {
    if (!swapper.is_open()) {
        return CPUError_CantSwap;
    };
    auto vp = physicalPageTable[virtualPageTable[page].physicalPage].buffer.byte_array;
    swapper.seekp(MAX_PAGE_SIZE*page, std::ios::beg);
    swapper.read(vp);
    return CPUError_None;
}

/**
 * Int_findFreePhysicalPages    -- Return a list of free physical pages
 *
 * @param pagelist  -- The list of pages returned
 */
void VMemMgr::Int_findFreePhysicalPages(std::vector<uint32_t> *pagelist) {
    for (auto ix = 0; ix < requestedPhysicalPages; ix++) {
        if (!physicalBitmap[ix]) { pagelist.push_back(ix); };
    };
}

/**
 * Int_findFreeVirtualPages -- Get a list of free virtual pages
 *
 * @param pagelist  -- The list of pages returned
 */
void VMemMgr::Int_findFreeVirtualPages(std::vector<uint32_t> *pagelist) {
    for (auto ix = 0; ix < requestedVirtualPages; ix++) {
        if (!virtualBitmap[ix]) { pagelist.push_back(ix); };
    };
}

/**************************************************************************************************
 **************************************************************************************************
 *
 * Actual interface functions
 *
 *************************************************************************************************
 *************************************************************************************************/

int32_t VMemMgr::initialize(uint32_t numVirt, uint32_t numPhys) {
    if (mmuIsReady) { return CPUError_MMUNotReady; };
    requestedVirtualPages   = numVirt;
    requestedPhysicalPages  = numPhys;
    virtualPageTable        = new VirtualPageObject[requestedVirtualPages];
    physicalPageTable       = new PhysicalPageObject[requestedPhysicalPages];
    virtualBitmap.reset();
    physicalBitmap.reset();
    swapper.open(SWAPFILE_NAME, std::ios::in|std::ios::out|std::ios:binary);
    mmuIsReady = true;
    return CPUError_None;
}

int32_t VMemMgr::terminate() {
    if (mmuIsReady) {
        swapper.close();
        mmuIsReady = false;
        delete virtualPageTable;
        delete physicalPageTable;
        return CPUError_None;
    } else {
        return CPUError_MMUNotReady;
    }
}

int32_t VMemMgr::allocateVirtualPage(VirtualPageObject po, uint32_t *pages) {
    if (mmuIsReady) {
        /* First see if we can get a physical page */
        std::vector<uint32_t> freePhysicalPages;
        std::vector<uint32_t> freeVirtualPages;

        Int_findFreeVirtualPages(&freeVirtualPages);
        Int_findFreePhysicalPages(&freePhysicalPages);

        /* If the list is empty, we need to do a swap */
        if (freePhysicalPages.empty()) {
            std::vector<uint32_t> oldestVirtualPages;

            Int_getOldestVirtualPages(&oldestVirtualPages);
            /* We don't want to swap out just one page */
            if (oldestVirtualPages.size() > MIN_VIRTUAL_PAGES) {
                for (auto ix = 0; ix < MIN_VIRTUAL_PAGES; ix++) {
                    Int_swapOutPage(oldestVirtualPages[ix]);
                };
            } else {
                Int_swapOutPage(oldestVirtualPages[0]);
            };
            Int_findFreePhysicalPages(&freePhysicalPages);
        };
        /* We now have at least one physical page */
        auto newPPage = freePhysicalPages[0];
        /* Now see about a free virtual page */
        if (freeVirtualPages.empty()) { return CPUError_NoVirtualPages; };
        /* We've got a physical and virtual page, make both as used */
        auto newVPage = freeVirtualPages[0];
        virtualBitmap.set(newPPage, true);
        physicalBitmap.set(newPPage, true);
        /* Now set up the page structure */
        virtualPageTable[newVPage].physicalPage     = newPPage;
        virtualPageTable[newVPage].state.inUse      = true;
        virtualPageTable[newVPage].state.isLocked   = false;
        virtualPageTable[newVPage].state.isDirty    = false;
        virtualPageTable[newVPage].protection = po.protection;
        *pages = newVPage;
        return CPUError_None;
    } else {
        return CPUError_MMUNotReady;
    }
}

int32_t VMemMgr::allocateNewVirtualPageSet(VirtualPageObject po, uint32_t numPages, std::vector<uint32_t> *pageset) {
    for (auto ix = -; ix < numPages; ix++) {
        uint32_t newPage;
        allocateVirtualPage(po, &newPage);
        pageset.push_back(newPage);
    }
    return CPUError_None;
}

int32_t VMemMgr::freeVirtualPage(uint32_t page) {

}

int32_t VMemMgr::freeVirtualPageSet(std::vector<uint32_t> *pagelist) {
    return 0;
}

int64_t VMemMgr::readAddress(uint64_t addr, int32_t *error) {
    return 0;
}

int32_t VMemMgr::writeAddress(uint64_t addr, int64_t value) {
    return 0;
}

void VMemMgr::info(VMemInfo *info) {

}

int32_t VMemMgr::loadPage(uint32_t pageid, PhysicalPageObject *buffer) {
    return 0;
}

int32_t VMemMgr::savePage(uint32_t pageid, PhysicalPageObject *buffer) {
    return 0;
}

