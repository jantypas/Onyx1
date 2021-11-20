//
// Created by jantypas on 10/15/21.
//

#include "CPUCore.h"

void CPUCore::connectReferences(VMemMgr *vmemRef, ProcessMgr *procRef) {
    memPtr = vmemRef;
    procPtr = procRef;
}

void CPUCore::doReset() {
    for (auto ix : context.registersA) { context.registersA[ix] = 0; };
    for (auto ix : context.registersB) { context.registersB[ix] = 0; };
    context.currentRegisters = (long *)&context.registersA;

    context.currentRegisters[REG_CONFIG]       = 0x0000'0000'0000'0000;
    context.currentRegisters[REG_INTR_MASK]    = 0x0000'0000'0000'0000;
    context.currentRegisters[REG_PC]           = 0x0000'0000'0000'0000;
    context.currentRegisters[REG_SP]           = 0x0000'0000'0000'0000;
    context.currentRegisters[FLAGS_CPU]        = 0x0000'0000'0000'0000;
}
