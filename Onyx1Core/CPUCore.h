//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_CPUCORE_H
#define ONYX1_CPUCORE_H
#include "Onyx1Const.h"

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
