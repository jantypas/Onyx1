//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_CPUCORE_H
#define ONYX1_CPUCORE_H
#include "../Onyx1Const.h"
#include <stdint-gcc.h>

const   uint32_t    REG_PC          = 0xBF;     // Processor PC
const   uint32_t    REG_SP          = 0Xbe;     // Stack pointer
const   uint32_t    REG_INTR_MASK   = 0xbd;     // Interrupt mask
const   uint32_t    REG_CONFIG      = 0xbc;     // Configuration mask
const   uint32_t    FLAGS_CPU       = 0xFF;     // Processor flags

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
