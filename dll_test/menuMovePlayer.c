#include "di_imports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"
#include "di_pad.h"
#include "di_imgui.h"
#include "menuPageTypes.h"

extern void doLockPlayerToggle();

void doMenuPage_MovePlayer()
{
    if (padIsButtonPressed(kButtonB))
        menuPageSetPrev();

    ObjectInstance* player = getPlayer();

    printLine("MOVE");

    if (!player)
    {
        printLine("NO PLAYER!");
        return;
    }

    beginMenu();
    {
        vsnprintf(&printLogHandler, 0, "x=%.2f, y=%.2f, z=%.2f", &player->transform.pos);
        logEndLine();

        doLockPlayerToggle();
    }
    endMenu();

    const float moveSpeed = 10.f;

    float stick_x = padGetStickXFromBuffer(0, -1) / 70.f;
    float stick_y = padGetStickYFromBuffer(0, -1) / 70.f;
    bool moveUp = padIsButtonDown(kButtonRTrig);
    bool moveDown = padIsButtonDown(kButtonLTrig);

    stick_x *= moveSpeed;
    stick_y *= moveSpeed;

    if (moveUp)
        player->transform.pos.y += moveSpeed;
    if (moveDown)
        player->transform.pos.y -= moveSpeed;

    Transform* camTransform = cameraGetTransform();

    if (!camTransform)
    {
        player->transform.pos.x -= stick_x;
        player->transform.pos.z += stick_y;
    }
    else
    {
        // only works because camera does a direct lookat at the player
        vec3f forward;
        vec3f_subtract(&player->transform.pos, &camTransform->pos, &forward);

        // only care about horizontal plane
        forward.y = 0;

        vec3f_normalize(&forward);

        static const vec3f up = { 0, 1, 0 };

        vec3f right;
        vec3f_cross(&forward, &up, &right);

        player->transform.pos.x += (forward.x * stick_y) + (right.x * stick_x);
        player->transform.pos.z += (forward.z * stick_y) + (right.z * stick_x);
    }
}