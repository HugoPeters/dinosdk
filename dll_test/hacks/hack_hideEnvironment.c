#include "../hackInfo.h"

#define ADDR_ENV_RENDERING 0x80042b58

HackInfo gHackHideEnv;
uint32 disableEnvRenderingOp;

void hackHideEnvironment_Activate()
{
    disableEnvRenderingOp = *(uint32*)ADDR_ENV_RENDERING; 
    *(uint32*)ADDR_ENV_RENDERING = 0;
}

void hackHideEnvironment_Deactivate()
{
    *(uint32*)ADDR_ENV_RENDERING = disableEnvRenderingOp;
    disableEnvRenderingOp = 0;
}

HackInfo* hackHideEnvironment_Register()
{
    gHackHideEnv.menuName = "hide environment";
    gHackHideEnv.onActivate = hackHideEnvironment_Activate;
    gHackHideEnv.onDeactivate = hackHideEnvironment_Deactivate;
    return &gHackHideEnv;
}