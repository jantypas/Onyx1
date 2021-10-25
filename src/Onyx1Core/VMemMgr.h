//
// Created by jantypas on 10/20/21.
//

#ifndef ONYX1_VMEMMGR_H
#define ONYX1_VMEMMGR_H
#include <stdint-gcc.h>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <queue>
#include "Onyx1Const.h"

/**
 * The VMemInfo structure is used to hold useful informatipon for the caller.
 */
struct VMemInfo {
    bool isVirtual;
    bool mmuIsReady;
};

/**
 * A physical page object holds a raw uint64_t buffer
 */
class PhysicalPageObject {
public :
    std::array<uint64_t, MAX_PAGE_SIZE> storage;
};

/**
 * A virtual segment holds each segment in the virtual memory system
 * NOTE: The actual pages are referred to in another segmentContainer map.  We jsut
 * store the inext.
 */
class VirtualSegment {
public :
    std::string name;
    uint32_t    protectioh;
    uint32_t    segmentIdx;
};

/**
 * The virtual page container maps a container index used by Virtual Segment to a series of pages
 */
class VirtualPageContainer {
public :
    std::vector<uint32_t> pages;
};

/**
 * For each segment in the system, we have a series of virtual pages.
 * Each virtual page has this structure
 */
class VirtualPageObject {
public :
    uint32_t    physicalPage;
    uint32_t    pageState;
    uint32_t    diskBlock;
    uint64_t    lastUsed;
};

/**
 * The Virtual Memory System itself
 */
class VMemMgr {
public :
    const uint32_t PAGE_IN_MEMORY = 0x0000'0001;
    const uint32_t PAGE_IS_DIRTY = 0x0000'0002;
    const uint32_t PAGE_IS_LOCKED = 0x0000'0004;
    const uint32_t PAGE_ON_DISK = 0x0000'0008;

    bool isVirtual = false;
    bool mmuIsReady = false;
    uint32_t requestedVirtualPages = 0;
    uint32_t requestedPhysicalPages = 0;
    uint32_t nextSegment = 0;
    uint32_t nextSegmentID = 0;

    PhysicalPageObject *physicalPageTable;
    VirtualPageObject *virtualPageTable;
    std::queue<uint32_t> usedPhysicalPages, freePhysicalPages;
    std::queue<uint32_t> usedVirtualPages, freeVirtualPages;
    std::map<uint32_t, VirtualPageContainer *> segmentContainerMap;
    std::map<uint32_t, VirtualSegment *> segmentMap;

    int32_t initializeAsRealMode(uint32_t numRealPages);
    int32_t initializeAsVirtual(uint32_t numVirtualPages, uint32_t numPhysicalPages);
    int32_t initialize(bool isVirt, uint32_t numVirt, uint32_t numPhys);
    int32_t terminate();
    int32_t allocateNewSegmentPages(uint32_t numPages);
    int32_t allocateNewSegment(std::string name, uint32_t mode, uint32_t pages);
    int32_t releaseSegment(uint32_t segid);
    int32_t allocateNewVirtualPage();
    int64_t readAddress(uint64_t addr, int32_t *error);
    int32_t writeAddress(uint64_t addr, int64_t value);
    void    info(VMemInfo *info);
    int32_t swapInPage(uint32_t pageid);
    uint32_t swapOutPage(uint32_t pageid);
    uint32_t swapOutNPages(uint32_t numPages);
    int32_t loadPage(uint32_t pageid, PhysicalPageObject *buffer);
    int32_t savePage(uint32_t pageid, PhysicalPageObject *buffer);
};

#endif //ONYX1_VMEMMGR_H
