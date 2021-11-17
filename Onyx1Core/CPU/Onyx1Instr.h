//
// Created by jantypas on 10/15/21.
//

#ifndef ONYX1_ONYX1INSTR_H
#define ONYX1_ONYX1INSTR_H


class Onyx1InstrOps {
    // Control instructions
    const unsigned short INSTR_STOP         = 0x0000;   // Stop the processor where ever it happens to be (STOP)
    const unsigned short INSTR_EXIT         = 0x0001;   // THe process is done and can be unloaded if needed (EXIT)
    const unsigned short INSTR_CONFIG       = 0x0002;   // CHange processor configuration state (SUPERVISOR) (CONFIG)

    // Memory instructions
    const unsigned short INSTR_MOVEL        = 0x0010;   // Move literal value
    const unsigned short INSTR_MOVE         = 0x0011;   // Move memory from one place to another
    const unsigned short INSTR_PCOPY        = 0x0012;   // Page copy
    const unsigned short INSTR_COPYN        = 0x0013;   // Copy N words
    const unsigned short INSTR_COPYV        = 0x0014;   // Copy until we see a value
    const unsigned short INSTR_COMPN        = 0x0015;   // Compare N words
    const unsigned short INSTR_COMPV        = 0x0016;   // Compare until we see a value
    const unsigned short INSTR_REGINC       = 0x0017;   // Increment register by one
    const unsigned short INSTR_REGDEC       = 0x0018;   // Decrement register by one
    const unsigned short INSTR_REGDISPLACE  = 0x0019;   // Displacement move (rX = (Ax+ry)
    const unsigned short INSTR_SWAPREG      = 0x001A;   // Swap register set

    // 64-bit math
    const unsigned short INSTR_IADD64       = 0x0020;   // Add
    const unsigned short INSTR_ISUB64       = 0x0021;   // Subtract
    const unsigned short INSTR_IMULT64      = 0x0022;   // Multiply
    const unsigned short INSTR_IDIV64       = 0x0023;   // Divide
    const unsigned short INSTR_IMOD64       = 0x0024;   // Modulus
    const unsigned short INSTR_IAND64       = 0x0025;   // Logical AND
    const unsigned short INSTR_IOR64        = 0x0026;   // Logical OR
    const unsigned short INSTR_IXOR64       = 0x0027;   // Logical XOR
    const unsigned short INSTR_ISHIFTL64    = 0x0028;   // Shift Left
    const unsigned short INSTR_ISHIFTR64    = 0x0029;   // Shift Right
    const unsigned short INSTR_INOT64       = 0x002A;   // Logical NOT

    // Calls and branches
    const unsigned short INSTR_CALL         = 0x0030;   // Call a subroutine
    const unsigned short INSTR_RETURN       = 0x0031;   // Return from a subroutine
    const unsigned short INSTR_COMPARE      = 0x0032;   // Compare values
    const unsigned short INSTR_BRRT         = 0x0033;   // Branch relative if true
    const unsigned short INSTR_BRAT         = 0x0034;   // Branch absolute if true
    const unsigned short INSTR_BRRF         = 0x0035;   // Branch relative if false
    const unsigned short INSTR_BRAF         = 0x0036;   // Branch absolute if false
    const unsigned short INSTR_JUMPA        = 0x0037;   // Jump absolute
    const unsigned short INSTR_JUMPR        = 0x0038;   // Jump relative
    const unsigned short INSTR_PUSH         = 0x0039;   // Push on stack
    const unsigned short INSTR_POP          = 0x003A;   // Pop from stack
    const unsigned short INSTR_SUB1BR       = 0x003B;   // Subtract 1 and branch relative if 0
    const unsigned short INSTR_SUB1BA       = 0x003C;   // Subtract 1 and branch absolute if 0

    // IO and Coprocessor functions
    const unsigned short INSTR_IOREQ        = 0x0040;   // Request I/O
    const unsigned short INSTR_IORET        = 0x0041;   // Return from I/O
    const unsigned short INSTR_IOCANCEL     = 0x0042;   // Cancel an I/O request
    const unsigned short INSTR_SYSCALL      = 0x0043;   // Perform a system call
    const unsigned short INSTR_SYSRET       = 0x0044;   // Return from a system call
    const unsigned short INSTR_COPROC       = 0x0045;   // Perform a coprocessor call
    const unsigned short INSTR_CORET        = 0x0046;   // Return from the coprocessor
    const unsigned short INSTR_COCANCEL     = 0x0047;   // Cancel the coprocessor call
    const unsigned short INSTR_WAIT4IO      = 0x0048;   // Wait on I/O
    const unsigned short INSTR_INTR         = 0x0049;   // Perform an interrupt
    const unsigned short INSTR_IRET         = 0x004A;   // Return from interrupt
    const unsigned short INSTR_WAITI        = 0x004B;   // Wait on interrupt

    // Multitasking
    const unsigned short INSTR_YIELD        = 0x0050;   // Yield the processor
    const unsigned short INSTR_SWAPOUT      = 0X0051;   // Swap the process out
    const unsigned short INSTR_SLEEP        = 0x0052;   // Sleep for a period of time
    const unsigned short INSTR_GETPID       = 0x0053;   // Get process ID
    const unsigned short INSTR_ISSUPER      = 0x0054;   // Is supervisor ready
    const unsigned short INSTR_DOSUPER      = 0x0055;   // Activate supervisor privs
    const unsigned short INSTR_DOUSER       = 0x0056;   // Release supervisor privs
    const unsigned short INSTR_FORK         = 0x0057;   // Fork a process
    const unsigned short INSTR_PKILL        = 0x0058;   // Kill a process
    const unsigned short INSTR_FPID         = 0x0059;   // Find a process by name

    // Messaging
    const unsigned short INSTR_SENDMSG      = 0x0060;   // Send a message between processes
    const unsigned short INSTR_RECVMSG      = 0x0061;   // Receive a message
    const unsigned short INSTR_MSGEMPTY     = 0x0062;   // Are there messages for me
    const unsigned short INSTR_MSGFLUSH     = 0x0063;   // Flush message queue

    // Special functions
    const unsigned short INSTR_CRC          = 0x00070;  // Compute CRC value
};



#endif //ONYX1_ONYX1INSTR_H
