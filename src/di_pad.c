#include "di_pad.h"
#include "di_exports.h"

static uint16 gPadButtons[2];
static int gPadRepeat;

void padInit()
{
    gPadButtons[0] = 0;
    gPadButtons[1] = 0;
    gPadRepeat = 0;
}

void padUpdate()
{
    if (gNumConnectedControllers == 0)
    {
        gPadButtons[0] = 0;
        gPadButtons[1] = 0;
    }
    else
    {
        uint16 newButtons = (gControllerState.states[0].state >> 16) & 0xFFFF;
        gPadButtons[1] = gPadButtons[0];
        gPadButtons[0] = newButtons;
    }

    if (gPadButtons[0] != 0)
    {
        --gPadRepeat;
        if (gPadRepeat < 0)
            gPadRepeat = 1;
    }
    else
    {
        gPadRepeat = 10;
    }
}

bool padIsButtonDown(PadButton button)
{
    return gPadButtons[0] & button;
}

bool padIsButtonPressed(PadButton button)
{
    return (~gPadButtons[1] & gPadButtons[0]) & button;
}

bool padIsButtonPressedRepeat(PadButton button)
{
    return false
           || padIsButtonPressed(button)
           || (padIsButtonDown(button) && gPadRepeat == 0);
}

bool padIsButtonReleased(PadButton button)
{
    return (~gPadButtons[0] & gPadButtons[1]) & button;
}

bool padIsAnyDPadButtonPressed()
{
    return false
           || padIsButtonDown(kButtonDPadRight)
           || padIsButtonDown(kButtonDPadLeft)
           || padIsButtonDown(kButtonDPadUp)
           || padIsButtonDown(kButtonDPadDown);
}