#include "dino_exports.h"

// this is simply a proof of concept, please don't take it too serious

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

// these are required as this DLL expects at least3 public functions
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

inline bool padIsButtonDown(PadButton button)
{
	return padGetButtons(0, -1) & button;
}

inline bool padIsButtonPressed(PadButton button)
{
	return (~padGetButtons(0, 0) & padGetButtons(0, 1)) & button;
}

inline bool padIsButtonReleased(PadButton button)
{
	return (~padGetButtons(0, 1) & padGetButtons(0, 0)) & button;
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

void printStr(const char* str)
{
	while (*str)
	{
		logPutChar(*str);
		++str;
	}
}

void printLine(const char* str)
{
	printStr(str);
	logEndLine();
}

// menu
typedef struct _DebugMenuState
{
	int pageId;
	int prevGameStateId;
	bool isAppearing;
} DebugMenuState;

typedef struct _DebugMenuPageState
{
	DebugMenuState* menuState;
	int selectedOption;
	int curDrawOption;
	int numOptions;
	int nextPage;
} DebugMenuPageState;

// utils
void beginMenu(DebugMenuPageState* page)
{
	page->curDrawOption = 0;
	page->nextPage = -1;
}

void endMenu(DebugMenuPageState* page)
{
	page->numOptions = page->curDrawOption;

	if (padIsButtonPressed(kButtonDPadUp))
		page->selectedOption--;
	if (padIsButtonPressed(kButtonDPadDown))
		page->selectedOption++;

	if (page->selectedOption < 0)
		page->selectedOption = page->numOptions - 1;
	else if (page->selectedOption >= page->numOptions)
		page->selectedOption = 0;
}

bool menuOption(DebugMenuPageState* page, const char* title)
{
	int thisOption = page->curDrawOption++;
	bool isSelected = thisOption == page->selectedOption;

	printStr(isSelected ? ">" : " ");
	printStr(title);
	logEndLine();

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

// pages
typedef enum _DebugMenuPage 
{
	kMenuPage_Main = 0,

	kMenuPage_NumMainPages,

	kMenuPage_MovePlayer,

	kMenuPage_NumPages
} DebugMenuPage;

void doMenuPage_Gameplay(DebugMenuPageState* page)
{
	beginMenu(page);

	if (menuOption(page, "move player"))
	{
		page->nextPage = kMenuPage_MovePlayer;
	}

	endMenu(page);
}

void doMenuPage_MovePlayer(DebugMenuPageState* page)
{
	if (padIsButtonDown(kButtonB))
		page->nextPage = kMenuPage_Main;

	printLine("MOVE");

	ObjectInstance* obj = getPlayer();

	float stick_x = padGetStickX(0, -1) / 70.f;
	float stick_y = padGetStickY(0, -1) / 70.f;
	bool moveUp = padIsButtonDown(kButtonRTrig);
	bool moveDown = padIsButtonDown(kButtonLTrig);

	if (moveUp)
		obj->pos.y += 10;
	if (moveDown)
		obj->pos.y -= 10;

	obj->pos.x += stick_x * 10.f;
	obj->pos.z += stick_y * 10.f;
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
	gMenuState.pageId = kMenuPage_Main;
	gMenuState.prevGameStateId = getGameStateId();
	gMenuState.isAppearing = true;
	setGameStateId(kGameStateDebug);
}

void exitDebugMenu()
{
	gMenuState.pageId = -1;
	setGameStateId(gMenuState.prevGameStateId);
}

void initDebugMenu()
{
	gMenuState.prevGameStateId = -1;
	gMenuState.pageId = -1;

	for (int i = 0; i < kMenuPage_NumPages; ++i)
	{
		DebugMenuPageState* page = &gMenuPages[i];
		page->menuState = &gMenuState;
		page->selectedOption = 0;
	}
}

void updateDebugMenu()
{
	if (gMenuState.pageId == -1)
	{
		if (isAnyDPadButtonPressed())
		{
			enterDebugMenu();
		}
	}

	if (gMenuState.pageId == -1)
		return;

	bool exitMenu = false;

	if (gMenuState.pageId < kMenuPage_NumMainPages)
	{
		if (!gMenuState.isAppearing && padIsButtonPressed(kButtonDPadLeft))
			--gMenuState.pageId;
		if (!gMenuState.isAppearing && padIsButtonPressed(kButtonDPadRight))
			++gMenuState.pageId;

		if (gMenuState.pageId < 0)
			gMenuState.pageId = kMenuPage_NumMainPages - 1;

		gMenuState.pageId = gMenuState.pageId % kMenuPage_NumMainPages;
	}
	
	DebugMenuPageState* page = &gMenuPages[gMenuState.pageId];
	page->nextPage = -1;

	switch (gMenuState.pageId)
	{
		case kMenuPage_Main: 		doMenuPage_Gameplay(page); break;
		case kMenuPage_MovePlayer: 	doMenuPage_MovePlayer(page); break;
	}

	gMenuState.isAppearing = false;

	if (page->nextPage != -1)
	{
		gMenuState.pageId = page->nextPage;
		gMenuState.isAppearing = true; // prevent closing menu next frame
	}

	if (!gMenuState.isAppearing && gMenuState.pageId == kMenuPage_Main && padIsButtonPressed(kButtonB))
		exitMenu = true;

	if (exitMenu)
		exitDebugMenu();
}

void frameHook()
{
	updateDebugMenu();
}