#include "di_bootMenu.h"
#include "di_imports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"
#include "di_pad.h"
#include "di_imgui.h"
#include "di_warp.h"
#include "stdint.h"

#define BMS_BOOT 0
#define BMS_RUN_WORLDINIT 1
#define BMS_POST_WORLDINIT 2
#define BMS_DO_WARP 3

static DebugMenuState gBootMenuState;
static int blockGameLoop = 1;
static int menuState = 0;
static int gTargetWarpIdx = -1;

typedef enum _BootMenuPage
{
    kBootMenu_Main,

    kBootMenu_NumPages
} BootMenuPage;

void bootMenuInstall(int dllId)
{
    int* menuDllIds = (int*)0x8008c840;
    menuDllIds[2] = dllId + 1;

    imguiInitMenuState(&gBootMenuState);
    enterDebugMenu(&gBootMenuState, kBootMenu_NumPages, kBootMenu_Main);
    blockGameLoop = 1;
    menuState = BMS_BOOT;
    gTargetWarpIdx = -1;
}

void bootMenu_MainPage()
{
    beginMenu();

    menuSetLines(15);

    logSetPos(100, 10);
    logSetColor(255, 0, 0, 255);

    printLine("- DinoLauncher v1.0 -");

    logSetPos(10, 30);
    logSetColor(0, 0, 0, 0x80);

    menuTitle("BOOT MENU (USE DPAD)");

    // if (menuOption("initworld + game loop"))
    // {
    //     menuState = BMS_RUN_WORLDINIT;
    // }

    if (menuOption("Go to save select"))
    {

    }

    if (menuOption("Go to intro"))
    {

    }

    if (menuOption("Normal boot"))
    {

    }

    menuText(" ");
    menuText("Quick warp (no save):");

    for (int i = 0; i < warpGetNumEntries(); ++i)
    {
        const WarpInfo* warpInfo = warpGetInfo(i);

        char optionName[64];
        snprintf2(optionName, sizeof(optionName), "%s", warpInfo->name);

        if (menuOption(optionName))
        {
            menuState = BMS_RUN_WORLDINIT;
            gTargetWarpIdx = warpInfo->index;
        }
    }

    endMenu();
}

int bootMenuUpdate()
{
    // TODO: only run this if world is not busy
    clearScreen((void**)0x800ae680, (void**)0x800ae680, 1);

    if (blockGameLoop == 0)
        return blockGameLoop;

    padUpdate();

    if (imguiBegin(&gBootMenuState, kBootMenu_NumPages, kBootMenu_Main))
    {
        switch (gBootMenuState.curPage)
        {
            case kBootMenu_Main: 		bootMenu_MainPage(); break;
        }

        imguiEnd();
    }

    switch (menuState)
    {
        case BMS_RUN_WORLDINIT:
        {
            void (*someInitGame)() = 0x80013fb4;
            someInitGame();

            menuState = BMS_POST_WORLDINIT;
            break;
        }

        case BMS_POST_WORLDINIT:
        {

            void (*ssetMenuPage)(int id) = 0x8000F404;
            void (*initTickVar)() = 0x8001442c;
            void (*unkBootFunc)(int id1, int id2) = 0x8001125c;
            
            ssetMenuPage(1);
            initTickVar();
            unkBootFunc(0, 0xC000);

            void* (*someGetUnk0)() = 0x80014604;
            void (*someSetupScreenMode)(int mode, void* unk0, int unk1) = 0x8005d410;

            someSetupScreenMode(1, someGetUnk0(), 0);

            void (*someEnableClrscr)(int en) = 0x80041d20;
            void (*someEnableRenderFlag)(int en) = 0x80041c6c;
            void (*someResetTickVar)() = 0x8001442c;

            someEnableClrscr(1);
            someEnableRenderFlag(1);
            someResetTickVar();

            blockGameLoop = 0;

            if (gTargetWarpIdx != -1)
            {
                warpPlayer(gTargetWarpIdx, 1);
            }

            menuState = BMS_DO_WARP;
            break;
        }

        case BMS_DO_WARP:
        {


            break;
        }
    }

    return blockGameLoop;
}