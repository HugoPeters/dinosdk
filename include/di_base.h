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
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end
#else // mainly to make intellisense play nice
#define MAKE_VISIBLE
#define PREVENT_OPTIMIZE
#define DLL_IMPORT
#define DLL_EXPORT
#define va_list void*
#define va_start
#define va_arg
#define va_end
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

typedef struct _vec3ui16
{
    uint16 x, y, z;
} vec3ui16;

typedef struct _mtx44
{
    float m[16];
} mtx44;

typedef struct _Transform // 24 bytes
{
    //x=yaw, y=pitch, z=roll
    vec3ui16 rot;
    int16 unk0;
    float scale;
    vec3f pos;
} Transform;

typedef struct _Object
{
    float unk0;     // 0-4
    float scale;    // 4-8
    char pad[0x5f-8];
    char name[16];
    uint8 classType;
} Object;

typedef struct _ObjectInstance
{
    Transform transform; // 0-24
    char padToObjDef[0x50 - 24];
    Object* objDef;
} ObjectInstance;

typedef struct _RawControllerState
{
    uint32 unk0;
    uint32 state;
} RawControllerState;

typedef struct _RawControllerStateArray
{
    RawControllerState states[4];
} RawControllerStateArray;

#endif // _DINO_BASE_H_