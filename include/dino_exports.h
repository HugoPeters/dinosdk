#ifndef _DINO_EXPORTS_H_
#define _DINO_EXPORTS_H_

#include "dino_base.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void  	        DLL_IMPORT 	setDisableEnvColor(bool disable);
extern bool  	        DLL_IMPORT 	getDisableEnvColor();
extern void             DLL_IMPORT  camSetFOV(float fovDeg);
extern void*            DLL_IMPORT  getObjList(int* outUnused, int* outNumObjs);
extern ObjectInstance*  DLL_IMPORT  getPlayer();

extern int              DLL_IMPORT  padGetStickX(int padId, int frame);
extern int              DLL_IMPORT  padGetStickY(int padId, int frame);
extern uint16           DLL_IMPORT  padGetButtons(int padId, int frame);

extern char*            DLL_IMPORT  gDebugLog;
extern int              DLL_IMPORT  gGameState;

#ifdef __cplusplus
}
#endif

#endif // _DINO_EXPORTS_H_