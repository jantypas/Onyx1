//
// Created by jantypas on 11/11/21.
//

#ifndef ONYX1_IOHANDLER_H
#define ONYX1_IOHANDLER_H
#include <string>
#include <stdint-gcc.h>

class IORequest {
    uint32_t    procID;
    uint64_t    ticket;
    bool        urgent;
    uint32_t    numWords;
    uint64_t    *buffer;
};

class IOService {
public :
    virtual int32_t     serviceInit() = 0;
    virtual int32_t     serviceTerminate() = 0;
    virtual int32_t     serviceSend(IORequest *req) = 0;
    virtual int32_t     serviceCancel(uint64_t ticket) = 0;
    virtual IORequest   *serviceReceive() = 0;
};

class IOInfo {
public :
    std::string name;
    std::string description;
    IOService   *servicePtr;
    IOInfo(std::string pName, std::string pDescr, IOService *pService) {
        name = pName;
        description = pDescr;
        servicePtr = pService;
    };
};


#endif //ONYX1_IOHANDLER_H
