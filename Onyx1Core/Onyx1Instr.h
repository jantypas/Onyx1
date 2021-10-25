//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_ONYX1INSTR_H
#define ONYX1_ONYX1INSTR_H


class Onyx1InstrOps {
    const unsigned short INSTR_STOP         = 0x0000;
    const unsigned short INSTR_EXIT         = 0x0001;
    const unsigned short INSTR_CONFIG       = 0x0002;

    const unsigned short INSTR_MOVEL        = 0x0010;
    const unsigned short INSTR_MOVE         = 0x0011;
    const unsigned short INSTR_PCOPY        = 0x0012;
    const unsigned short INSTR_COPYN        = 0x0013;
    const unsigned short INSTR_COPYV        = 0x0014;
    const unsigned short INSTR_COMPN        = 0x0015;
    const unsigned short INSTR_COMPV        = 0x0016;
    const unsigned short INSTR_REGINC       = 0x0017;
    const unsigned short INSTR_REGDEC       = 0x0018;
    const unsigned short INSTR_REGDISPLACE  = 0x0019;

    const unsigned short INSTR_IADD64       = 0x0020;
    const unsigned short INSTR_ISUB64       = 0x0021;
    const unsigned short INSTR_IMULT64      = 0x0022;
    const unsigned short INSTR_IDIV64       = 0x0023;
    const unsigned short INSTR_IMOD64       = 0x0024;
    const unsigned short INSTR_IAND64       = 0x0025;
    const unsigned short INSTR_IOR64        = 0x0026;
    const unsigned short INSTR_IXOR64       = 0x0027;
    const unsigned short INSTR_ISHIFTL64    = 0x0028;
    const unsigned short INSTR_ISHIFTR64    = 0x0029;

    const unsigned short INSTR_CALL         = 0x0030;
    const unsigned short INSTR_RETURN       = 0x0031;
    const unsigned short INSTR_COMPARE      = 0x0032;
    const unsigned short INSTR_BRRT         = 0x0033;
    const unsigned short INSTR_BRAT         = 0x0034;
    const unsigned short INSTR_BRRF         = 0x0035;
    const unsigned short INSTR_BRAF         = 0x0036;
    const unsigned short INSTR_JUMPA        = 0x0037;
    const unsigned short INSTR_JUMPR        = 0x0038;
    const unsigned short INSTR_PUSH         = 0x0039;
    const unsigned short INSTR_POP          = 0x003A;

    const unsigned short INSTR_IOREQ        = 0x0040;
    const unsigned short INSTR_IORET        = 0x0041;
    const unsigned short INSTR_IOCANCEL     = 0x0042;
    const unsigned short INSTR_SYSCALL      = 0x0043;
    const unsigned short INSTR_SYSRET       = 0x0044;
    const unsigned short INSTR_COPROC       = 0x0045;
    const unsigned short INSTR_CORET        = 0x0046;
    const unsigned short INSTR_COCANCEL     = 0x0047;
    const unsigned short INSTR_WAIT4IO      = 0x0048;
    const unsigned short INSTR_INTR         = 0x0049;
    const unsigned short INSTR_IRET         = 0x004A;
    const unsigned short INSTR_WAITI        = 0x004B;

    const unsigned short INSTR_YIELD        = 0x0050;
    const unsigned short INSTR_SWAPOUT      = 0X0051;
    const unsigned short INSTR_SLEEP        = 0x0052;

    const unsigned short INSTR_CRC          = 0x00060;
};



#endif //ONYX1_ONYX1INSTR_H
