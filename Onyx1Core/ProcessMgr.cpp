//
// Created by jantypas on 10/25/21.
//

#include "ProcessMgr.h"
#include "CPUError.h"

/**
 * initialize -- Initialize the process manager
 *
 * @param mgr   -- The VMemMgr we're using
 * @return      -- Result code
 */
int32_t ProcessMgr::initialize(VMemMgr *mgr) {
    if (!mgr->mmuIsReady) { return CPUError_None; };
    processTable.clear();
    isReady = true;
    return CPUError_None;
}

/**
 * create -- Create a new process
 *
 * @param pcb       -- A process control block that defines our request
 * @param procid    -- The process ID that was created for this process
 * @return          -- Result code
 */
int32_t ProcessMgr::create(ProcessControlBlock *pcb, uint32_t *procid) {
    return 0;
}

int32_t ProcessMgr::destroy(uint32_t procID) {
    return 0;
}

int32_t ProcessMgr::changeState(uint32_t procID, ProcessState newState) {
    return 0;
}

int32_t ProcessMgr::readAddress(uint32_t procid, uint64_t addr, uint64_t *result) {
    return 0;
}

int32_t ProcessMgr::writeAddress(uint32_t procid, uint64_t addr, uint64_t value) {
    return 0;
}

int32_t ProcessMgr::terminate() {
    return 0;
}

std::map<uint32_t, ProcessControlBlock *> *ProcessMgr::getProcessTable() {
    return nullptr;
}

