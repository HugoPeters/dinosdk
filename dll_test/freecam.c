#include "freecam.h"
#include "di_imports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"
#include "di_pad.h"
#include "di_imgui.h"

typedef struct _FreeCamState
{
    CameraBehavior* camBehavior;
    uint16 lastCamBehaviorType;
    uint32 lastCamUpdateFunc;
    int speedRaw;
    int showCamSpeedTimer;
} FreeCamState;

FreeCamState gFreeCam;

void freecamUpdate(CameraState* state)
{
    float stick_x = padGetStickXFromBuffer(0, -1) / 70.f;
    float stick_y = padGetStickYFromBuffer(0, -1) / 70.f;

    if (gFreeCam.showCamSpeedTimer > 0)
    {
        printLine("speed: %i", gFreeCam.speedRaw);
        --gFreeCam.showCamSpeedTimer;
    }

    if (padIsButtonDown(kButtonZ))
    {
        state->transform.rot.x = 0;
        state->transform.rot.y = 0;
        state->transform.rot.z = 0;
        state->fov = 60;
        gFreeCam.speedRaw = 100;
        gFreeCam.showCamSpeedTimer = 60;
    }

    bool moveFast = padIsButtonDown(kButtonA);

    // use as switch -> if A is pressed, use dpad up/down as FOV control
    if (!moveFast)
    {
        if (padIsButtonPressedRepeat(kButtonDPadUp))
        {
            gFreeCam.showCamSpeedTimer = 60;
            ++gFreeCam.speedRaw;
        }
        if (padIsButtonPressedRepeat(kButtonDPadDown))
        {
            gFreeCam.showCamSpeedTimer = 60;
            --gFreeCam.speedRaw;
        }

        if (gFreeCam.speedRaw < 1)
            gFreeCam.speedRaw = 1;
    }

    float camSpeed = (moveFast ? gFreeCam.speedRaw * 3 : gFreeCam.speedRaw) / 10.f;

    bool moveUp = padIsButtonDown(kButtonRTrig);
    bool moveDown = padIsButtonDown(kButtonLTrig);

    bool crotLeft = padIsButtonDown(kButtonCLeft);
    bool crotRight = padIsButtonDown(kButtonCRight);
    bool crotUp = padIsButtonDown(kButtonCUp);
    bool crotDown = padIsButtonDown(kButtonCDown);

    bool dpadLeft = padIsButtonDown(kButtonDPadLeft);
    bool dpadRight = padIsButtonDown(kButtonDPadRight);

    int16 deltaRot = (int16)(0x300 * (camSpeed / 10.f));
    state->transform.rot.x += (crotLeft ? -deltaRot : 0) + (crotRight ? deltaRot : 0);
    state->transform.rot.y += (crotUp ? -deltaRot : 0) + (crotDown ? deltaRot : 0);
    state->transform.rot.z += (dpadLeft ? -deltaRot : 0) + (dpadRight ? deltaRot : 0);

    mtx44 camMtx;

    // for some reason cam transform has 0 scale
    Transform transformScaled = state->transform;
    transformScaled.scale = 1;
    mtx44_createFromTransform(&camMtx, &transformScaled);

    vec3f camDirX, camDirZ;
    mtx44_getAxisX(&camMtx, &camDirX);
    mtx44_getAxisZ(&camMtx, &camDirZ);

    vec3f_scale_add(&state->transform.pos, &camDirZ, stick_y * camSpeed, &state->transform.pos);
    vec3f_scale_add(&state->transform.pos, &camDirX, stick_x * camSpeed, &state->transform.pos);

    state->transform.pos.y += (moveUp ? camSpeed : 0) + (moveDown ? -camSpeed : 0);

    if (moveFast)
    {
        // use dpad for FOV
        bool dpadUp = padIsButtonDown(kButtonDPadUp);
        bool dpadDown = padIsButtonDown(kButtonDPadDown);

        float fovDelta = 0.1f * (gFreeCam.speedRaw / 10.f);
        state->fov += (dpadUp ? -fovDelta : 0) + (dpadDown ? fovDelta : 0);

        // adhere to game limits
        if (state->fov < 40.f)
            state->fov = 40.f;
        if (state->fov > 90.f)
            state->fov = 90.f;
    }
}

void freecamPatchFuncs()
{
    uint32* exports = gFreeCam.camBehavior->dllFuncs->exports;
    exports[2] = (uint32)&freecamUpdate;
}

void freecamEnter()
{
    // now, we could do this properly, it would mean writing a custom camera interface DLL
    // and using setBehavior here, but who really cares : )

    gFreeCam.lastCamBehaviorType = -1;
    gFreeCam.camBehavior = cameraGetBehavior();
    gFreeCam.speedRaw = 100;
    gFreeCam.showCamSpeedTimer = 0;

    uint32* exports = gFreeCam.camBehavior->dllFuncs->exports;
    gFreeCam.lastCamUpdateFunc = exports[2];

    freecamPatchFuncs();
}

void freecamExit()
{
    uint32* exports = gFreeCam.camBehavior->dllFuncs->exports;
    exports[2] = gFreeCam.lastCamUpdateFunc;
}