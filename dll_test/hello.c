#include "di_exports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"

// this is simply a proof of concept, please don't take it too serious

// todo: get rid of libc standard stuff so we dont have to name things "printf2" to avoid warnings

void initDebugMenu();
void frameHook();

void DLL_EXPORT onLoad(void* aThis)
{
    initDebugMenu();

    // need to spend time doing a proper frame hook, instead of overriding some address
    *((int*)0x805173B4) = &frameHook;
}

void DLL_EXPORT onUnload(void* aThis)
{
}

// these are required as this DLL expects at least 3 public functions
void DLL_EXPORT func1()
{
}

void DLL_EXPORT func2()
{
}

void DLL_EXPORT func3()
{
}

// gamepad functionality
static uint16 gPadButtons[2];
static int gPadRepeat;

static bool padIsButtonDown(PadButton button)
{
    return gPadButtons[0] & button;
}

static bool padIsButtonPressed(PadButton button)
{
    return (~gPadButtons[1] & gPadButtons[0]) & button;
}

static bool padIsButtonPressedRepeat(PadButton button)
{
    return false
        || padIsButtonPressed(button)
        || (padIsButtonDown(button) && gPadRepeat == 0);
}

static bool padIsButtonReleased(PadButton button)
{
    return (~gPadButtons[0] & gPadButtons[1]) & button;
}

// misc game stuff

inline void setGameStateId(GameState newId)
{
    gGameState = newId;
}

inline int getGameStateId()
{
    return gGameState;
}

// logging 

inline void logPutChar(char c)
{
    *(gDebugLog++) = c;
}

inline void logEndLine()
{
    logPutChar('\n');
    logPutChar(0);
}

void* printLogHandler(void* arg, const char* buff, uint32 length)
{
    for (uint32 i = 0; i < length; ++i)
        logPutChar(buff[i]);
    return (void*)1;
}

void printStr(const char* fmt, ...)
{
    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, 0, fmt, vlist);
    va_end(vlist);
}

void printLine(const char* fmt, ...)
{
    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, 0, fmt, vlist);
    va_end(vlist);
    logEndLine();
}

void printf2(const char* fmt, ...)
{
    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, 0, fmt, vlist);
    va_end(vlist);
    logPutChar(0x0);
}

typedef struct _snprintfState 
{
    char* buff;
    uint32 buffLeft;
} snprintfState;

void* snprintfHandler(void* arg, const char* buff, uint32 length)
{
    snprintfState* state =	(snprintfState*)arg;

    while (length > 0 && state->buffLeft > 0)
    {
        *(state->buff++) = *(buff++);
        --state->buffLeft;
        --length;
    }

    return state->buffLeft > 0 ? state : 0;
}

void snprintf2(char* buff, uint32 buffSize, const char* fmt, ...)
{
    if (buffSize == 0)
        return;

    if (buffSize == 1)
    {
        buff[0] = 0;
        return;
    }

    snprintfState state;
    state.buffLeft = buffSize - 1;
    state.buff = buff;
    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&snprintfHandler, &state, fmt, vlist);
    va_end(vlist);

    buff[state.buff - buff] = 0;
}

// menu
typedef struct _DebugMenuState
{
    int curPage;
    int prevPage;
    int numPages;
    int prevGameStateId;
    bool isAppearing;
    bool lockPlayer;
} DebugMenuState;

typedef struct _DebugMenuPageState
{
    DebugMenuState* menuState;
    int curEntry;
    int numEntries;
    int maxEntries;
    int scrollOffset;
    int selectedOption;
    int nextPage;
} DebugMenuPageState;

// utils
void beginMenu(DebugMenuPageState* page)
{
    page->curEntry = 0;
    page->numEntries = 0;
}

void endMenu(DebugMenuPageState* page)
{
    if (padIsButtonPressedRepeat(kButtonDPadUp))
        page->selectedOption--;
    if (padIsButtonPressedRepeat(kButtonDPadDown))
        page->selectedOption++;

    if (page->selectedOption < 0)
        page->selectedOption = page->numEntries - 1;
    else if (page->selectedOption >= page->numEntries)
        page->selectedOption = 0;

    int maxEntryStart = page->scrollOffset;
    int maxEntryEnd = maxEntryStart + page->maxEntries;

    if (page->selectedOption > maxEntryEnd)
        page->scrollOffset = page->selectedOption - page->maxEntries;
    else if (page->selectedOption < maxEntryStart)
        page->scrollOffset = page->selectedOption;
}

bool menuSelectable(DebugMenuPageState* page)
{
    bool isSelected = page->curEntry == page->selectedOption;

    printStr(isSelected ? "> " : "  ");

    bool didSelect = false;

    if (isSelected)
    {
        if (page->menuState->isAppearing == false && padIsButtonPressed(kButtonA))
        {
            didSelect = true;
        }
    }

    return didSelect;
}

bool beginMenuEntry(DebugMenuPageState* page)
{
    ++page->numEntries;
    page->curEntry = page->numEntries - 1;

    int maxEntryStart = page->scrollOffset;
    int maxEntryEnd = maxEntryStart + page->maxEntries;

    if (page->curEntry < maxEntryStart
        || page->curEntry > maxEntryEnd)
        return false;

    return true;
}

bool menuOption(DebugMenuPageState* page, const char* title)
{
    if (!beginMenuEntry(page))
        return false;

    bool selected = menuSelectable(page);
    printStr(title);
    logEndLine();

    return selected;
}

bool menuCheckbox(DebugMenuPageState* page, const char* title, int* isSelected)
{
    if (!beginMenuEntry(page))
        return false;

    bool dotoggle = menuSelectable(page);

    if (dotoggle)
        *isSelected = !*isSelected;

    printStr("[%c] %s", *isSelected ? 'X' : ' ', title);
    logEndLine();

    return dotoggle;
}

void menuTitle(DebugMenuPageState* page, const char* fmt, ...)
{
    printStr("<%i/%i>: ", page->menuState->curPage + 1, page->menuState->numPages);

    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, 0, fmt, vlist);
    va_end(vlist);

    logEndLine();
}

// pages
typedef enum _DebugMenuPage 
{
    kMenuPage_MainGameplay = 0,
    kMenuPage_MainObjects,
    kMenuPage_MainCamera,
    kMenuPage_MainWarp,

    kMenuPage_NumMainPages,

    kMenuPage_GameplayMovePlayer,
    kMenuPage_CameraFreeCam,

    kMenuPage_NumPages
} DebugMenuPage;

void doLockPlayerToggle(DebugMenuPageState* page)
{
    static int lockPlayer = 1;
    if (menuCheckbox(page, "lock player", &lockPlayer))
    {
        page->menuState->lockPlayer = lockPlayer == 1 ? true : false;
    }
}

void doMenuPage_Gameplay(DebugMenuPageState* page)
{
    beginMenu(page);

    ObjectInstance* player = getPlayer();
    menuTitle(page, "gameplay - player: %s", player ? player->objDef->name : "NO PLAYER!");

    if (menuOption(page, "move player"))
    {
        page->nextPage = kMenuPage_GameplayMovePlayer;
    }

    doLockPlayerToggle(page);

    endMenu(page);
}

void doMenuPage_MovePlayer(DebugMenuPageState* page)
{
    if (padIsButtonPressed(kButtonB))
        page->nextPage = page->menuState->prevPage;
        
    ObjectInstance* player = getPlayer();

    printLine("MOVE");

    if (!player)
    {
        printLine("NO PLAYER!");
        return;
    }

    beginMenu(page);


    vsnprintf(&printLogHandler, 0, "x=%.2f, y=%.2f, z=%.2f", &player->transform.pos);
    logEndLine();

    doLockPlayerToggle(page);
    endMenu(page);

    const float moveSpeed = 10.f;
    
    float stick_x = padGetStickX(0, -1) / 70.f;
    float stick_y = padGetStickY(0, -1) / 70.f;
    bool moveUp = padIsButtonDown(kButtonRTrig);
    bool moveDown = padIsButtonDown(kButtonLTrig);

    stick_x *= moveSpeed;
    stick_y *= moveSpeed;

    if (moveUp)
        player->transform.pos.y += moveSpeed;
    if (moveDown)
        player->transform.pos.y -= moveSpeed;

    Transform* camTransform = cameraGetTransform();

    if (!camTransform)
    {
        player->transform.pos.x -= stick_x;
        player->transform.pos.z += stick_y;
    }
    else
    {
        // only works because camera does a direct lookat at the player
        vec3f forward;
        vec3f_subtract(&player->transform.pos, &camTransform->pos, &forward);
        
        // only care about horizontal plane
        forward.y = 0;

        vec3f_normalize(&forward);

        static const vec3f up = { 0, 1, 0 };

        vec3f right;
        vec3f_cross(&forward, &up, &right);
        
        player->transform.pos.x += (forward.x * stick_y) + (right.x * stick_x);
        player->transform.pos.z += (forward.z * stick_y) + (right.z * stick_x);
    }
}

void doMenuPage_Objects(DebugMenuPageState* page)
{
    int unused, numObjs;
    ObjectInstance** objectList = getObjectsList(&unused, &numObjs);

    menuTitle(page, "objects (%i)", numObjs);

    beginMenu(page);

    for (int i = 0; i < numObjs; ++i)
    {
        static char optionName[32];
        snprintf2(optionName, sizeof(optionName), "%i: %s", i, objectList[i]->objDef->name);

        menuOption(page, optionName);
    }

    endMenu(page);
}


typedef struct _FreeCamState
{
    CameraBehavior* camBehavior;
    uint16 lastCamBehaviorType;
    uint32 lastCamUpdateFunc;
    int speedRaw;
    int showCamSpeedTimer;
} FreeCamState;

FreeCamState gFreeCam;

void updateFreeCam(CameraState* state)
{
    float stick_x = padGetStickX(0, -1) / 70.f;
    float stick_y = padGetStickY(0, -1) / 70.f;

    if (gFreeCam.showCamSpeedTimer > 0)
    {
        printLine("speed: %i", gFreeCam.speedRaw);
        --gFreeCam.showCamSpeedTimer;
    } 

    if (padIsButtonDown(kButtonZ))
    {
        state->transform.rot.x = 0;
        state->transform.rot.y = 0;
        state->transform.rot.z = 0;
        state->fov = 60;
        gFreeCam.speedRaw = 100;
        gFreeCam.showCamSpeedTimer = 60;
    }

    bool moveFast = padIsButtonDown(kButtonA);

    // use as switch -> if A is pressed, use dpad up/down as FOV control
    if (!moveFast)
    {
        if (padIsButtonPressedRepeat(kButtonDPadUp))
        {
            gFreeCam.showCamSpeedTimer = 60;
            ++gFreeCam.speedRaw;
        }
        if (padIsButtonPressedRepeat(kButtonDPadDown))
        {
            gFreeCam.showCamSpeedTimer = 60;
            --gFreeCam.speedRaw;
        }

        if (gFreeCam.speedRaw < 1)
            gFreeCam.speedRaw = 1;
    }

    float camSpeed = (moveFast ? gFreeCam.speedRaw * 3 : gFreeCam.speedRaw) / 10.f;

    bool moveUp = padIsButtonDown(kButtonRTrig);
    bool moveDown = padIsButtonDown(kButtonLTrig);

    bool crotLeft = padIsButtonDown(kButtonCLeft);
    bool crotRight = padIsButtonDown(kButtonCRight);
    bool crotUp = padIsButtonDown(kButtonCUp);
    bool crotDown = padIsButtonDown(kButtonCDown);

    bool dpadLeft = padIsButtonDown(kButtonDPadLeft);
    bool dpadRight = padIsButtonDown(kButtonDPadRight);

    int16 deltaRot = (int16)(0x300 * (camSpeed / 10.f));
    state->transform.rot.x += (crotLeft ? -deltaRot : 0) + (crotRight ? deltaRot : 0);
    state->transform.rot.y += (crotUp ? -deltaRot : 0) + (crotDown ? deltaRot : 0);
    state->transform.rot.z += (dpadLeft ? -deltaRot : 0) + (dpadRight ? deltaRot : 0);

    mtx44 camMtx;

    // for some reason cam transform has 0 scale
    Transform transformScaled = state->transform;
    transformScaled.scale = 1;
    mtx44_createFromTransform(&camMtx, &transformScaled);

    vec3f camDirX, camDirZ;
    mtx44_getAxisX(&camMtx, &camDirX);
    mtx44_getAxisZ(&camMtx, &camDirZ);

    vec3f_scale_add(&state->transform.pos, &camDirZ, stick_y * camSpeed, &state->transform.pos);
    vec3f_scale_add(&state->transform.pos, &camDirX, stick_x * camSpeed, &state->transform.pos);

    state->transform.pos.y += (moveUp ? camSpeed : 0) + (moveDown ? -camSpeed : 0);

    if (moveFast)
    {
        // use dpad for FOV
        bool dpadUp = padIsButtonDown(kButtonDPadUp);
        bool dpadDown = padIsButtonDown(kButtonDPadDown);
        
        float fovDelta = 0.1f * (gFreeCam.speedRaw / 10.f);
        state->fov += (dpadUp ? -fovDelta : 0) + (dpadDown ? fovDelta : 0);

        // adhere to game limits
        if (state->fov < 40.f)
            state->fov = 40.f;
        if (state->fov > 90.f)
            state->fov = 90.f;
    }
}

void patchFreeCamFunctions()
{
    uint32* exports = gFreeCam.camBehavior->dllFuncs->exports;
    exports[2] = &updateFreeCam;
}

void enterFreeCam()
{
    // now, we could do this properly, it would mean writing a custom camera interface DLL
    // and using setBehavior here, but who really cares : )

    gFreeCam.lastCamBehaviorType = -1;
    gFreeCam.camBehavior = cameraGetBehavior();
    gFreeCam.speedRaw = 100;
    gFreeCam.showCamSpeedTimer = 0;

    uint32* exports = gFreeCam.camBehavior->dllFuncs->exports;
    gFreeCam.lastCamUpdateFunc = exports[2];

    patchFreeCamFunctions();
}

void exitFreeCam()
{
    uint32* exports = gFreeCam.camBehavior->dllFuncs->exports;
    exports[2] = gFreeCam.lastCamUpdateFunc;
}

void doMenuPage_FreeCamera(DebugMenuPageState* page)
{
    patchFreeCamFunctions();

    if (padIsButtonPressed(kButtonB))
    {
        exitFreeCam();
        page->nextPage = page->menuState->prevPage;
    }
}

void doMenuPage_Camera(DebugMenuPageState* page)
{
    menuTitle(page, "camera");

    beginMenu(page);

    if (menuOption(page, "free cam"))
    {
        enterFreeCam();
        page->nextPage = kMenuPage_CameraFreeCam;
    }

    int widescreenFlag = gWidescreenFlag;
    if (menuCheckbox(page, "widescreen", &widescreenFlag))
        gWidescreenFlag = (uint8)widescreenFlag;

    endMenu(page);
}

void doMenuPage_Warp(DebugMenuPageState* page)
{
    beginMenu(page);

    menuTitle(page, "warp");

    typedef struct _WarpInfo
    {
        int index;
        char name[32];
    } WarpInfo;

    static const WarpInfo sWarpInfos[] =
    {
        { 2, "Ice Mountain" },
        { 5, "WM Back" },
        { 6, "WM Front" },
        { 12, "CloudRunner Prison" },
        { 14, "SwapStone Circle" },
        { 15, "SwapStone Hollow" },
        { 25, "Warlock Mountain" },
        { 26, "Ice Mountain" },
        { 29, "Boss Galadon" },
        { 30, "Boss Galadon Mouth" },
        { 31, "Desert" },
        // bunch of useless WM ones here
        { 36, "Test 1" },
        { 37, "Test 2" },
        { 38, "Test 3" },
        { 39, "Test 4" },
        { 40, "Test 5" },
        { 41, "Test 6" },
        { 42, "Test 7" },
        { 43, "Test 8" },
        // bunch of useless WM ones here
        { 54, "Boss Galadon" },
        { 55, "Boss Galadon Outro" },
        // more WM ones
        { 60, "Removed? CloudRunner Bottom" },
        { 61, "Inside Galleon" },
        { 63, "WM Chamber Top" },
        { 64, "Discovery Falls" },
        { 66, "Diamond Bay" },
        { 68, "Unavailable?" },
        { 73, "Moon Mountain Pass" },
        // more WM
        { 80, "SwapStone Circle intro" },
        { 81, "Volcano Force Point" },
        { 82, "SwapStone Hollow QE" },
        { 85, "SwapStone Circle" },
        { 86, "Shop" },
        { 87, "Magic Cave" },
        { 88, "Dragon Rock Minecarts" },
        { 90, "Boss T-Rex" },
        { 91, "Walled City" },
        { 92, "Boss Galadon intro" },
        { 93, "Unavailable?" },
        { 95, "Northern Wastes" },
    };

    for (int i = 0; i < sizeof(sWarpInfos) / sizeof(WarpInfo); ++i)
    {
        char optionName[64];
        snprintf2(optionName, sizeof(optionName), "%i: %s", sWarpInfos[i].index, sWarpInfos[i].name);

        if (menuOption(page, optionName))
        {
            warpPlayer(sWarpInfos[i].index, 1);
        }
    }

    endMenu(page);
}

static DebugMenuPageState gMenuPages[kMenuPage_NumPages];
static DebugMenuState gMenuState;

bool isAnyDPadButtonPressed()
{
    return false
        || padIsButtonDown(kButtonDPadRight)
        || padIsButtonDown(kButtonDPadLeft)
        || padIsButtonDown(kButtonDPadUp)
        || padIsButtonDown(kButtonDPadDown);
}

void enterDebugMenu()
{
    gMenuState.numPages = kMenuPage_NumMainPages;
    gMenuState.curPage = kMenuPage_MainGameplay;
    gMenuState.prevPage = -1;
    gMenuState.prevGameStateId = getGameStateId();
    gMenuState.isAppearing = true;
    gMenuState.lockPlayer = true;
    setGameStateId(kGameStateDebug);
}

void exitDebugMenu()
{
    gMenuState.curPage = -1;
    setGameStateId(gMenuState.prevGameStateId);
}

void initDebugMenu()
{
    gPadButtons[0] = 0;
    gPadButtons[1] = 0;
    
    gMenuState.prevGameStateId = -1;
    gMenuState.curPage = -1;
    gMenuState.lockPlayer = true;

    for (int i = 0; i < kMenuPage_NumPages; ++i)
    {
        DebugMenuPageState* page = &gMenuPages[i];
        page->menuState = &gMenuState;
        page->selectedOption = 0;
        page->curEntry = 0;
        page->maxEntries = 10;
        page->scrollOffset = 0;
    }
}

void updateMenuInput()
{
    // update controller state
    if (gNumConnectedControllers == 0)
    {
        gPadButtons[0] = 0;
        gPadButtons[1] = 0;
    }
    else
    {
        uint16 newButtons = (gControllerState.states[0].state >> 16) & 0xFFFF;
        gPadButtons[1] = gPadButtons[0];
        gPadButtons[0] = newButtons;
    }

    if (gPadButtons[0] != 0)
    {
        --gPadRepeat;
        if (gPadRepeat < 0)
            gPadRepeat = 1;
    }
    else
    {
        gPadRepeat = 10;
    }
}

void updateMenuScale()
{
    int screenRes = getScreenResolution();
    int screenResX = screenRes & 0xFFFF;
    int screenResY = screenRes >> 0x10;

    int scaleX = 0;
    int scaleY = 0;

    if (screenResX > 320)
        scaleX = 1;
    if (screenResY > 240)
        scaleY = 1;

    gDebugLogScaleX = scaleX;
    gDebugLogScaleY = scaleY;
}

void updateDebugMenu()
{
    updateMenuInput();

    // open the menu if any DPad button was pressed
    if (gMenuState.curPage == -1)
    {
        if (isAnyDPadButtonPressed())
        {
            enterDebugMenu();
        }
    }

    // if the menu isn't open, bail out
    if (gMenuState.curPage == -1)
        return;

    updateMenuScale();

    bool exitMenu = false;

    // advance menu pages
    if (gMenuState.curPage < kMenuPage_NumMainPages)
    {
        if (!gMenuState.isAppearing && padIsButtonPressed(kButtonDPadLeft))
            --gMenuState.curPage;
        if (!gMenuState.isAppearing && padIsButtonPressed(kButtonDPadRight))
            ++gMenuState.curPage;

        if (gMenuState.curPage < 0)
            gMenuState.curPage = kMenuPage_NumMainPages - 1;

        gMenuState.curPage = gMenuState.curPage % kMenuPage_NumMainPages;
    }
    
    // update the menu page
    DebugMenuPageState* page = &gMenuPages[gMenuState.curPage];
    page->nextPage = -1;

    switch (gMenuState.curPage)
    {
        case kMenuPage_MainGameplay: 		doMenuPage_Gameplay(page); break;
        case kMenuPage_MainObjects: 		doMenuPage_Objects(page); break;
        case kMenuPage_MainCamera: 		    doMenuPage_Camera(page); break;
        //case kMenuPage_MainWarp: 		    doMenuPage_Warp(page); break;

        case kMenuPage_GameplayMovePlayer: 	doMenuPage_MovePlayer(page); break;
    }

    switch (gMenuState.curPage)
    {
        case kMenuPage_MainWarp: 		    doMenuPage_Warp(page); break;

        case kMenuPage_CameraFreeCam: 		doMenuPage_FreeCamera(page); break;
    }

    gMenuState.isAppearing = false;

    // set the next menu page if indicated
    if (page->nextPage != -1)
    {
        gMenuState.prevPage = gMenuState.curPage;
        gMenuState.curPage = page->nextPage;
        gMenuState.isAppearing = true; // prevent closing menu next frame
    }

    if (gMenuState.lockPlayer)
        setGameStateId(kGameStateDebug);
    else
        setGameStateId(kGameStateInGame);

    if (!gMenuState.isAppearing && gMenuState.curPage < kMenuPage_NumMainPages && padIsButtonPressed(kButtonB))
        exitMenu = true;

    if (exitMenu)
        exitDebugMenu();
}

void frameHook()
{
    updateDebugMenu();
}