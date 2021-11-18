//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_CPUCORE_H
#define ONYX1_CPUCORE_H
#include <queue>
#include "../Onyx1Const.h"
#include <stdint-gcc.h>

const   uint32_t    REG_PC          = 0x7F;     // Processor PC
const   uint32_t    REG_SP          = 0X7E;     // Stack pointer

const   uint32_t    REG_INTR_MASK   = 0xBF;     // Interrupt mask
const   uint32_t    REG_CONFIG      = 0xBE;     // Configuration mask
const   uint32_t    REG_BASETIME    = 0xBD;     // Base timer value
const   uint32_t    REG_INSTRCOUNT  = 0xBC;     // Base instruction count
const   uint32_t    REG_IDISPATCH   = 0xBB;     // Interrupt dispatch table address
const   uint32_t    REG_TRAPADDR    = 0xBA;     // Trap dispatch address
const   uint32_t    REG_BUFFERPAGE  = 0xB9;     // Buffer page structure address

const   uint32_t    FLAGS_CPU       = 0x80;     // Processor flags
const   uint32_t    FLAGS_IOSERVICE = 0x81;     // Who needs IO Service

class RegFile {
    long dRegisters[8];
    long aRegisters[8];
};

class CPUCore {
    long                registersA[MAX_REGISTERS];
    long                registersB[MAX_REGISTERS];
    long                *currentRegisters;
    std::queue<RegFile> regQueue;

    void doReset();
    void SetFlag(long v);
    void ClearFlag(long v);
    void IsFlagSet(long v);

};

class CPUContext {

};

#endif //ONYX1_CPUCORE_H
