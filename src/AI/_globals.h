#ifndef LEMBALL_SCAFFOLD_AI__GLOBALS_H
#define LEMBALL_SCAFFOLD_AI__GLOBALS_H

#include "../Common.h"
#include "Base/GameObject.h" // complete type

// Confirmed module globals.
extern unsigned char g_abObjectIdBitmap[32];
extern GameObject* g_pObjects[256];
extern word g_wObjectCount;
extern unsigned char g_abBitMasks[8];
extern word g_wNetworkLemmingIndex;
extern word g_wLocalLemmingIndex;
extern word g_wLemmingCount;
#endif
