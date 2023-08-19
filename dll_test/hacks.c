#include "di_imports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"
#include "di_pad.h"
#include "di_imgui.h"
#include "hackInfo.h"
#include "menuPageTypes.h"

extern HackInfo* hackHideEnvironment_Register();
extern HackInfo* hackShowHiddenObjects_Register();
extern HackInfo* hackShowMemStats_Register();
extern HackInfo* hackEnablePrint_Register();

typedef struct _HacksState
{
    int showInfo;
    int forceObjectsUseDefaultRendering;
} HacksState;

static HacksState gHacks;
static HackInfo* gRootHack;

void hacksShowInfo()
{
    ObjectInstance* player = getPlayer();
    printLine("currentState @ 0x%08X: %i", &player->playerState->currentStateId, player->playerState->currentStateId);
    printLine("currentAnimId: %i", player->currentAnimId);
    printLine("animProgress: %f", player->animProgress);
    printLine("gGameState: %i", gGameState);
}

void addHack(HackInfo*(*registerFunc)())
{
    HackInfo* info = registerFunc();

    if (gRootHack == NULL)
        gRootHack = info;
    else
    {
        HackInfo* lastHack = gRootHack;
        while (lastHack->next)
            lastHack = lastHack->next;
        lastHack->next = info;
    }
}

void hacksRegister()
{
    addHack(hackEnablePrint_Register);
    addHack(hackHideEnvironment_Register);
    addHack(hackShowHiddenObjects_Register);
    addHack(hackShowMemStats_Register);
}

void hacksUpdateMenu()
{
    if (menuOption("select player"))
        menuPageSetNext(kMenuPage_SelectPlayer);

    if (menuOption("test flag"))
    {
        uint8* someFlagPtr = (uint8*)0x800b4a58;
        *someFlagPtr = 1;
    }

    menuCheckbox("show state info", &gHacks.showInfo);
    menuCheckbox("force default rendering", &gHacks.forceObjectsUseDefaultRendering);

    HackInfo* curHack = gRootHack;

    while (curHack)
    {
        bool activateStateChange = false;

        if (curHack->onMenuUpdate)
            activateStateChange = curHack->onMenuUpdate();
        else
            activateStateChange = menuCheckbox(curHack->menuName, &curHack->isActive);

        if (activateStateChange == TRUE)
        {
            if (curHack->isActive)
            {
                if (curHack->onActivate)
                    curHack->onActivate();
            }
            else
            {
                if (curHack->onDeactivate)
                    curHack->onDeactivate();
            }
        }

        curHack = (HackInfo*)curHack->next;
    }
}

void hacksUpdate()
{
    HackInfo* curHack = gRootHack;

    while (curHack)
    {
        if (curHack->onUpdate)
            curHack->onUpdate();

        curHack = (HackInfo*)curHack->next;
    }

    if (gHacks.showInfo)
        hacksShowInfo();

    if (gHacks.forceObjectsUseDefaultRendering)
    {
        int numObjectInstances;
        ObjectInstance** objInsts = getObjectsList(NULL, &numObjectInstances);

        for (int i = 0; i < numObjectInstances; ++i)
        {
            ObjectInstance* obj = objInsts[i];

            // force all objects to use default rendering (except for krystal and sabre) - this will show more debug objects like TriggerPlanes
            if (obj->objectTypeId == 0 || obj->objectTypeId == 31)
                continue;

            obj->someFlags2 |= 0x4000;
        }
    }
}

void hacksUpdateMenuPage()
{
    switch (menuPageGetCurrent())
    {
        case kMenuPage_SelectPlayer:
        {
            if (padIsButtonPressed(kButtonB))
                menuPageSetPrev();

            beginMenu();
            printLine("select player");

            if (menuOption("Krystal"))
            {
                spawnPlayer(kPlayerTypeKrystal);
                menuPageSetNext(-1);
            }

            if (menuOption("Sabre"))
            {
                spawnPlayer(kPlayerTypeSabre);
                menuPageSetNext(-1);
            }

            endMenu();
            break;
        }
    }
}