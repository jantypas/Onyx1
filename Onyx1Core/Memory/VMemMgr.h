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
#include <fstream>
#include <bitset>
#include "../Onyx1Const.h"

/**
 * We need some forward references....
 */
class ProcessMgr;
class cpuCore;

/**
 * A physical page object holds a raw uint64_t buffer
 * We need to treat this as a union variant because we need to easily convert
 * this data to a byte array for swapping
 */
class PhysicalPageObject {
public :
    union {
        uint8_t     byte_array[MAX_PAGE_SIZE*sizeof(uint64_t)];
        uint64_t    long_array[MAX_PAGE_SIZE];
    } buffer;
};

/**
 * VPageProection   -- Protection bits for a page
 * VPageState       == State bits for a page
 */

enum VPageSegmentType {
    CODE,                   // Code segment
    DATA,                   // Data segment
    STACK,                  // Stack segment
    KERNEL,                 // Supervisor kernel segment
    IO,                     // IO memory
    OTHER,                  // Other (non-managed) segment
    EMPTY,                  // Empty page
    INFO                    // Read only memory
};

struct VPageProtection {
    bool isReadable;    // Page is readable
    bool isWritable;    // Page is writable
    bool isExecutable;  // Page is executable
    bool isPrivileged;  // Page is kernel level
    bool isStack;       // Page is a stack page
};

struct VPageState {
    VPageSegmentType    segType;
    bool                inUse;      // Page is in use
    bool                isDirty;    // Page is dirty and needs writing out
    bool                isLocked;   // Page is locked in memory
    bool                isOnDisk;   // Page is on disk
};

/**
 * For each segment in the system, we have a series of virtual pages.
 * Each virtual page has this structure
 */
struct __attribute__ ((packed)) VirtualPageObject {
public :
    uint32_t        physicalPage;   // THe physical page we're mapped to
    VPageProtection protection;
    VPageState      state;
    uint32_t        processID;  // Process that owns this page
    uint32_t        diskBlock;  // Disk block page is on
    uint64_t        lastUsed;   // Timestamp of last use
};

/**
 * VSegment -- Contains the information about a memory segment
 * This is a user-level structure that contains the required information to create, or alter
 * a segment, but it does not contain all of the management information.
 * Typically, it's used by the ProcessMgr.
 */
struct VSegment {
    uint32_t        procID;             // Process that owns this segment
    uint32_t        startPage, endPage; // Page addresses for start and end of segment
    VPageProtection protection;         // Desired protections
    VPageState      state;              // Desired state
};

/**
 * The VMemInfo structure is used to hold useful information for the caller.
 * Typically used by the ProcessMgr, it allows the process manager and CPU to inquire
 * about virtual memory.
 */
struct VMemInfo {
    uint64_t                swapIns;        // How many swap ins have we done during the last inquiry
    uint64_t                swapOuts;       // How many swap outs have we done during the last inquiry
    bool                    mmuIsReady;     // Is our MMU active
    std::vector<VSegment>   segmentInfo;    // THe list of segments we ahve
};


/**
 * The Virtual Memory System itself
 */
class VMemMgr {
public :
    ProcessMgr  *procRef;
    cpuCore     *cpuRef;

    /*
     * Sme bookkeeping items we need to keep rather than recomputing them as we need them
     */
    bool        mmuIsReady              = false;
    uint32_t    requestedVirtualPages   = 0;
    uint32_t    requestedPhysicalPages  = 0;
    VMemInfo    currentInfo;

    /*
     * The page table objects and their free/used bitmaps
     */
    PhysicalPageObject          *physicalPageTable;
    VirtualPageObject           *virtualPageTable;
    std::bitset<MAX_PAGE_BITS>  virtualBitmap;
    std::bitset<MAX_PAGE_BITS>  physicalBitmap;

    /*
     * The stream for disk swapping
     */
    std::fstream            swapper;

    /*
     * Internal functions to make and manage pages
     */

    /*
     * Our test functions
     */
    bool      Int_isPageSwappable(uint32_t page);
    bool      Int_isPageInUse(uint32_t page);
    bool      Int_isPageLocked(uint32_t page);
    bool      Int_isPageSwappedIn(uint32_t page);
    bool      Int_isPageSwappedOut(uint32_t page);
    bool      Int_isPageDirty(uint32_t page);

    /*
     * Internal functions that let us find what's going on in page tables
     */
    void      Int_findSwappablePages(std::vector<uint32_t> *foundPages);
    void      Int_getOldestVirtualPages(std::vector<uint32_t> *foundPages);
    void      Int_findFreePhysicalPages(std::vector<uint32_t> *pagelist);
    void      Int_findFreeVirtualPages(std::vector<uint32_t> *pagelist);

    /*
     * Internal functions for swapping
     */
    uint32_t  Int_writeOutPageToDisk(uint32_t page);
    uint32_t  Int_readInPageFromDisk(uint32_t page);
    uint32_t  Int_swapOutPage(uint32_t pageid);
    uint32_t  Int_swapOutNPages(std::vector<uint32_t> *pagelist);
    int32_t   Int_swapInPage(uint32_t pageid);

    /*
     * Public methods
     */
    int32_t initialize(uint32_t numVirt, uint32_t numPhys);
    void    connectReferences(ProcessMgr *proc, cpuCore *cpu);
    int32_t terminate();
    int32_t allocateVirtualPage(VSegment vs, uint32_t *pages);
    int32_t allocateNewVirtualPageSet(VSegment vs, uint32_t numPages, std::vector<uint32_t> *pageset);
    int32_t freeVirtualPage(uint32_t page);
    int32_t freeVirtualPageSet(std::vector<uint32_t> *pagelist);
    int64_t readAddress(uint64_t addr, int64_t *value);
    int32_t writeAddress(uint64_t addr, int64_t value);
    void    info(VMemInfo *info);
    int32_t loadPage(uint32_t pageid, PhysicalPageObject *buffer);
    int32_t savePage(uint32_t pageid, PhysicalPageObject *buffer);
};

#endif //ONYX1_VMEMMGR_H
