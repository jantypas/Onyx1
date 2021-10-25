//
// Created by jantypas on 10/20/21.
//

#include "VMemMgr.h"
#include "CPUError.h"

/**
 * Some internal functions
 */
uint32_t addrToPage(uint64_t addr) { return addr / MAX_PAGE_SIZE; };
uint32_t addrToOffset(uint64_t addr) { return addr % MAX_PAGE_SIZE; };

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

    /* Right now, we only support physical mode */
    if (!isVirt) {
        isVirtual = false;  // We're in physical mode
        requestedPhysicalPages = numPhys;

        // Create the tables
        physicalPageTable = new PhysicalPageObject[numPhys];
        virtualPageTable = new VirtualPageObject[numPhys];
        // Build the free and used page maps
        for (int32_t ix = 0; ix < numPhys; ix++) {
            freePhysicalPages.push(ix);
            freeVirtualPages.pop();
        };
        // All done
        mmuIsReady = true;
        return CPUError_None;
    } else {
        // We don't with virtual memory right now
        mmuIsReady = false;
        return CPUError_InvalidConfiguration;
    };
};

/**
 * initializeAsRealMode   -- Initialize memory in real mode
 *
 * @param numRealPages  - Number of physical pages we need
 * @return              - Result code
 */
int32_t VMemMgr::initializeAsRealMode(uint32_t numRealPages) {
    return initialize(false, numRealPages);
}

/**
 * initializeAsVirtual - Initialize memory in virtual memory mode
 *
 * @param numVirtualPages   - Number of virtual pages we need
 * @param numPhysicalPages  - Number of physical pages we need
 * @return                  - Result code
 */
int32_t VMemMgr::initializeAsVirtual(uint32_t numVirtualPages, uint32_t numPhysicalPages) {
    return initialize(true, numVirtualPages, numPhysicalPages);
};

/**
 * allocateNewVirtualPage() -- Allocate a new virtual page
 *
 * Returns a page number or error value
 */
int32_t VMemMgr::allocateNewVirtualPage() {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (!isVirtual) { return CPUError_InvalidConfiguration; };

    if (freeVirtualPages.empty()) {
        return CPUError_NoVirtualPages;
    } else {
        /**
         * Get a new virtual page number
         **/
        auto newVPage = freeVirtualPages.front();
        freeVirtualPages.pop();
        usedVirtualPages.push(newVPage);
        /**
         * Get a physical page to go with it
         **/
        auto newPPage = freePhysicalPages.front();
        freePhysicalPages.pop();
        usedPhysicalPages.push(newPPage);
        /**
         * Tie the virtual page to a physical page
         **/
        virtualPageTable[newVPage].physicalPage = newPPage;
        virtualPageTable[newVPage].diskBlock    = 0;
        virtualPageTable[newVPage].physicalPage = PAGE_IN_MEMORY;
        virtualPageTable[newVPage].lastUsed     = time(std::nullptr_t);
        return static_cast<uint32_t> (newVPage);
    }
}

/**
 * alocateNewSegmentPages -- Allocate new a memory segment
 *
 * @param numPages  -- Allocate N pages for the new segment
 * @param mode      -- The mode for this segment
 * @return          -- Result code or segment ID
 */
int32_t VMemMgr::allocateNewSegmentPages(uint32_t numPages) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (isVirtual) { return CPUError_InvalidConfiguration; };

    auto container = new VirtualPageContainer();
    for (auto ix = 0; ix < numPages; ix++) {
        auto newPage = allocateNewVirtualPage();
        container->pages.push_back(newPage);
    };
    nextSegmentID++;
    segmentContainerMap.emplace(nextSegmentID, container );
    return static_cast<uint32_t>(nextSegmentID);
}

/**
 * allocateNewSegment -- Allocate a new segment
 *
 * @param name      -- Name of the segment
 * @param mode      -- Segment mode bits
 * @param pages     -- Number of pages we need
 * @return          -- Error or segment number
 */
int32_t VMemMgr::allocateNewSegment(std::string name, uint32_t mode, uint32_t pages) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (isVirtual) { return CPUError_InvalidConfiguration; };
    /* Allocate the basic segment structure */
    auto newSeg = new VirtualSegment();
    newSeg->protectioh = mode;
    newSeg->name = name;
    newSeg->segmentIdx = allocateNewSegmentPages(pages);
    nextSegmentID++;
    segmentMap.emplace(nextSegmentID, newSeg);
    return CPUError_None;
}

/**
 * releaseSegment -- Release an existing segment
 *
 * @param segid -- The segment we want to releaswe
 * @return      -- Error code
 */
int32_t VMemMgr::releaseSegment(uint32_t segid) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (!isVirtual) { return CPUError_InvalidConfiguration; };
    if (!segmentMap.contains(segid)) { return CPUError_InvalidSegment; };
    auto segmentObject = segmentMap[segid];
    auto pagelist = segmentContainerMap[segmentObject->segmentIdx];
    pagelist->pages.clear();
    segmentMap.erase(segid));
    return CPUError_None;
}

/**
 * terminate -- Terminate the memory manager.
 *        Free all resources and shut the manager down
 * @return = Result code
 */
int32_t VMemMgr::terminate() {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    for (ix : segmentMap) {
        releaseSegment(ix.second->segmentIdx);
    }
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
    if (!isVirtual) { *error = CPUError_InvalidConfiguration; return 0; };

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
    if (!isVirtual) { return CPUError_InvalidConfiguration; };

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
    info->isVirtual = isVirtual;
    info->mmuIsReady - mmuIsReady;
}

/**
 * swapInPage -- Swap in a page of virtual memory
 *
 * @param pageid    -- Page to swap in
 * @return          == Result code
 */
int32_t VMemMgr::swapInPage(uint32_t pageid) {
    return CPUError_InvalidConfiguration;
}

/**
 * swapOutPage -- Swap a page out to disk
 *
 * @param pageid    -- Page to swap out
 * @return
 */
uint32_t VMemMgr::swapOutPage(uint32_t pageid) {
    return CPUError_InvalidConfiguration;
}

/**
 * swapOutNPages -- Swap out at lease N pages.
 *
 * Given a need for virtual memory and physical memory, ask the MMU to swap out a set of pages.
 *
 * @param numPages  -- Number of pages to try and swap out
 * @return          -- Result code
 */
uint32_t VMemMgr::swapOutNPages(uint32_t numPages) {
    return CPUError_InvalidConfiguration;
};

/**
 * loadPage -- Load a page of virtual memory with the contents of a page buffer
 *
 * @param pageid    -- The virtual page to load
 * @param buffer    -- The buffer to load from
 * @return          -- Result code
 */
int32_t VMemMgr::loadPage(uint32_t pageid, PhysicalPageObject *buffer) {
    if (!mmuIsReady) { return CPUError_MMUNotReady; };
    if (!isVirtual) { return CPUError_InvalidConfiguration; };

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
    if (!isVirtual) { return CPUError_InvalidConfiguration; };

    if (pageid > requestedPhysicalPages) { return CPUError_InvalidPage; };
    auto physPage = virtualPageTable[pageid];
    auto memptr =  physicalPageTable[physPage.physicalPage];
    buffer.storage = memptr.storage;
    return CPUError_None;
}