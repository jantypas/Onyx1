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
#include "Onyx1Const.h"

/**
 * A physical page object holds a raw uint64_t buffer
 */
class PhysicalPageObject {
public :
    std::array<uint64_t, MAX_PAGE_SIZE> storage;
};

/**
 * For each segment in the system, we have a series of virtual pages.
 * Each virtual page has this structure
 */
struct __attribute__ ((packed)) VirtualPageObject {
public :
    uint32_t    physicalPage;   // THe physical page we're mapped to
    struct {
        bool isReadable;    // Page is readable
        bool isWritable;    // Page is writable
        bool isExecutable;  // Page is executable
        bool isPrivileged;  // Protect is kernel level
        bool isStack;       // Page is a stack page
    } protection;
    struct {
        bool    inUse;      // Page is in use
        bool    isDirty;    // Page is dirty and needs writing out
        bool    isLocked;   // Page is locked in memory
        bool    isOnDisk;   // Page is on disk
    } state;
    uint32_t    processID;  // Process that owns this page
    uint32_t    diskBlock;  // Disk block page is on
    uint64_t    lastUsed;   // Timestamp of last use
};
/**
 * The VMemInfo structure is used to hold useful informatipon for the caller.
 */
struct VMemInfo {
    bool isVirtual;
    bool mmuIsReady;
};


/**
 * The Virtual Memory System itself
 */
class VMemMgr {
public :
    /*
     * Sme bookkeeping items we need to keep rather than recomputing them as we need them
     */
    bool        mmuIsReady              = false;
    uint32_t    requestedVirtualPages   = 0;
    uint32_t    requestedPhysicalPages  = 0;

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

    /* Useful utility methods and factory methods */
    VirtualPageObject   *makeCodePage();
    VirtualPageObject   *makeDataObject();
    VirtualPageObject   *makeStackObject();
    VirtualPageObject   *makeKernalPage();
    VirtualPageObject   *makeUnusedPage();

    /*
     * Internal functions to make and manage pages
     */
    bool      movePhysicalFromFreeToUsed(uint32_t page);
    bool      movePhysicalFromUsedToFree(uint32_t page);
    bool      moveVirtualFromFreeToUsed(uint32_t page);
    bool      moveVirtualFromUsedToFree(uint32_t page);
    bool      isPageSwappable(uint32_t page);
    bool      isPageInUse(uint32_t page);
    bool      isPageLocked(uint32_t page);
    bool      isPageSwappedIn(uint32_t page);
    bool      isPageSwappedOut(uint32_t page);
    void      findSwappablePages(std::vector<uint32_t> *foundPages);
    void      getOldestVirtualPages(std::vector<uint32_t> *foundPages);
    void      findFreePhysicalPages(std::vector<uint32_t> *pagelist);
    void      findFreeVirtualPages(std::vector<uint32_t> *pagelist);

    /*
     * Internal functions for swapping
     */
    uint32_t  writeOutPageToDisk(uint32_t page);
    uint32_t  readInPageFromDisk(uint32_t page);
    uint32_t  swapOutPage(uint32_t pageid);
    uint32_t  swapOutNPages(uint32_t numPages);
    int32_t   swapInPage(uint32_t pageid);

    /*
     * Public methods
     */
    int32_t initialize(uint32_t numVirt, uint32_t numPhys);
    int32_t terminate();
    int32_t allocateVirtualPages(VirtualPageObject po, uint32_t numPages, std::vector<uint32_t> *pages);
    int32_t allocateNewVirtualPage(VirtualPageObject po, uint32_t *pageid);
    int32_t freeVirtualPage(uint32_t page);
    int32_t freeSegmentPageSet(std::vector<uint32_t> *pagelist);
    int64_t readAddress(uint64_t addr, int32_t *error);
    int32_t writeAddress(uint64_t addr, int64_t value);
    void    info(VMemInfo *info);
    int32_t loadPage(uint32_t pageid, PhysicalPageObject *buffer);
    int32_t savePage(uint32_t pageid, PhysicalPageObject *buffer);
};

#endif //ONYX1_VMEMMGR_H
