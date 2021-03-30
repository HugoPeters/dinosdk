#ifndef _DINO_BASE_H_
#define _DINO_BASE_H_

#include <stdbool.h>

typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned int       uint;
typedef unsigned long long uint64;
typedef unsigned long	   ulong;

#define NULL 0

#ifdef __GNUC__
#define MAKE_VISIBLE __attribute__((visibility("default")))
#define PREVENT_OPTIMIZE __attribute__((used))
#define DLL_IMPORT __attribute__((weak))
#define DLL_EXPORT MAKE_VISIBLE PREVENT_OPTIMIZE
#else // mainly to make intellisense play nice
#define MAKE_VISIBLE
#define PREVENT_OPTIMIZE
#define DLL_IMPORT
#define DLL_EXPORT
#endif

typedef enum _PadButton
{
    kButtonCRight       = (1 << 0),
    kButtonCLeft        = (1 << 1),
    kButtonCDown        = (1 << 2),
    kButtonCUp          = (1 << 3),
    kButtonRTrig        = (1 << 4),
    kButtonLTrig        = (1 << 5),
    kButtonDPadRight    = (1 << 0) << 8,
    kButtonDPadLeft     = (1 << 1) << 8,
    kButtonDPadDown     = (1 << 2) << 8,
    kButtonDPadUp       = (1 << 3) << 8,
    kButtonStart        = (1 << 4) << 8,
    kButtonZ            = (1 << 5) << 8,
    kButtonB            = (1 << 6) << 8,
    kButtonA            = (1 << 7) << 8,
} PadButton;

typedef enum _GameState
{
    kGameStateInvalid           = 0,
    kGameStateInGame            = 1,
    kGameStateMenuBoot          = 2,
    kGameStateMenuRare          = 3,
    kGameStateDebug             = 4,
    kGameStateMenuFileSelect    = 5,
    kGameStateMenuNameInput     = 6,
    kGameStateUnknown7          = 7,
    kGameStateMenuPause         = 8,
    kGameStateMenuGameOver      = 9,
} GameState;

typedef struct _vec3f
{
    float x, y, z;
} vec3f;

typedef struct _vec3i16
{
    int16 x, y, z;
} vec3i16;

typedef struct _ObjectInstance
{
    vec3i16 rot;
    int16 unk0;
    float scale;
    vec3f pos;
} ObjectInstance;

#endif // _DINO_BASE_H_