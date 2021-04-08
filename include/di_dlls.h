#ifndef _DLLS_H_
#define _DLLS_H_

#include "di_exports.h"

typedef enum _DLLTypes
{
    kDLL_CmdMenu = 1,
    kDLL_Camera = 2,
    kDLL_Anim = 3,
    kDLL_Race = 4,
    kDLL_Amseq = 5,
    kDLL_Amsfx = 6
    // etc
} DLLTypes;

typedef struct _DLLEntryPoint
{
    void* exports;
    void* dllEnd;
} DLLEntryPoint;

typedef struct _DLLInfo
{
    int id;
    int numRefs;
    DLLEntryPoint entryPoint;
} DLLInfo;

extern DLLInfo* DLL_IMPORT  dllGetLoadedList(int* outNumDLLs);

inline DLLInfo* dllFind(int id)
{
    int numDLLs = 0;
    DLLInfo* dlls = dllGetLoadedList(&numDLLs);

    for (int i = 0; i < numDLLs; ++i)
    {
        if (dlls[i].id == id)
            return &dlls[i];
    }

    return NULL;
}

inline void* dllGetFuncs(int id)
{
    DLLInfo* dll = dllFind(id);
    return dll ? dll->entryPoint.exports : NULL;
}

#endif // _DLLS_H_