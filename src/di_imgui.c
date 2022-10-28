#include "di_imgui.h"
#include "di_exports.h"
#include "di_pad.h"

static DebugMenuState* gCurrentMenuState;

inline void setGameStateId(GameState newId)
{
    gGameState = newId;
}

inline int getGameStateId()
{
    return gGameState;
}

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

void beginMenu()
{
    DebugMenuPageState* page = &gCurrentMenuState->pageState;

    page->curEntry = 0;
    page->numEntries = 0;

    logSetColor(0, 0, 0, 0x80);
}

void endMenu()
{
    DebugMenuPageState* page = &gCurrentMenuState->pageState;
    page->lastSelectedOption = page->selectedOption;

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

    printLine("%i/%i", page->selectedOption + 1, page->numEntries);
}

bool menuSelectable()
{
    DebugMenuPageState* page = &gCurrentMenuState->pageState;

    bool isSelected = page->curEntry == page->selectedOption;

    printStr(isSelected ? "> " : "  ");

    bool didSelect = false;

    if (isSelected)
    {
        if (gCurrentMenuState->isAppearing == false && padIsButtonPressed(kButtonA))
        {
            didSelect = true;
        }
    }

    return didSelect;
}

bool beginMenuEntry()
{
    DebugMenuPageState* page = &gCurrentMenuState->pageState;

    ++page->numEntries;
    page->curEntry = page->numEntries - 1;

    int maxEntryStart = page->scrollOffset;
    int maxEntryEnd = maxEntryStart + page->maxEntries;

    if (page->curEntry < maxEntryStart
        || page->curEntry > maxEntryEnd)
        return false;

    if (page->curEntry == 0)
    {
        gCurrentMenuState->lockPlayer = true;
    }

    return true;
}

bool menuOption(const char* title)
{
    if (!beginMenuEntry())
        return false;

    bool selected = menuSelectable();
    printStr("[%s]", title);
    logEndLine();

    return selected;
}

bool menuCheckbox(const char* title, int* isSelected)
{
    if (!beginMenuEntry())
        return false;

    bool dotoggle = menuSelectable();

    if (dotoggle)
        *isSelected = !*isSelected;

    printStr("[%c] %s", *isSelected ? 'X' : ' ', title);
    logEndLine();

    return dotoggle;
}

void menuTitle(const char* fmt, ...)
{
    printStr("<%i/%i>: ", gCurrentMenuState->curPage + 1, gCurrentMenuState->numPages);

    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, 0, fmt, vlist);
    va_end(vlist);

    logEndLine();
}

void menuText(const char* fmt, ...)
{
    if (!beginMenuEntry())
        return;

    DebugMenuPageState* page = &gCurrentMenuState->pageState;

    if (page->curEntry == page->selectedOption)
    {
        if (page->selectedOption > page->lastSelectedOption)
            gCurrentMenuState->pageState.selectedOption++;  
        else // going backwards
            gCurrentMenuState->pageState.selectedOption--;  
    }

    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(&printLogHandler, 0, fmt, vlist);
    va_end(vlist);

    logEndLine();
}

void menuSetLines(int num)
{
    if (!gCurrentMenuState)
        return;

    gCurrentMenuState->pageState.maxEntries = num;
}

void enterDebugMenu(DebugMenuState* menuState, int numPages, int startPageId)
{
    menuState->numPages = numPages;
    menuState->curPage = startPageId;
    menuState->prevPage = -1;
    menuState->prevGameStateId = getGameStateId();
    menuState->isAppearing = true;
    menuState->lockPlayer = true;
    setGameStateId(kGameStateDebug);
}

void menuPageSetNext(int pageId)
{
    gCurrentMenuState->pageState.nextPage = pageId;
}

void menuPageSetPrev()
{
    gCurrentMenuState->pageState.nextPage = gCurrentMenuState->prevPage;
}

int menuPageGetCurrent()
{
    return gCurrentMenuState->curPage;
}

void imguiInit()
{
    gCurrentMenuState = NULL;
}

void imguiInitMenuState(DebugMenuState* menuState)
{
    menuState->prevGameStateId = -1;
    menuState->curPage = -1;
    menuState->lockPlayer = true;

    DebugMenuPageState* page = &menuState->pageState;
    page->selectedOption = 0;
    page->curEntry = 0;
    page->maxEntries = 10;
    page->scrollOffset = 0;
}

bool imguiBegin(DebugMenuState* menuState, int numPages, int defaultPageId)
{
    if (gCurrentMenuState || !menuState)
        return false;

    // open the menu if any DPad button was pressed
    if (menuState->curPage == -1)
        if (padIsAnyDPadButtonPressed())
            enterDebugMenu(menuState, numPages, defaultPageId);

    // if the menu isn't open, bail out
    if (menuState->curPage == -1)
        return false;

    updateMenuScale();

    // advance menu pages
    if (menuState->curPage < menuState->numPages)
    {
        if (!menuState->isAppearing && padIsButtonPressed(kButtonDPadLeft))
            --menuState->curPage;
        if (!menuState->isAppearing && padIsButtonPressed(kButtonDPadRight))
            ++menuState->curPage;

        if (menuState->curPage < 0)
            menuState->curPage = menuState->numPages - 1;

        menuState->curPage = menuState->curPage % menuState->numPages;
    }

    menuState->pageState.nextPage = -1;
    gCurrentMenuState = menuState;

    return true;
}

void exitDebugMenu()
{
    gCurrentMenuState->curPage = -1;
    setGameStateId(gCurrentMenuState->prevGameStateId);
}

void imguiEnd()
{
    DebugMenuState* menuState = gCurrentMenuState;

    if (!menuState)
        return;

    menuState->isAppearing = false;

    DebugMenuPageState* page = &menuState->pageState;

    // set the next menu page if indicated
    if (page->nextPage != -1)
    {
        menuState->prevPage = menuState->curPage;
        menuState->curPage = page->nextPage;
        menuState->isAppearing = true; // prevent closing menu next frame
    }

    if (menuState->lockPlayer)
        setGameStateId(kGameStateDebug);
    else
        setGameStateId(kGameStateInGame);

    if (!menuState->isAppearing && menuState->curPage < menuState->numPages && padIsButtonPressed(kButtonB))
        exitDebugMenu();

    gCurrentMenuState = NULL;
}
