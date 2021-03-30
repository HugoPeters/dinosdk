#ifndef _DINO_EXPORTS_H_
#define _DINO_EXPORTS_H_

#include "dino_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32(*_vsnprintf_func)(uint32 maxSize, const char* buff, uint32 length);

extern void  	                        DLL_IMPORT 	setDisableEnvColor(bool disable);
extern bool  	                        DLL_IMPORT 	getDisableEnvColor();
extern void                             DLL_IMPORT  camSetFOV(float fovDeg);
extern ObjectInstance**                 DLL_IMPORT  getObjectsList(int* outUnused, int* outNumObjs);
extern ObjectInstance*                  DLL_IMPORT  getPlayer();
extern int                              DLL_IMPORT  getScreenResolution();     
extern int                              DLL_IMPORT  vsnprintf(_vsnprintf_func, uint32 maxSize, const char* fmt, void* args);

extern int                              DLL_IMPORT  padGetStickX(int padId, int frame);
extern int                              DLL_IMPORT  padGetStickY(int padId, int frame);
extern uint16                           DLL_IMPORT  padGetButtons(int padId, int frame);

extern char*                            DLL_IMPORT  gDebugLog;
extern uint8                            DLL_IMPORT  gDebugLogScaleX;
extern uint8                            DLL_IMPORT  gDebugLogScaleY;
extern int                              DLL_IMPORT  gGameState;
extern RawControllerStateArray          DLL_IMPORT  gControllerState;  
extern uint8                            DLL_IMPORT  gNumConnectedControllers;

#ifdef __cplusplus
}
#endif

#endif // _DINO_EXPORTS_H_