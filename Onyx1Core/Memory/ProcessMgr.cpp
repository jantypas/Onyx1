//
// Created by jantypas on 11/9/21.
//
#include "../CPU/CPUError.h"
#include "ProcessMgr.h"
#include "../Memory/VMemMgr.h"

/**
 * startup -- Start up the process manager
 * @parm vmgr   -- Pointer to the VMemMgr we're using
 * @return      -- Result code
 */
int32_t ProcessMgr::startup(VMemMgr *vmgr) {
    vmemptr = vmgr;
    processTable.clear();
    return CPUError_None;
}

/**
 * createProcess -- Create a new process
 *
 * @param name          -- Name of the process
 * @param args          -- Argument list of the process
 * @param owner         -- Owner ID of the process
 * @param group         -- Group ID of the process
 * @param supervisor    -- Is this a supervisor process
 * @param segments      -- List of segment requets for this process
 * @param procID        -- Returns the process ID
 * @return              -- Result code
 */
int32_t ProcessMgr::createProcess(std::string name, std::vector<std::string> args, uint32_t owner, uint32_t group,
                                  bool supervisor, std::vector<SegmentInfo> segments, uint32_t *procID) {
    /*
     * Create a new process structure to put things in
     */
    ProcessControlBlock *pcb = new ProcessControlBlock();
    /*
     * Try to get all our segments from VMemMgr
     */
    int32_t bottomPage = 0x0000'0100;

    for (auto segid : segments) {
        int32_t result;
        std::vector<uint32_t> newPageSet;
        /*
         * Try to get the desired number of pages with the desired segment state
         */
        result = vmemptr->allocateNewVirtualPageSet(segid.segmentInfo, segid.numPages, &newPageSet);
        if (result != CPUError_None) { return CPUError_InvalidPage; };
        /*
         * Now that we've got pages, move that list into process memory space/
         * Note the start and end pages of that segment and store that in the segment object.
         */
        segid.segmentInfo.startPage = bottomPage;
        segid.segmentInfo.endPage += segid.numPages;
        pcb->memoryPages.insert(pcb->memoryPages.end(), newPageSet.begin(), newPageSet.end());
        bottomPage += segid.numPages+1;
    };
    /*
     * Now set up the rest of the PCB
     */
    pcb->state = ASLEEP;
    pcb->supervisor = supervisor;
    pcb->owner = owner;
    pcb->group = group;
    pcb->args = args;
    pcb->processName = name;
    pcb->createdOn = time(NULL);
    /*
     * Put the new process PCB in the map
     */
    processTable.emplace(nextProcess, pcb);
    *procID = nextProcess;
    return CPUError_None;
}

/**
 * destroyProcess -- Destory a process
 *
 * @param procID    -- Process to destroy
 * @return          -- Result code
 */
int32_t ProcessMgr::destroyProcess(uint32_t procID) {
    /**
     * If the process we have is in the process map
     */
    if (processTable.contains(procID)) {
        /*
         * Get the process control block
         */
        auto pcb = processTable[procID];
        /*
         * Free segment pages
         */
        vmemptr->freeVirtualPageSet(&pcb->memoryPages);
        /*
         * Remove from the map
         */
        processTable.erase(procID);
        return CPUError_None;
    } else {
        return CPUError_InvalidProcess;
    }
}

/**
 * changeState -- Change state of a process
 *
 * @param procID    -- Process ID to change
 * @param newState  -- New state
 * @return          -- Result code
 */
int32_t ProcessMgr::changeState(uint32_t procID, ProcessState newState) {
    /*
     * If the process ID is value, change the pcb state
     */
    if (processTable.contains(procID)) {
        auto pcb = processTable[procID];
        pcb->state = newState;
        return CPUError_None;
    } else {
        return CPUError_InvalidProcess;
    }
}

/**
 * loadProcessFromDisk -- LOad a process into memory from a disk file
 *
 * @param procID        -- Process ID to laod into
 * @param startPage     -- Load into this start page
 * @param fs            -- The file stream to laod from
 * @return              -- Result code
 */
int32_t ProcessMgr::loadProcessFromDisk(uint32_t procID, uint32_t startPage, std::fstream fs) {
    /*
     * If we have a valid process ID
     */
    if (processTable.contains(procID)) {

    } else {
        return CPUError_InvalidProcess;
    }

}

int32_t ProcessMgr::saveProcessToFile(uint32_t procID, uint32_t startPage, uint32_t endPage, std::fstream fs) {
    return 0;
}

int32_t ProcessMgr::readAddress(uint32_t procID, uint64_t addr, uint64_t *value) {
    return 0;
}

int32_t ProcessMgr::writeAddress(uint32_t procID, uint64_t addr, uint64_t value) {
    return 0;
}

int32_t ProcessMgr::shutdown() {
    return 0;
}
