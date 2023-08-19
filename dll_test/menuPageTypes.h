#ifndef _menuPageTypes_h_
#define _menuPageTypes_h_

typedef enum _DebugMenuPage
{
    kMenuPage_MainGameplay = 0,
    kMenuPage_MainObjects,
    kMenuPage_MainCamera,
    kMenuPage_MainWarp,
    kMenuPage_SelectHacks,

    kMenuPage_NumMainPages,

    kMenuPage_GameplayMovePlayer,
    kMenuPage_CameraFreeCam,
    kMenuPage_SelectPlayer,

    kMenuPage_NumPages
} DebugMenuPage;

#endif // _menuPageTypes_h_
