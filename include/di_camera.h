#ifndef _DINO_CAMERA_H_
#define _DINO_CAMERA_H_

#include "di_base.h"

typedef struct _CameraState
{
    Transform transform;
    char pad[0xA0 - 24];
    void* characterState;
} CameraState;

typedef struct _DLL_Camera
{
    uint32          reserved;
    void            (*func1)(float a, float b, float c, float d);
    void            (*update)();
    CameraState*    (*getState)();
} DLL_Camera;

CameraState* cameraGetState();
Transform* cameraGetTransform();

#endif // _DINO_CAMERA_H_