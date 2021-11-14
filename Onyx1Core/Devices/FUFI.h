//
// Created by jantypas on 11/13/21.
//

#ifndef ONYX1_FUFI_H
#define ONYX1_FUFI_H
#include <stdint-gcc.h>
#include "../CPU/IOHandler.h"

const unsigned long FUFI_ID = 0x0000'0000'0000'0000;

class FUFI_Serivce : public IOService {
    int32_t serviceInit() { return 0; };
    int32_t serviceTerminate() { return 0; };
    int32_t serviceSend(IORequest *req) { return 0; };
    int32_t serviceCancel(uint64_t ticket) { return 0; };
    IORequest *serviceReceive() { return nullptr; };
};

#endif //ONYX1_FUFI_H
