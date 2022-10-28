#ifndef _DI_WARP_H_
#define _DI_WARP_H_

typedef struct _WarpInfo
{
    int index;
    char name[32];
} WarpInfo;

int warpGetNumEntries();
const WarpInfo* warpGetInfo(int idx);

#endif // _DI_WARP_H_