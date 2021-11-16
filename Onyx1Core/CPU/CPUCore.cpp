//
// Created by jantypas on 10/15/21.
//

#include "CPUCore.h"

void CPUCore::doReset() {
    for (auto ix : registers) { registers[ix] = 0; };
    registers[REG_CONFIG]       = 0x0000'0000'0000'0000;
    registers[REG_INTR_MASK]    = 0x0000'0000'0000'0000;
    registers[REG_PC]           = 0x0000'0000'0000'0000;
    registers[REG_SP]           = 0x0000'0000'0000'0000;
    registers[FLAGS_CPU]        = 0x0000'0000'0000'0000;
}
