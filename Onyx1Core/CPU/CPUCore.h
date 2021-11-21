//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_CPUCORE_H
#define ONYX1_CPUCORE_H
#include <queue>
#include "../Onyx1Const.h"
#include <stdint-gcc.h>
#include "../Memory/VMemMgr.h"
#include "../Memory/ProcessMgr.h"

const   uint32_t    REG_PC          = 0x7F;     // Processor PC
const   uint32_t    REG_SP          = 0X7E;     // Stack pointer

const   uint32_t    REG_INTR_MASK   = 0xBF;     // Interrupt mask
const   uint32_t    REG_CONFIG      = 0xBE;     // Configuration mask
const   uint32_t    REG_BASETIME    = 0xBD;     // Base timer value
const   uint32_t    REG_INSTRCOUNT  = 0xBC;     // Base instruction count
const   uint32_t    REG_IDISPATCH   = 0xBB;     // Interrupt dispatch table address
const   uint32_t    REG_TRAPADDR    = 0xBA;     // Trap dispatch address
const   uint32_t    REG_BUFFERPAGE  = 0xB9;     // Buffer page structure address
const   uint32_t    REG_CONTROLA    = 0xB8;     // 64-bit control register A

const   uint32_t    FLAGS_CPU       = 0x80;     // Processor flags
const   uint32_t    FLAGS_IOSERVICE = 0x81;     // Who needs IO Service

class RegFile {
    long dRegisters[8];
    long aRegisters[8];
};

union cpuFeatures {
    struct {
        uint64_t value;
    } bits;
    struct {
        bool doShadowStack : 1 = true;
        bool doAutoRegPush : 1 = true;
    } features;
};

union cpuControl {
    struct {
        uint64_t value;
    } bits;
    struct {
        bool debug  : 1 = false;
    } features;
};

union cpuFlags {
    struct                                                                                                                                                         ruct {
        uint64_t value;
    } bits;
    struct {
        bool divideBVZero : 1   = false;
        bool overflow : 1       = false;
        bool softError : 1      = false;
        bool hardError : 1      = false;
        bool isTrue : 1         = false;
    } features;
};

struct CPUContext {
    long        registersA[MAX_REGISTERS];
    long        registersB[MAX_REGISTERS];
    long        regFile[REGFILE_SIZE][REGFILE_LENGTH][2];
    long        *currentRegisters;
    cpuFeatures featuresA;
    cpuFlags    flags;
    cpuControl  control;
};

class VMemMgr;
class ProcessMgr;

class CPUCore {
    CPUContext          context;
    VMemMgr             *memPtr;
    ProcessMgr          *procPtr;

    void connectReferences(VMemMgr *vmemRef, ProcessMgr *procRef);
    void doReset();
    void SetFlag(long v);
    void ClearFlag(long v);
    void IsFlagSet(long v);

};

#endif //ONYX1_CPUCORE_H
