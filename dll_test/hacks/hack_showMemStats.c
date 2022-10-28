#include "../hackInfo.h"
#include "di_exports.h"

HackInfo gHackShowMemStats;

void hackShowMemStats_Update()
{
    if (gHackShowMemStats.isActive == FALSE)
        return;

    void (*printMemStats)() = 0x80017a24;
    printMemStats();
}

HackInfo* hackShowMemStats_Register()
{
    gHackShowMemStats.menuName = "show memory stats";
    gHackShowMemStats.onUpdate = hackShowMemStats_Update;
    return &gHackShowMemStats;
}