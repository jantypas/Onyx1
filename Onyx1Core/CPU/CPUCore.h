//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_CPUCORE_H
#define ONYX1_CPUCORE_H
#include "../Onyx1Const.h"
#include <stdint-gcc.h>

const   uint32_t    REG_PC          = 0xBF;     // Processor PC
const   uint32_t    REG_SP          = 0XBE;     // Stack pointer
const   uint32_t    REG_INTR_MASK   = 0xBD;     // Interrupt mask
const   uint32_t    REG_CONFIG      = 0xBC;     // Configuration mask
const   uint32_t    REG_BASETIME    = 0xBB;     // Base timer value
const   uint32_t    REG_INSTRCOUNT  = 0xBA;     // Base instruction count
const   uint32_t    REG_IDISPATCH   = 0xB9;     // Interrupt dispatch table address
const   uint32_t    REG_TRAPADDR    = 0xB8;     // Trap dispatch address
const   uint32_t    REG_BUFFERPAGE  = 0xB7;     // Buffer page structure address
const   uint32_t    CPU_INFO        = 0xB6;     // CPU Info page
const   uint32_t    MEM_LIMITS      = 0xB5;     // Memory limit control

const   uint32_t    FLAGS_CPU       = 0xFF;     // Processor flags
const   uint32_t    FLAGS_IOSERVICE = 0xFE;     // Who needs IO Service

class CPUCore {
    long registers[MAX_REGISTERS];

    void doReset();
    void SetFlag(long v);
    void ClearFlag(long v);
    void IsFlagSet(long v);

};

class CPUContext {

};

#endif //ONYX1_CPUCORE_H
