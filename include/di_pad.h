#ifndef _PAD_H_
#define _PAD_H_

#include "di_base.h"

void padInit();
void padUpdate();

bool padIsButtonDown(PadButton button);
bool padIsButtonPressed(PadButton button);
bool padIsButtonPressedRepeat(PadButton button);
bool padIsButtonReleased(PadButton button);
bool padIsAnyDPadButtonPressed();

#endif // _PAD_H_