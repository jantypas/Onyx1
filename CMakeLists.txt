cmake_minimum_required(VERSION 3.20)
project(Onyx1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Onyx1 main.cpp
        src/Onyx1Core/CPUCore.cpp src/Onyx1Core/CPUCore.h
        src/Onyx1Core/CPUError.h
        src/Onyx1Core/Onyx1Const.h
        src/Onyx1Core/Onyx1Instr.cpp src/Onyx1Core/Onyx1Instr.h
        src/Onyx1Core/VMemMgr.cpp src/Onyx1Core/VMemMgr.h src/Onyx1Core/PageTable.cpp src/Onyx1Core/PageTable.h)
