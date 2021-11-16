//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_ONYX1CONST_H
#define ONYX1_ONYX1CONST_H
#include <stdint-gcc.h>

#define MAX_REGISTERS       0xFF                // 256 registers !!SHOULD NOT BE CHANGED WITHOUT CODE REWORK!!
#define MAX_PAGE_SIZE       0xFFF               // 4096 64-bit words

#define MIN_VIRTUAL_PAGES   0xFF              // 64KW
#define MAX_VIRTUAL_PAGES   0xFFFF'FFFF         // 4.2GW
#define MIN_PHYSICAL_PAGES  0xFF            // 64KW
#define MAX_PHYSICAL_PAGES  0xFFFF'FFFF         // 4GW
#define MAX_PAGE_BITS       1024*1024
#define PROC_MSG_SIZE       128

#define SWAPFILE_NAME       "~/swapfile.swp"
#define SWAP_MINIMUM_PAGES  4

#endif //ONYX1_ONYX1CONST_H
