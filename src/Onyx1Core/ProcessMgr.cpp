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
    if (mgr->mmuIsReady) { return CPUError_NotImplemented; };
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
int32_t ProcessMgr::create(ProcessControlBlock *pcb, uint32_t *procid) {
    if (!isReady) { return CPUError_ProcessNotReady; };
    auto newPCB = new ProcessControlBlock();
    newPCB->name        = pcb->name;
    newPCB->args        = pcb->args;
    newPCB->doNotSwap   = false;



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
