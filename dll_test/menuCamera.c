#include "di_imports.h"
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

    endMenu();
}