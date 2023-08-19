#ifndef _DINO_BASE_H_
#define _DINO_BASE_H_

#include <stdbool.h>

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]

#define OFF_STRUCT(members) union { members }

#define OFF_MEMB(offset, member)                 \
        PACKED_STRUCT {                          \
            char MAKE_PAD(offset);               \
            member;                              \
        }


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
#define TRUE 1
#define FALSE 0

#ifdef __GNUC__
#define MAKE_VISIBLE __attribute__((visibility("default")))
#define PREVENT_OPTIMIZE __attribute__((used))
#define DLL_IMPORT __attribute__((weak))
#define DLL_EXPORT MAKE_VISIBLE PREVENT_OPTIMIZE
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end
#define PACKED_STRUCT struct __attribute__ ((__packed__))
#else // mainly to make intellisense play nice
#define MAKE_VISIBLE
#define PREVENT_OPTIMIZE
#define DLL_IMPORT
#define DLL_EXPORT
#define va_list void*
#define va_start
#define va_arg
#define va_end
#define PACKED_STRUCT struct
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

typedef enum _PlayerType
{
    kPlayerTypeSabre            = 0,
    kPlayerTypeKrystal          = 1
} PlayerType;

typedef PACKED_STRUCT _vec3f
{
    float x, y, z;
} vec3f;

typedef PACKED_STRUCT _vec3ui16
{
    uint16 x, y, z;
} vec3ui16;

typedef PACKED_STRUCT _mtx44
{
    float m[16];
} mtx44;

typedef PACKED_STRUCT _Transform // 24 bytes
{
    //x=yaw, y=pitch, z=roll
    vec3ui16 rot;
    int16 unk0;
    float scale;
    vec3f pos;
} Transform;

typedef enum _ObjectSetupFlag
{
    kObjectSetupFlag_DisableModels  = 1,
} ObjectSetupFlag;

typedef PACKED_STRUCT _Object
{
    OFF_STRUCT(
        OFF_MEMB(0, float unk0);
        OFF_MEMB(4, float scale);
        OFF_MEMB(8, uint32 modelIdsOffset);
        OFF_MEMB(16, uint32 sequenceBonesOffset);
        OFF_MEMB(28, uint32 objSeqsOffset);
        OFF_MEMB(68, uint32 setupFlags);
        OFF_MEMB(93, uint8 numModels);
        OFF_MEMB(95, char name[16]);
        OFF_MEMB(111, uint8 classType);
        OFF_MEMB(114, uint8 numSequenceBones);
        OFF_MEMB(120, int16 modLinesId);
        OFF_MEMB(122, uint16 numObjSeqs);
    );
} Object;

typedef PACKED_STRUCT _PlayerState
{
    OFF_STRUCT(
        OFF_MEMB(200, void* weaponObject);
        OFF_MEMB(620, int16 nextStateId);
        OFF_MEMB(622, int16 currentStateId);
        OFF_MEMB(626, int8 isStateSwitch);
        OFF_MEMB(948, void* weaponData);
        OFF_MEMB(952, void* charSpecificData1);
        OFF_MEMB(956, void* charSpecificHandlerFunc);
        OFF_MEMB(2208, int8 numWeapons);
        OFF_MEMB(2228, int8 playerType);
    );
} PlayerState;

typedef PACKED_STRUCT _ObjectInstance
{
    OFF_STRUCT(
        OFF_MEMB(0, Transform transform);
        OFF_MEMB(6, uint16 someFlags);
        OFF_MEMB(48, void* rootObjectInstance);
        OFF_MEMB(70, int16 objectTypeId);
        OFF_MEMB(80, Object* objDef);
        OFF_MEMB(124, void* loadedModels);
        OFF_MEMB(152, float animProgress);
        OFF_MEMB(156, float animProgress2);
        OFF_MEMB(160, int16 currentAnimId);
        OFF_MEMB(173, char someActiveModelId);
        OFF_MEMB(176, uint16 someFlags2);
        OFF_MEMB(184, PlayerState* playerState);
        OFF_MEMB(188, void* updateFunc);
        OFF_MEMB(200, void* weaponObject);
    );
} ObjectInstance;

typedef PACKED_STRUCT _RawControllerState
{
    uint32 unk0;
    uint32 state;
} RawControllerState;

typedef PACKED_STRUCT _RawControllerStateArray
{
    RawControllerState states[4];
} RawControllerStateArray;

typedef PACKED_STRUCT _DLLEntryPoint
{
    void* exports;
    void* dllEnd;
} DLLEntryPoint;

typedef PACKED_STRUCT _DLLInfo
{
    int id;
    int numRefs;
    DLLEntryPoint entryPoint;
} DLLInfo;

#endif // _DINO_BASE_H_