//
// Created by jantypas on 11/10/21.
//

#ifndef ONYX1_IOCNTRL_H
#define ONYX1_IOCNTRL_H
#include <stdint-gcc.h>
#include <string>
#include <map>
#include <queue>
#include "../Onyx1Const.h"
#include "CPUError.h"
#include "IOHandler.h"
#include "../Devices/FUFI.h"

class IOCtrl {
public :
    std::map<uint64_t, IOService *> ioMap ;

    IOCtrl() {
        ioMap.emplace(FUFI_ID, new FUFI_Serivce());
    }
};

#endif //ONYX1_IOCNTRL_H
