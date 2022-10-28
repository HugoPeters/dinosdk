#ifndef _HACK_INFO_H_
#define _HACK_INFO_H_

#include "di_base.h"

typedef struct HackInfo_
{
    void(*onActivate)();
    void(*onDeactivate)();
    void(*onUpdate)();
    bool(*onMenuUpdate)();
    const char* menuName;
    int isActive;
    void* next;
} HackInfo;

#endif // _HACK_INFO_H_