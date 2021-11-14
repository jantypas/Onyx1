//
// Created by jantypas on 11/13/21.
//

#ifndef ONYX1_FUFI_H
#define ONYX1_FUFI_H
#include <stdint-gcc.h>
#include "../CPU/IOHandler.h"

const unsigned long FUFI_ID = 0x0000'0000'0000'0000;

class FUFI_Serivce : public IOService {
    std::string getName();
    std::string getDescription();
    int32_t     serviceInit();
    int32_t serviceTerminate();
    int32_t serviceSend(IORequest *req);
    int32_t serviceCancel(uint64_t ticket);
    IORequest *serviceReceive();
};

#endif //ONYX1_FUFI_H
