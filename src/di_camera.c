#include "di_camera.h"
#include "di_dlls.h"

CameraState* cameraGetState()
{
    DLL_Camera* camDLL = (DLL_Camera*)gDLL_camcontrol->entryPoint.exports;

    if (!camDLL)
        return NULL;

    CameraState* camState = camDLL->getState();

    return camState;
}

CameraBehavior* cameraGetBehavior()
{
    DLL_Camera* camDLL = (DLL_Camera*)gDLL_camcontrol->entryPoint.exports;

    if (!camDLL)
        return NULL;

    CameraBehavior* camBehavior = camDLL->getBehavior();

    return camBehavior;
}

Transform* cameraGetTransform()
{
    CameraState* camState = cameraGetState();

    if (!camState)
        return NULL;

    return &camState->transform;
}