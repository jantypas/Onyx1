//
// Created by jantypas on 10/31/21.
//

#ifndef ONYX1_VMGRSWAP_H
#define ONYX1_VMGRSWAP_H
#include <fstream>

class VMgrSwap {
public :
    uint64_t swapIns    = 0;
    uint64_t swapOuts   = 0;
    std::fstream swapfile;
    bool open();
    bool close();
    int32_t doSwapping(uint32_t desiredFreePages);
};


#endif //ONYX1_VMGRSWAP_H
