#ifndef _DINO_IMPORTS_H_
#define _DINO_IMPORTS_H_

#include "di_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void*(*_vsnprintf_func)(void* arg, const char* buff, uint32 length);

extern void*  	                        DLL_IMPORT  diMalloc(uint32 size, int category, const char* name);
extern void  	                        DLL_IMPORT  diFree(void* p);
extern void  	                        DLL_IMPORT  diMemZero(void* p, uint32 size);
extern void  	                        DLL_IMPORT  diMemcpyFast(void* src, void* dst, uint32 size);
extern void  	                        DLL_IMPORT  setDisableEnvColor(bool disable);
extern bool  	                        DLL_IMPORT  getDisableEnvColor();
extern void                             DLL_IMPORT  camSetFOV(float fovDeg);
extern ObjectInstance**                 DLL_IMPORT  getObjectsList(int* outUnused, int* outNumObjs);
extern ObjectInstance*                  DLL_IMPORT  getPlayer();
extern void                             DLL_IMPORT  spawnPlayer(uint typ);
extern int                              DLL_IMPORT  getScreenResolution();     
extern int                              DLL_IMPORT  vsnprintf(_vsnprintf_func, void* arg, const char* fmt, void* args);
extern void                             DLL_IMPORT  clearScreen(void** gfx, void** mtx, int mode);
extern void*                            DLL_IMPORT  dllLoadThreaded(int dllId);
extern void                             DLL_IMPORT  logSetColor(uint8 r, uint8 g, uint8 b, uint8 a);
extern void                             DLL_IMPORT  logSetPos(int32 x, int32 y);

extern int                              DLL_IMPORT  padGetStickXFromBuffer(int padId, int frame);
extern int                              DLL_IMPORT  padGetStickYFromBuffer(int padId, int frame);
extern uint16                           DLL_IMPORT  padGetButtonsFromBuffer(int padId, int frame);

extern void                             DLL_IMPORT  warpPlayer(int warpId, char unk0);
extern void                             DLL_IMPORT  initGameForMenu(int mapId, int unk0, int playerType, int unk1);
extern void                             DLL_IMPORT  setMenuPage(int menuPageId);

extern DLLInfo*                         DLL_IMPORT  dllGetLoadedList(int* outNumDLLs);

extern char*                            DLL_IMPORT  gDebugLog;
extern uint8                            DLL_IMPORT  gDebugLogScaleX;
extern uint8                            DLL_IMPORT  gDebugLogScaleY;
extern int                              DLL_IMPORT  gGameState;
extern RawControllerStateArray          DLL_IMPORT  gControllerState;  
extern uint8                            DLL_IMPORT  gNumConnectedControllers;
extern uint8                            DLL_IMPORT  gWidescreenFlag;
extern void*                            DLL_IMPORT  gRenderCmdList;
extern void*                            DLL_IMPORT  gRenderCmdMtx;
extern uint32                           DLL_IMPORT  gNumObjectTypes;
extern Object**                         DLL_IMPORT  gObjectTypesList;

extern DLLInfo*                         DLL_IMPORT  gDLL_camcontrol;
extern DLLInfo*                         DLL_IMPORT  gDLL_AMSFX;
extern DLLInfo*                         DLL_IMPORT  gDLL_text;
extern DLLInfo*                         DLL_IMPORT  gDLL_gameplay;
extern DLLInfo*                         DLL_IMPORT  gDLL_clrscr;
extern DLLInfo*                         DLL_IMPORT  gDLL_subtitles;
extern DLLInfo*                         DLL_IMPORT  gDLL_projgfx;
extern DLLInfo*                         DLL_IMPORT  gDLL_AMSEQ;
extern DLLInfo*                         DLL_IMPORT  gDLL_partfx;
extern DLLInfo*                         DLL_IMPORT  gDLL_CURVES;
extern DLLInfo*                         DLL_IMPORT  gDLL_newfog;
extern DLLInfo*                         DLL_IMPORT  gDLL_minic;
extern DLLInfo*                         DLL_IMPORT  gDLL_expgfx;
extern DLLInfo*                         DLL_IMPORT  gDLL_paths;
extern DLLInfo*                         DLL_IMPORT  gDLL_Save;
extern DLLInfo*                         DLL_IMPORT  gDLL_waterfx;
extern DLLInfo*                         DLL_IMPORT  gDLL_tasktext;
extern DLLInfo*                         DLL_IMPORT  gDLL_frontend;
extern DLLInfo*                         DLL_IMPORT  gDLL_SCREENS;
extern DLLInfo*                         DLL_IMPORT  gDLL_modgfx;
extern DLLInfo*                         DLL_IMPORT  gDLL_newclouds;
extern DLLInfo*                         DLL_IMPORT  gDLL_ANIM;
extern DLLInfo*                         DLL_IMPORT  gDLL_pickup;
extern DLLInfo*                         DLL_IMPORT  gDLL_Race;

#ifdef __cplusplus
}
#endif

#endif // _DINO_IMPORTS_H_