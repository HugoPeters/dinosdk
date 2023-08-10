#ifndef _freecam_H_
#define _freecam_H_

#include "di_camera.h"

void freecamUpdate(CameraState* state);
void freecamPatchFuncs();
void freecamEnter();
void freecamExit();

#endif // _freecam_H_