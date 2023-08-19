#ifndef _PATCH_UTILS_H_
#define _PATCH_UTILS_H_

#include "di_base.h"

void patchUtilInjectFunctionCallWrap(void* sourceFunc, void* targetFunc);

#endif // _PATCH_UTILS_H_