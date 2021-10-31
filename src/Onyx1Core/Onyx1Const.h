//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_ONYX1CONST_H
#define ONYX1_ONYX1CONST_H
#include <stdint-gcc.h>

#define MAX_REGISTERS       0xFF                // 256 registers !!SHOULD NOT BE CHANGED WITHOUT CODE REWORK!!
#define MAX_PAGE_SIZE       0xFFF               // 4096 64-bit words
#define MAX_VIRTUAL_PAGES   0xFFFF'FFFF         // 4.2GW
#define MIN_VIRTUAL_PAGES   0xFFFF              // 64KW
#define MAX_PHYSICAL_PAGES  0xFFFF'FFFF         // 4GW
#define MIN_VIRTUAL_PAGES   0x0'FFFF            // 64KW


const uint64_t PRIV_KERNEL     = 0x0000'0000'0000'0001L;
const uint64_t PRIV_READABLE   = 0x0000'0000'0000'0002l;
const uint64_t PRIV_WRITABLE   = 0x0000'0000'0000'0004L;
const uint64_t PRIV_EXECUTABLE = 0x0000'0000'0000'0008L;

const uint32_t  PAGE_STATE_EMPTY    = 0x0000'0000;
const uint32_t  PAGE_STATE_INUSE    = 0x0000'0001;
const uint32_t  PAGE_STATE_LOCKED   = 0x0000'0002;
const uint32_t  PAGE_STATE_DIRTY    = 0x0000'0004;
const uint32_t  PAGE_STATE_ONDISK   = 0x0000'0008;

#endif //ONYX1_ONYX1CONST_H
