//
// Created by jantypas on 11/13/21.
//

#include "FUFI.h"
#include "../CPU/CPUError.h"

std::string FUFI_Serivce::getName() {
    return "FUFI";
}

std::string FUFI_Serivce::getDescription() {
    return "FUFI Bios Service";
}

int32_t FUFI_Serivce::serviceInit() {
    return CPUError_None;
}

int32_t FUFI_Serivce::serviceTerminate() {
    return CPUError_None;
}

int32_t FUFI_Serivce::serviceSend(IORequest *req) {
    return CPUError_None;
}

int32_t FUFI_Serivce::serviceCancel(uint64_t ticket) {
    return CPUError_None;
}

IORequest *FUFI_Serivce::serviceReceive() {
    return nullptr;
}
