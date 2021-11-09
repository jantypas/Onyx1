//
// Created by jantypas on 10/16/21.
//

#ifndef ONYX1_CPUERROR_H
#define ONYX1_CPUERROR_H

const int CPUError_None                 = 0;
const int CPUError_NotImplemented       = -1;
const int CPUError_InvalidConfiguration = -2;
const int CPUError_MMUNotReady          = -3;
const int CPUError_InvalidProcess       = -4;
const int CPUError_InvalidPage          = -5;
const int CPUError_NoVirtualPages       = -6;
const int CPUError_ProcessNotReady      = -7;
const int CPUError_PageIsFree           = -8;
const int CPUError_CantSwap             = -9;
const int CPUError_NoPhysicalPages      = -10;
const int CPUError_PageSwapError        = -11;

#endif //ONYX1_CPUERROR_H
