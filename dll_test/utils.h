#ifndef _UTILS_H_
#define _UTILS_H_

#include "di_base.h"

// jump with return address
inline uint32 makeOp_JAL(void* addr)
{
    uint32 instr = 0x0C000000;
    instr |= (((uint32)addr) - 0x80000000) / 4;
    return instr;
}

// simple jump
inline uint32 makeOp_J(void* addr)
{
    uint32 instr = (2 << 26);
    instr |= ((((uint32)addr) - 0x80000000) >> 2);
    return instr;
}

#endif // _UTILS_H_