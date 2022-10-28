#include "di_exports.h"
#include "di_dlls.h"
#include "di_camera.h"
#include "di_math.h"
#include "di_pad.h"
#include "di_imgui.h"
#include "menuPageTypes.h"

extern void freecamEnter();

void doMenuPage_Camera()
{
    menuTitle("camera");

    beginMenu();

    if (menuOption("free cam"))
    {
        freecamEnter();
        menuPageSetNext(kMenuPage_CameraFreeCam);
    }

    int widescreenFlag = gWidescreenFlag;
    if (menuCheckbox("widescreen", &widescreenFlag))
        gWidescreenFlag = (uint8)widescreenFlag;

    void (*setScreenWidthHeight)(int unk0, int width, int height) = 0x8005d8c0;

    if (menuOption("high res"))
    {
        setScreenWidthHeight(0, 640, 480);
    }

    if (menuOption("normal res"))
    {
        setScreenWidthHeight(0, 320, 240);
    }

    endMenu();
}