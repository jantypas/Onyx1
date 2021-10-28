//
// Created by jantypas on 10/25/21.
//

#ifndef ONYX1_PROCESSMGR_H
#define ONYX1_PROCESSMGR_H
#include <stdint-gcc.h>
#include <string>
#include <vector>
#include "VMemMgr.h"

/**
 * To build a process, we need an idea of what segments we want in memory
 */
class SegnentRequest {
    std::string     name;               // The segment name
    uint32_t        protection;         // The segment mode bits
    uint32_t        desiredPages;       // HOw many pages we would like
    uint32_t        desiredStartPage;   // Where we would like this segment to staart
};

/**
 * Processes can be in a variety of states
 */
enum ProcessState {
    IDLE,           // Process is idle and effectively in "no state"
    ASLEEP,         // Process is asleep
    READY_TO_RUN,   // Process is ready to run
    RUNNING,        // Process is running
    TERMINATING     // Process is terminated, needs cleanup
};

/**
 * The ProcessControlBlock holds information about each process in the system
 */
class ProcessControlBlock {
public :
    std::string                             name;               // Name of the process
    std::vector<std::string>                args;               // Process arguments
    bool                                    supervisor;         // Is this process kernel mode?
    bool                                    doNotSwap;          // Can we timeslice away right now
    uint32_t                                owner, group;       // Owner and group of the process
    std::vector<uint32_t>                   processPages;       // Mapped memory pages for this process
    std::map<uint32_t, VirtualSegment *>    actualSegments;     // Segments we actually got from VMemMgr
    std::map<std::string, uint32_t>         pageStartMap;       // Start addresses for segments
    ProcessState                            state;              // Process state
};

/**
 * Process Manager
 */
class ProcessMgr {
    bool                                        isReady;        // Is the process manager ready
    std::map<uint32_t, ProcessControlBlock *>   processTable;   // The table of processes
    VMemMgr                                     *vmemptr;       // Pointer to our VMemMgr instance

    int32_t initialize(VMemMgr *mgr);
    int32_t create(ProcessControlBlock *pcb, std::vector<SegnentRequest *> segs, uint32_t *procid);
    int32_t destroy(uint32_t procID);
    int32_t readAddress(uint32_t procid, uint64_t addr, uint64_t *result);
    int32_t writeAddress(uint32_t procid, uint64_t addr, uint64_t value);
    int32_t changeState(uint32_t procID, ProcessState newState);
    std::map<uint32_t, ProcessControlBlock *> *getProcessTable();
    int32_t terminate();
};


#endif //ONYX1_PROCESSMGR_H
