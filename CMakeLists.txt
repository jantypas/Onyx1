cmake_minimum_required(VERSION 3.20)
project(Onyx1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Onyx1 main.cpp
        Onyx1Core/CPU/CPUCore.cpp Onyx1Core/CPU/CPUCore.h
        Onyx1Core/CPU/CPUError.h
        Onyx1Core/Onyx1Const.h
        Onyx1Core/CPU/Onyx1Instr.cpp Onyx1Core/CPU/Onyx1Instr.h
        Onyx1Core/Memory/VMemMgr.cpp Onyx1Core/Memory/VMemMgr.h Onyx1Core/Memory/ProcessMgr.cpp Onyx1Core/Memory/ProcessMgr.h Onyx1Core/CPU/IOCntrl.cpp Onyx1Core/CPU/IOCntrl.h Onyx1Core/CPU/IOHandler.h Onyx1Core/Devices/FUFI.cpp Onyx1Core/Devices/FUFI.h Onyx1Core/Devices/CardUnit.cpp Onyx1Core/Devices/CardUnit.h Onyx1Core/Devices/DiskPack.cpp Onyx1Core/Devices/DiskPack.h Onyx1Core/Devices/LinePrinter.cpp Onyx1Core/Devices/LinePrinter.h Onyx1Core/Devices/LegacyTape.cpp Onyx1Core/Devices/LegacyTape.h Onyx1Core/Devices/LegacyFrontPanel.cpp Onyx1Core/Devices/LegacyFrontPanel.h Onyx1Core/Devices/LeasedLine.cpp Onyx1Core/Devices/LeasedLine.h)
