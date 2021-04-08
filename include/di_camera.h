#ifndef _DINO_CAMERA_H_
#define _DINO_CAMERA_H_

#include "di_base.h"
#include "di_dlls.h"

typedef struct _CameraState
{
    Transform transform;
    char pad[0xB0 - 24];
    float fov;
} CameraState;

typedef struct _CameraBehavior
{
    uint16 dllType;
    uint16 unk0;
    DLLEntryPoint* dllFuncs;
 } CameraBehavior;

typedef struct _DLL_Camera
{
    uint32          reserved;
    void            (*func1)(float a, float b, float c, float d);
    void            (*update)();
    CameraState*    (*getState)();
    int             (*getBehaviorDLLType)();
    void*           (*getBehavior)();
    void*           (*getDefaultBehavior)();
} DLL_Camera;

CameraState* cameraGetState();
CameraBehavior* cameraGetBehavior();
Transform* cameraGetTransform();

#endif // _DINO_CAMERA_H_