#include "di_camera.h"
#include "di_dlls.h"

CameraState* cameraGetState()
{
    DLL_Camera* camDLL = (DLL_Camera*)dllGetFuncs(kDLL_Camera);

    if (!camDLL)
        return NULL;

    CameraState* camState = camDLL->getState();

    return camState;
}

Transform* cameraGetTransform()
{
    CameraState* camState = cameraGetState();

    if (!camState)
        return NULL;

    return &camState->transform;
}