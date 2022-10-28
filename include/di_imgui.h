#ifndef _IMGUI_H_
#define _IMGUI_H_

#include "di_base.h"

typedef struct _DebugMenuPageState
{
    int curEntry;
    int numEntries;
    int maxEntries;
    int scrollOffset;
    int lastSelectedOption;
    int selectedOption;
    int nextPage;
} DebugMenuPageState;

typedef struct _DebugMenuState
{
    DebugMenuPageState pageState;
    int curPage;
    int prevPage;
    int numPages;
    int prevGameStateId;
    bool isAppearing;
    bool lockPlayer;
} DebugMenuState;

void printStr(const char* fmt, ...);
void printLine(const char* fmt, ...);
void printf2(const char* fmt, ...);
void logEndLine();
void* printLogHandler(void* arg, const char* buff, uint32 length);
void snprintf2(char* buff, uint32 buffSize, const char* fmt, ...);

void beginMenu();
void endMenu();
bool menuSelectable();
bool beginMenuEntry();
bool menuOption(const char* title);
bool menuCheckbox(const char* title, int* isSelected);
void menuTitle(const char* fmt, ...);
void menuText(const char* fmt, ...);
void menuSetLines(int num);

void menuPageSetNext(int pageId);
void menuPageSetPrev();
int menuPageGetCurrent();

void imguiInit();
void imguiInitMenuState(DebugMenuState* menuState);
bool imguiBegin(DebugMenuState* menuState, int numPages, int defaultPageId);
void imguiEnd();

void enterDebugMenu(DebugMenuState* menuState, int numPages, int startPageId);

#endif // _IMGUI_H_