#ifndef _DINO_EXPORTS_H_
#define _DINO_EXPORTS_H_

#include "dino_base.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void  	DLL_IMPORT 	setDisableEnvColor(bool disable);
extern bool  	DLL_IMPORT 	getDisableEnvColor();
extern void     DLL_IMPORT  camSetFOV(float fovDeg);
extern void*    DLL_IMPORT  getObjList(int* outUnused, int* outNumObjs);

#ifdef __cplusplus
}
#endif

#endif // _DINO_EXPORTS_H_