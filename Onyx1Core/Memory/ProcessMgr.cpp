//
// Created by jantypas on 11/9/21.
//

#include "ProcessMgr.h"

int32_t ProcessMgr::startup() {
    return 0;
}


int32_t ProcessMgr::createProcess(std::string name, std::vector<std::string> args, uint32_t owner, uint32_t group,
                                  bool supervisor, std::vector<SegmentInfo> segments, uint32_t *procID) {
    return 0;
}

int32_t ProcessMgr::destroyProcess(uint32_t procID) {
    return 0;
}

int32_t ProcessMgr::changeState(uint32_t procID, ProcessState newState) {
    return 0;
}

int32_t ProcessMgr::loadProcessFromDisk(uint32_t procID, uint32_t startPage, std::fstream fs) {
    return 0;
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
