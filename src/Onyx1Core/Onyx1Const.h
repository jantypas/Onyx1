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
#define SWAPFILE_NAME       "~/swapfile.swp"
#define SWAP_MINIMUM_PAGES  4
#define MAX_PAGE_BITS       1024*1024

#endif //ONYX1_ONYX1CONST_H
