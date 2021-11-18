//
// Created by jantypas on 11/9/21.
//

#ifndef ONYX1_PROCESSMGR_H
#define ONYX1_PROCESSMGR_H
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <queue>
#include "../Onyx1Const.h"
#include "../CPU/CPUError.h"
#include "../CPU/CPUCore.h"
#include "VMemMgr.h"

enum ProcessState {
    ASLEEP,
    READY_TO_RUN,
    RUNNING,
    RUNNING_LOCKED,
    WAITING_FOR_IO,
    WAITING_FOR_COPROCESSOR,
    WAITING_FOR_SYSCALL
};

struct SegmentInfo {
    VSegment        segmentInfo;
    VPageProtection protectiohn;
    uint32_t        numPages;
};

class procMsg_t {
    uint32_t senderPID;
    uint32_t receiverPID;
    uint64_t timeSent;
    uint64_t msg[PROC_MSG_SIZE];
};

class ProcessControlBlock {
    uint32_t                        ppid;
    std::string                     processName;
    std::vector<std::string>        args;
    uint32_t                        owner, group;
    uint64_t                        createdOn;
    bool                            supervisor;
    std::map<uint32_t, SegmentInfo> segAddresses;
    std::vector<uint32_t>           memoryPages;
    ProcessState                    state;
    CPUContext                      cpuData;
    std::queue<procMsg_t>           msgQUeue;
};

class ProcessMgr {
    std::map<uint32_t, ProcessControlBlock> processTable;

    int32_t startup();
    int32_t createProcess(std::string name, std::vector<std::string> args, uint32_t owner, uint32_t group,
                          bool supervisor, std::vector<SegmentInfo> segments, uint32_t *procID);
    int32_t destroyProcess(uint32_t procID);
    int32_t changeState(uint32_t procID, ProcessState newState);
    int32_t loadProcessFromDisk(uint32_t procID, uint32_t startPage, std::fstream fs);
    int32_t saveProcessToFile(uint32_t procID, uint32_t startPage, uint32_t endPage, std::fstream fs);
    int32_t readAddress(uint32_t procID, uint64_t addr, uint64_t *value);
    int32_t writeAddress(uint32_t procID, uint64_t addr, uint64_t value);
    int32_t shutdown();
};

#endif //ONYX1_PROCESSMGR_H
