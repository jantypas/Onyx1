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
    virtualBitmap.set(page, true);
    virtualPageTable[page].state.inUse = true;
    return true;
}

bool VMemMgr::moveVirtualFromUsedToFree(uint32_t page) {
    virtualBitmap.set(page, false);
    virtualPageTable[page].state.inUse = false;
    return true;
}

bool VMemMgr::movePhysicalFromFreeToUsed(uint32_t page) {
    physicalBitmap.set(page, true);
    return true;
}

bool VMemMgr::movePhysicalFromUsedToFree(uint32_t page) {
    physicalBitmap.set(page, false);
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
    vpo->state.inUse                = true;
    vpo->physicalPage               = 0;
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

VirtualPageObject *VMemMgr::makeUnusedPage() {
    auto *vpo = new VirtualPageObject();
    vpo->protection.isExecutable    = false;
    vpo->protection.isReadable      = false;
    vpo->protection.isPrivileged    = false;
    vpo->protection.isStack         = false;
    vpo->protection.isWritable      = false;
    vpo->state.inUse                = false;
    vpo->physicalPage               = 0;
    return vpo;
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
    writeOutPageToDisk(pageid);
    /*
     * Now, let's put the physical page we had back in the free pool and remove it from the used pool
     * First remove the physical page from the used pool, then add it to the free pool
     */
    movePhysicalFromUsedToFree(virtualPageTable[pageid].physicalPage);
    /*
     * Now, update our virtual page to reflect that it has no physical page and is swapped out
     */
    virtualPageTable[pageid].diskBlock         = pageid;
    virtualPageTable[pageid].state.isOnDisk    = true;
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
    getOldestVirtualPages()
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

uint32_t VMemMgr::writeOutPageToDisk(uint32_t page) {
    if (!swapper.is_open()) {
        return CPUError_CantSwap;
    };
    auto vp = physicalPageTable[virtualPageTable[page].physicalPage].storage;
    swapper.seekp(MAX_PAGE_SIZE*page, std::ios::beg);
    swapper.write(vp);
    swapper.sync();
    return CPUError_None;
}

uint32_t VMemMgr::readInPageFromDisk(uint32_t page) {
    if (!swapper.is_open()) {
        return CPUError_CantSwap;
    };
    auto vp = physicalPageTable[virtualPageTable[page].physicalPage].storage;
    swapper.seekp(MAX_PAGE_SIZE*page, std::ios::beg);
    swapper.read(vp);
    return CPUError_None;
}

void VMemMgr::findFreePhysicalPages(std::vector<uint32_t> *pagelist) {
    for (auto ix = 0; ix < requestedPhysicalPages; ix++) {
        if (!physicalBitmap[ix]) { pagelist.push_back(ix); };
    };
}

void VMemMgr::findFreeVirtualPages(std::vector<uint32_t> *pagelist) {
    for (auto ix = 0; ix < requestedVirtualPages; ix++) {
        if (!virtualBitmap[ix]) { pagelist.push_back(ix); };
    };
}

/**************************************************************************************************
 *
 * Actual interface functions
 *
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

int32_t VMemMgr::allocateNewVirtualPage(VirtualPageObject po, uint32_t *pageid) {
    if (mmuIsReady) {
        std::vector<uint32_t> pagelist;
        findFreePhysicalPages(&pagelist);
        if (pagelist.size() == 0) {
            std::vector<uint32_t> newPageList;
            getOldestVirtualPages(&newPageList);
            for (auto ix = 0; ix < MIN_VIRTUAL_PAGES; ix++) {
                swapOutPage(newPageList[ix]);
            }
        };
        auto newVPageID = findFreeVirtualPage();
        auto newPPageID = findFreePhysicalPage();
        auto newVPO     = &virtualPageTable[newVPageID];

    } else {
        return CPUError_MMUNotReady;
    }
}

int32_t VMemMgr::allocateVirtualPages(VirtualPageObject po, uint32_t numPages, std::vector<uint32_t> *pages) {
    return 0;
}

