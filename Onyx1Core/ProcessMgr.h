//
// Created by jantypas on 10/25/21.
//

#ifndef ONYX1_PROCESSMGR_H
#define ONYX1_PROCESSMGR_H
#include <stdint-gcc.h>
#include <string>
#include <vector>
#include "VMemMgr.h"

class SegnentRequest {
    std::string     name;
    uint32_t        protection;
    uint32_t        desiredPages;
};

enum ProcessState {
    IDLE,
    ASLEEP,
    READY_TO_RUN,
    RUNNING,
    TERMINATING
};

class ProcessControlBlock {
    std::string                             name;
    std::vector<std::string>                args;
    bool                                    supervisor;
    bool                                    doNotSwap;
    uint32_t                                owner, group;
    std::vector<SegnentRequest *>           requestedSegments;
    std::vector<uint32_t>                   processPages;
    std::map<uint32_t, VirtualSegment *>    actualSegments;
    ProcessState                            state;
};


class ProcessMgr {
    bool                                        isReady;
    std::map<uint32_t, ProcessControlBlock *>   processTable;

    int32_t initialize(VMemMgr *mgr);
    int32_t create(ProcessControlBlock *pcb, uint32_t *procid);
    int32_t destroy(uint32_t procID);
    int32_t readAddress(uint32_t procid, uint64_t addr, uint64_t *result);
    int32_t writeAddress(uint32_t procid, uint64_t addr, uint64_t value);
    int32_t changeState(uint32_t procID, ProcessState newState);
    std::map<uint32_t, ProcessControlBlock *> *getProcessTable();
    int32_t terminate();
};


#endif //ONYX1_PROCESSMGR_H
