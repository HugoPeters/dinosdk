#include "../hackInfo.h"
#include "di_imports.h"
#include "di_imgui.h"
#include "../utils.h"

#define PRINT_HOOK_ADDR 0x80060b34

HackInfo gHackEnablePrint;

void printHook(const char* fmt, ...)
{
    logSetColor(0, 0, 0, 0x80);
    
    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, NULL, fmt, vlist);
    va_end(vlist);
    logEndLine();
}

void hackEnablePrint_Activate()
{
    // remove sp push because we will not be poppin it
    (*(uint32*)0x80060b20) = 0;

    // the jump, does not modify $ra so end of jumped func will actually return to this parent function
    (*(uint32*)0x80060b24) = makeOp_J(&printHook);

    // don't do any funny shit in the delay slot
    (*(uint32*)0x80060b28) = 0;
}

void hackEnablePrint_Deactivate()
{
    (*(uint32*)PRINT_HOOK_ADDR) = 0;
}

HackInfo* hackEnablePrint_Register()
{
    gHackEnablePrint.menuName = "enable print";
    gHackEnablePrint.onActivate = hackEnablePrint_Activate;
    gHackEnablePrint.onDeactivate = hackEnablePrint_Deactivate;
    return &gHackEnablePrint;
}