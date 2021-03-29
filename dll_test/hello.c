#include "dino_exports.h"

// this is simply a proof of concept, please don't take it too serious

void frameHook();

void DLL_EXPORT onLoad(void* aThis)
{
	// need to spend time doing a proper frame hook, instead of override some address
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

void frameHook()
{
	int unused, numObjs;
	void* objList = getObjList(&unused, &numObjs);

	// once the game has any objects loaded, we start flickering the environment color
	if (numObjs > 0)
	{
		switcher = (switcher + 1) % 2;
		setDisableEnvColor(switcher == 0);
	}
}