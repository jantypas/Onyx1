//
// Created by jantypas on 10/20/21.
//

#include "VMemMgr.h"
#include "CPUError.h"
#include <fstream>

/******************************************************************************************************
 * Utility and support functions
 *
 * These functions are used all over this source code.  These functions create and destroy key objects and
 * manage things like various state flags
 *
 ******************************************************************************************************/
uint32_t    addrToPage(uint64_t addr) { return addr / MAX_PAGE_SIZE; };
uint32_t    addrToOffset(uint64_t addr) { return addr % MAX_PAGE_SIZE; };

bool VMemMgr::moveVirtualFromFreeToUsed(uint32_t page) {
    freeVirtualPages.erase(page);
    usedVirtualPages.emplace(page, 0);
    return true;
}

bool VMemMgr::moveVirtualFromUsedToFree(uint32_t page) {
    usedVirtualPages.erase(page);
    freeVirtualPages.emplace(page, 0);
    return true;
}

bool VMemMgr::movePhysicalFromFreeToUsed(uint32_t page) {
    freePhysicalPages.erase(page);
    usedPhysicalPages.emplace(page, 0);
    return true;
}

bool VMemMgr::movePhysicalFromUsedToFree(uint32_t page) {
    usedPhysicalPages.erase(page);
    freePhysicalPages.emplace(page, 0);
    return true;
}

VirtualPageObject *VMemMgr::makeCodePage() {
    auto *vpo = new VirtualPageObject();
    vpo->protection.isExecutable    = true;
    vpo->protection.isReadable      = true;
    vpo->protection.isPrivileged    = true;
    vpo->protection.isStack         = false;
    vpo->protection.isWritable      = false;
    vpo->state.inUse                = false;
    return vpo;
}

VirtualPageObject *VMemMgr::makeDataObject() {
    auto *vpo = new VirtualPageObject();
    vpo->protection.isExecutable    = false;
    vpo->protection.isReadable      = true;
    vpo->protection.isPrivileged    = false;
    vpo->protection.isStack         = false;
    vpo->protection.isWritable      = true;
    vpo->state.inUse                = false;
    return vpo;
}

VirtualPageObject *VMemMgr::makeStackObject() {
    auto *vpo = new VirtualPageObject();
    vpo->protection.isExecutable    = false;
    vpo->protection.isReadable      = true;
    vpo->protection.isPrivileged    = false;
    vpo->protection.isStack         = true;
    vpo->protection.isWritable      = true;
    vpo->state.inUse                = false;
    return vpo;
}

VirtualPageObject *VMemMgr::makeKernalPage() {
    auto *vpo = new VirtualPageObject();
    vpo->protection.isExecutable    = true;
    vpo->protection.isReadable      = true;
    vpo->protection.isPrivileged    = true;
    vpo->protection.isStack         = true;
    vpo->protection.isWritable      = true;
    vpo->state.inUse                = false;
    return vpo;
}

bool VMemMgr::isPageSwappable(uint32_t page) {
    auto pp = virtualPageTable[page];
    if (!pp.state.inUse)    { return false; };
    if (pp.state.isDirty)   { return false; };
    if (pp.state.isLocked)  { return false; };
    if (pp.state.isOnDisk)  { return false; };
    return true;
}

void VMemMgr::getOldestVirtualPages(std::vector<uint32_t> *foundPages) {
    std::map<uint64_t, uint32_t> sortedMap;
    /* Walk the virtual page table and place all swappable pages in the sorted map */
    for (uint32_t ix = 0; ix < requestedVirtualPages; ix++) {
        auto vp = &virtualPageTable[ix];
        if (isPageSwappable(ix)) {
            sortedMap.emplace(vp->lastUsed, ix);
        }
    };
}


uint32_t VMemMgr::swapOutPage(uint32_t pageid) {
    /* Make sure our page is valid, in use, and swappable */
    if (pageid >= requestedVirtualPages)    { return CPUError_InvalidPage; };
    if (!isPageInUse(pageid))               { return CPUError_PageIsFree; };
    if (!isPageSwappable(pageid))           { return CPUError_InvalidPage; };
    /*
     * Seek to the offset for this block of memory and write it out
     * Don't forget to sync to make sure this page is actually written to disk
     */
    auto vref = &virtualPageTable[pageid];
    swapper.seekp(MAX_PAGE_SIZE*pageid);
    swapper.write(physicalPageTable[vref->physicalPage).storage);
    swapper.sync();
    /*
     * Now, let's put the physical page we had back in the free pool and remove it from the used pool
     * First remove the physical page from the used pool, then add it to the free pool
     */
    movePhysicalFromUsedToFree(vref->physicalPage);
    /*
     * Now, update our virtual page to reflect that it has no physical page and is swapped out
     */
    vref->state.isOnDisk    = true;
    vref->diskBlock         = pageid;
    return CPUError_None;
}


uint32_t VMemMgr::swapOutNPages(uint32_t numPages) {
    std::vector<uint32_t> pageset;
    findSwappablePages(&pageset);
    if (pageset.size() == 0) {
        return CPUError_NoVirtualPages;
    };
    for (auto ix : pageset) {
        swapOutPage(ix);
    };
    return CPUError_None;
}

bool VMemMgr::isPageSwappedOut(uint32_t page) {
    return virtualPageTable[page].state.isOnDisk;
}

bool VMemMgr::isPageInUse(uint32_t page) {
    return virtualPageTable[page].state.inUse;
}


int32_t VMemMgr::swapInPage(uint32_t pageid) {
    if (pageid >= requestedVirtualPages) { return CPUError_InvalidPage; };
    if (!isPageSwappedOut(pageid)) { return CPUError_PageSwapError; };
    uint32_t newPage = 0;

    /* Get a reference to our page */
    auto vp = &virtualPageTable[pageid];
    /* If we're out of free physical pages, try to make some */
    if (freePhysicalPages.empty()) {
        std::vector<uint32_t> freePages;
        getOldestVirtualPages(&freePages);
        /* Still no free pages -- this is bad */
        if (freePages.size() == 0) { return CPUError_NoPhysicalPages; };
        swapOutPage(freePages[0]);
        newPage = freeVirtualPages.begin()->first;
        moveVirtualFromFreeToUsed(newPage);
    } else {
        newPage = freeVirtualPages.begin()->first;
        moveVirtualFromFreeToUsed(newPage);
    };
    /* Now that we have a page, update the virtual page reference */
    virtualPageTable[pageid].state.isOnDisk = false;
    virtualPageTable[pageid].physicalPage   = newPage;

    /* Load the page from disk into physical memory */
    swapper.seekg(MAX_PAGE_SIZE*pageid);
    swapper.read(&physicalPageTable[newPage].storage);
    return CPUError_None;
}

bool VMemMgr::isPageSwappedIn(uint32_t page) {
    return (!virtualPageTable[page].state.isOnDisk);
}

bool VMemMgr::isPageLocked(uint32_t page) {
    return virtualPageTable[page].state.isLocked;
}

void VMemMgr::findSwappablePages(std::vector<uint32_t> *foundPages) {
    for (uint32_t ix = 0; ix < requestedVirtualPages; ix++) {
        if (isPageSwappable(ix)) { foundPages->push_back(ix) };
    }
}

int32_t VMemMgr::swapOutTopPage() {
    auto topPage = freeVirtualPages.begin().first;
    swapOutPage(topPage);
    return CPUError_None;
}

/**************************************************************************************************
 *
 * Actual interface functions
 *
 *************************************************************************************************/

/**
 * initialize - Initialize the virutal memory manager.
 *
 * *INTERNAL ROUTINE*
 *
 * @param isVirt    - Is this in virtual memory mode
 * @param numVirt   - Number of virtual pages we need
 * @param numPhys   - Number of physical pages we need
 * @return          - Result code
 */
int32_t VMemMgr::initialize(bool isVirt, uint32_t numVirt, uint32_t numPhys) {

    requestedPhysicalPages  = numPhys;
    requestedVirtualPages   = numVirt;

    // Create the tables
    physicalPageTable   = new PhysicalPageObject[numPhys];
    virtualPageTable    = new VirtualPageObject[numPhys];

    // Build the free and used page maps
    for (uint32_t ix = 0; ix < requestedPhysicalPages; ix++) { freePhysicalPages.emplace(ix, 0); };
    for (uint32_t ix = 0; ix < requestedVirtualPages; ix++) { freeVirtualPages.emplace(ix, 0); };
    usedPhysicalPages.clear();
    usedVirtualPages.clear();

    /* Set up swapping */
    swapper.open(SWAPFILE_NAME, std::ios::in|std::ios::out|std::ios::binary);
    if (!swapper.is_open()) {
        delete physicalPageTable;
        delete virtualPageTable;
        return CPUError_CantSwap;
    }

    // All done
    mmuIsReady = true;
    return CPUError_None;
};

/**
 * allocateNewVirtualPage() -- Allocate a new virtual page
 *
 * Returns a page number or error value
 */
int32_t VMemMgr::allocateNewVirtualPage(VirtualPageObject po, uint32_t *pageid) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };

    if (freeVirtualPages.empty()) {
        return CPUError_NoVirtualPages;
    } else {
        /**
         * See if we have any free physical pages left.
         * If we don't, let's swwap some out
         */
        if (freePhysicalPages.empty()) {
          std::vector<uint32_t> pagesAvail;
          findSwappablePages(&pagesAvail);
          if (pagesAvail.size() == 0) {
              return CPUError_NoVirtualPages;
          }
          /**
           * Ok, we have pages we can release.  We don't really want to release one page.
           * If we did, we'd have to do this all the time.
           */
          auto freeNPages = 0;
          if (pagesAvail.size() > SWAP_MINIMUM_PAGES) {
              for (auto ix = SWAP_MINIMUM_PAGES; ix != 0; ix--) {
                  swapOutTopPage();
              }
          } else {
              swapOutTopPage();
          }

        };
        /**
         * Get a new virtual page number
         **/
        auto newVPage = freeVirtualPages.back();
        freeVirtualPages.pop_back();
        usedVirtualPages.push_back(newVPage);
        /**
         * Get a physical page to go with it
         **/
        auto newPPage = freePhysicalPages.back();
        freePhysicalPages.pop_back();
        usedPhysicalPages.push_back(newPPage);
        /**
         * Tie the virtual page to a physical page
         **/
        virtualPageTable[newVPage].physicalPage = newPPage;
        virtualPageTable[newVPage].diskBlock    = 0;
        virtualPageTable[newVPage].physicalPage = PAGE_IN_MEMORY;
        virtualPageTable[newVPage].lastUsed     = time(std::nullptr_t);
        virtualPageTable[newVPage].protection   = po.protection;
        virtualPageTable[newVPage].pageState    |= PAGE_STATE_INUSE;
        *pageid = newVPage;
        return CPUError_None;
    }
}

/**
 * allocateNewSegmentPages -- Allocate new a memory segment
 *
 * @param numPages  -- Allocate N pages for the new segment
 * @param mode      -- The mode for this segment
 * @return          -- Result code or segment ID
 */
int32_t VMemMgr::allocateNewSegmentPages(VirtualPageObject po, uint32_t numPages, std::vector<uint32_t> *pages) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (numPages > requestedVirtualPages) { return CPUError_NoVirtualPages; };

    for (auto ix = 0; ix < numPages; ix++) {
        auto newPage = allocateNewVirtualPage(po);
        pages->push_back(newPage);
    };
    return CPUError_None;
}


/**
 * terminate -- Terminate the memory manager.
 *        Free all resources and shut the manager down
 * @return = Result code
 */
int32_t VMemMgr::terminate() {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (!swapper.close()) { };
    return CPUError_None;
}

/**
 * readAddress -- Read an address from virtual memory
 *
 * @param addr  -- Address to read
 * @param error -- Was there an error -- return the error code
 * @return      -- Result code
 */
int64_t VMemMgr::readAddress(uint64_t addr, int32_t *error) {
    if (!mmuIsReady) { *error = CPUError_MMUNotReady; return 0; };

    auto page   = addrToPage(addr);
    auto offset = addrToOffset(addr);
    if (page > requestedPhysicalPages) { *error = CPUError_InvalidPage; return 0; };
    return virtualPageTable[page].physicalPage[offset];
}

/**
 * writeAddress -- Write to a virtual memory address
 *
 * @param addr      -- Address to write to
 * @param value     -- Value to write
 * @return          -- Result code
 */
int32_t VMemMgr::writeAddress(uint64_t addr, int64_t value) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };

    auto page   = addrToPage(addr);
    auto offset = addrToOffset(addr);
    if (page > requestedPhysicalPages) { return CPUError_InvalidPage; };
    virtualPageTable[page].physicalPage[offset] = value;
    return CPUError_None;
}

/**
 * info -- Get memory manager info
 *
 * @param info  -- The info structure we'll fill in
 */
void VMemMgr::info(VMemInfo *info) {
    info->mmuIsReady - mmuIsReady;
}

/**
 * loadPage -- Load a page of virtual memory with the contents of a page buffer
 *
 * @param pageid    -- The virtual page to load
 * @param buffer    -- The buffer to load from
 * @return          -- Result code
 */
int32_t VMemMgr::loadPage(uint32_t pageid, PhysicalPageObject *buffer) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };

    if (pageid > requestedPhysicalPages) { return CPUError_InvalidPage; };
    auto physPage = virtualPageTable[pageid];
    auto memptr =  physicalPageTable[physPage.physicalPage];
    memptr.storage = buffer.storage;
    return CPUError_None;
}

/**
 * savePage -- Save a page to a buffer
 *
 * @param pageid    -- THe page to save
 * @param buffer    -- The buffer to save it to
 * @return          -- The result code
 */
int32_t VMemMgr::savePage(uint32_t pageid, PhysicalPageObject *buffer) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };

    if (pageid > requestedPhysicalPages) { return CPUError_InvalidPage; };
    auto physPage = virtualPageTable[pageid];
    auto memptr =  physicalPageTable[physPage.physicalPage];
    buffer.storage = memptr.storage;
    return CPUError_None;
}
