cmake_minimum_required(VERSION 3.20)
project(Onyx1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Onyx1 main.cpp
        Onyx1Core/CPUCore.cpp Onyx1Core/CPUCore.h
        Onyx1Core/CPUError.h
        Onyx1Core/Onyx1Const.h
        Onyx1Core/Onyx1Instr.cpp Onyx1Core/Onyx1Instr.h
        Onyx1Core/VMemMgr.cpp Onyx1Core/VMemMgr.h Onyx1Core/PageTable.cpp Onyx1Core/PageTable.h)
