//
// Created by jantypas on 10/15/21.
//

#include "CPUCore.h"

void CPUCore::doReset() {
    for (auto ix : registersA) { registersA[ix] = 0; };
    for (auto ix : registersB) { registersB[ix] = 0; };
    currentRegisters = (long *)&registersA;
    while (!regQueue.empty()) { regQueue.pop(); };

    currentRegisters[REG_CONFIG]       = 0x0000'0000'0000'0000;
    currentRegisters[REG_INTR_MASK]    = 0x0000'0000'0000'0000;
    currentRegisters[REG_PC]           = 0x0000'0000'0000'0000;
    currentRegisters[REG_SP]           = 0x0000'0000'0000'0000;
    currentRegisters[FLAGS_CPU]        = 0x0000'0000'0000'0000;
}
