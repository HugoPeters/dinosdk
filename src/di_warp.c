#include "di_warp.h"

static const WarpInfo gWarpInfos[] =
{
    { 2, "Ice Mountain" },
    { 5, "WM Back" },
    { 6, "WM Front" },
    { 12, "CloudRunner Prison" },
    { 14, "SwapStone Circle" },
    { 15, "SwapStone Hollow" },
    { 25, "Warlock Mountain" },
    { 26, "Ice Mountain" },
    { 29, "Boss Galadon" },
    { 30, "Boss Galadon Mouth" },
    { 31, "Desert" },
    // bunch of useless WM ones here
    { 36, "Test 1" },
    { 37, "Test 2" },
    { 38, "Test 3" },
    { 39, "Test 4" },
    { 40, "Test 5" },
    { 41, "Test 6" },
    { 42, "Test 7" },
    { 43, "Test 8" },
    // bunch of useless WM ones here
    { 54, "Boss Galadon" },
    { 55, "Boss Galadon Outro" },
    // more WM ones
    { 60, "Removed? CloudRunner Bottom" },
    { 61, "Inside Galleon" },
    { 63, "WM Chamber Top" },
    { 64, "Discovery Falls" },
    { 66, "Diamond Bay" },
    { 68, "Unavailable?" },
    { 73, "Moon Mountain Pass" },
    // more WM
    { 80, "SwapStone Circle intro" },
    { 81, "Volcano Force Point" },
    { 82, "SwapStone Hollow QE" },
    { 85, "SwapStone Circle" },
    { 86, "Shop" },
    { 87, "Magic Cave" },
    { 88, "Dragon Rock Minecarts" },
    { 90, "Boss T-Rex" },
    { 91, "Walled City" },
    { 92, "Boss Galadon intro" },
    { 93, "Unavailable?" },
    { 95, "Northern Wastes" },
};

int warpGetNumEntries()
{
    return sizeof(gWarpInfos) / sizeof(WarpInfo);
}

const WarpInfo* warpGetInfo(int idx)
{
    return &gWarpInfos[idx];
}