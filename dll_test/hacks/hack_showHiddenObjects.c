#include "../hackInfo.h"
#include "di_imports.h"

HackInfo gHackShowHiddenObj;

void hackShowHiddenObjects_Update()
{
    if (gHackShowHiddenObj.isActive == FALSE)
        return;

    for (uint32 i = 0; i < gNumObjectTypes; ++i)
    {
        Object* objType = gObjectTypesList[i];

        if (objType == NULL)
            continue;

        objType->setupFlags &= (~kObjectSetupFlag_DisableModels);
    }
}

HackInfo* hackShowHiddenObjects_Register()
{
    gHackShowHiddenObj.menuName = "show hidden objects";
    gHackShowHiddenObj.onUpdate = hackShowHiddenObjects_Update;
    return &gHackShowHiddenObj;
}