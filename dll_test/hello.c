#include "di_imports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"
#include "di_pad.h"
#include "di_imgui.h"
#include "di_warp.h"
#include "di_bootMenu.h"
#include "utils.h"
#include "freecam.h"

#include "menuPageTypes.h"

#define THIS_DLL_ID 79

// this is simply a proof of concept, please don't take it too serious

// todo: get rid of libc standard stuff so we dont have to name things "printf2" to avoid warnings

static DebugMenuState gMenuState;

void frameHook();

extern void doMenuPage_Camera();
extern void doMenuPage_MovePlayer();

extern void hacksRegister();
extern void hacksUpdateMenu();
extern void hacksUpdate();
extern void hacksUpdateMenuPage();

typedef void (*dllCtor)(void*);

void dllHook()
{
    uint32 dllAddr; 
    uint32 dllNumExports = 0;
    __asm__ ("sw $a0, %0" : "=m" (dllAddr));
    __asm__ ("sw $a3, %0" : "=m" (dllNumExports));

    if (dllNumExports > 0)
    {
        dllCtor ctorFunc = *(dllCtor*)(dllAddr + 0x10);
        ctorFunc((void*)dllAddr);
    }
}

void dllInstallHook()
{
    /*
        8000c088 11 40 00 06     beq        t2,zero,LAB_8000c0a4
        8000c08c 24 c6 00 08     _addiu     param_3,param_3,0x8
        8000c090 af a6 00 28     sw         param_3=>DAT_80917f18,0x28(sp)                   = 1FA21E33h
        8000c094 8c 99 00 10     lw         t9,0x10(param_1)
        8000c098 03 20 f8 09     jalr       t9
    */

    /* Base address */
    uint32* hookAddr = (uint32*)0x8000c088; 

    /* Kill all stuff + make a jump */
    *(hookAddr++) = 0;
    *(hookAddr++) = 0;
    *(hookAddr++) = 0;
    *(hookAddr++) = 0;
    *(hookAddr++) = makeOp_JAL(&dllHook);
}

void DLL_EXPORT onLoad(void* aThis)
{
    padInit();
    imguiInit();
    imguiInitMenuState(&gMenuState);

    // need to spend time doing a proper frame hook, instead of overriding some address
    *((uint32*)0x805173B4) = (uint32)&frameHook;

    // hook the "dll load" function so we get a callback anytime a DLL is loaded, so we can patch
    // dllInstallHook();

    // hook the menu DLL IDs to redirect to this DLL for main menu functionality!
    bootMenuInstall(THIS_DLL_ID);

    hacksRegister();
}

void DLL_EXPORT onUnload(void* aThis)
{
}

// these are required as this DLL expects at least 3 public functions
int DLL_EXPORT func1()
{
    return 0;
}

int DLL_EXPORT func2()
{
    return 0;
}

int DLL_EXPORT func3()
{
	// return 1 to disable game loop, 0 to enable
    return bootMenuUpdate();
}

int DLL_EXPORT func4()
{
    return 1;
}

void doLockPlayerToggle()
{
    static int lockPlayer = 1;
    if (menuCheckbox("lock player", &lockPlayer))
    {
        gMenuState.lockPlayer = lockPlayer == 1 ? true : false;
    }
}

void doMenuPage_Gameplay()
{
    beginMenu();

    ObjectInstance* player = getPlayer();
    menuTitle("gameplay - player: %s", player ? player->objDef->name : "NO PLAYER!");

    if (menuOption("move player"))
        menuPageSetNext(kMenuPage_GameplayMovePlayer);

    hacksUpdateMenu();

    doLockPlayerToggle();

    endMenu();
}

void doMenuPage_Objects()
{
    int numObjs;
    ObjectInstance** objectList = getObjectsList(NULL, &numObjs);

    menuTitle("objects (%i)", numObjs);

    beginMenu();

    for (int i = 0; i < numObjs; ++i)
    {
        static char optionName[32];
        snprintf2(optionName, sizeof(optionName), "%i: %s", i, objectList[i]->objDef->name);
        menuOption(optionName);
    }

    endMenu();
}

void doMenuPage_FreeCamera()
{
    freecamPatchFuncs();

    if (padIsButtonPressed(kButtonB))
    {
        freecamExit();
        menuPageSetPrev();
    }
}

void doMenuPage_Warp()
{
    beginMenu();

    menuTitle("warp");

    for (int i = 0; i < warpGetNumEntries(); ++i)
    {
        const WarpInfo* warpInfo = warpGetInfo(i);

        char optionName[64];
        snprintf2(optionName, sizeof(optionName), "%i: %s", warpInfo->index, warpInfo->name);

        if (menuOption(optionName))
            warpPlayer(warpInfo->index, 1);
    }

    endMenu();
}

void frameHook()
{
    padUpdate();
    hacksUpdate();

    if (imguiBegin(&gMenuState, kMenuPage_NumMainPages, kMenuPage_MainGameplay))
    {
        menuSetLines(16);

        switch (gMenuState.curPage)
        {
            case kMenuPage_MainGameplay: 		doMenuPage_Gameplay(); break;
            case kMenuPage_MainObjects: 		doMenuPage_Objects(); break;
            case kMenuPage_MainCamera: 		    doMenuPage_Camera(); break;
        }

        switch (gMenuState.curPage)
        {
            case kMenuPage_GameplayMovePlayer: 	doMenuPage_MovePlayer(); break;
            case kMenuPage_MainWarp: 		    doMenuPage_Warp(); break;
            case kMenuPage_CameraFreeCam: 		doMenuPage_FreeCamera(); break;

            default:                            hacksUpdateMenuPage(); break;
        }

        imguiEnd();
    }
}