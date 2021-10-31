//
// Created by jantypas on 10/25/21.
//

#include "ProcessMgr.h"
#include "CPUError.h"

/**
 * initialize -- Initialize the Process Manager
 *
 * @param mgr   -- Reference to the Virtual Memory Manager
 * @return      -- Return result
 */
int32_t ProcessMgr::initialize(VMemMgr *mgr) {
    /* Make sure the VMemMgr is ready for us */
    if (mgr->mmuIsReady) { return CPUError_NotImplemented; };
    /* Clear the process table and save our reference to the VMemMgr */
    processTable.clear();   // Clear the process table
    vmemptr = mgr;          // Save a reference to our VMemMgr
    isReady = true;         // We're ready to go
    return CPUError_None;
}

/**
 * create -- Create a new process.
 *
 * Given a process control block, attempt to use it to create the new process
 *
 * @param pcb       -- The process control block
 * @param procid    -- The process ID will be filled in here
 * @return          -- Result code
 */
int32_t ProcessMgr::create(ProcessControlBlock *pcb, std::vector<SegnentRequest *> segs, uint32_t *procid) {
    /**
     * Make sure everything is up and running
     **/
    if (!isReady) { return CPUError_ProcessNotReady; };

    /**
     * Create a new process object and copy all static items to it
     **/
    auto newPCB = new ProcessControlBlock();
    *newPCB = *pcb;
    /**
     * Now, for each segment request object, create a segment and allocate it to allocatedSegments
     **/
    for (auto sp : segs) {
        /* Create a virtual page object we can use for this page group */
        VirtualPageObject po;
        std::vector<uint32_t> pageout;

        po.pageState   = PAGE_STATE_INUSE;
        po.pageState   = sp->protection;
        po.diskBlock   = 0;
        auto newSeg = vmemptr->allocateNewSegmentPages(po, sp->desiredPages, &pageout);
        /* For our new set of pages, build our porcess page table */
        for (auto ix : pageout) {
            newPCB->processPages.push_back(ix);
        };
        nextProcessID++;
        processTable.emplace(nextProcessID, newPCB);
        return CPUError_None;
    }
}

int32_t ProcessMgr::destroy(uint32_t procID) {
    return 0;
}

int32_t ProcessMgr::readAddress(uint32_t procid, uint64_t addr, uint64_t *result) {
    return 0;
}

int32_t ProcessMgr::writeAddress(uint32_t procid, uint64_t addr, uint64_t value) {
    return 0;
}

int32_t ProcessMgr::changeState(uint32_t procID, ProcessState newState) {
    return 0;
}

std::map<uint32_t, ProcessControlBlock *> *ProcessMgr::getProcessTable() {
    return nullptr;
}

int32_t ProcessMgr::terminate() {
    return 0;
}
