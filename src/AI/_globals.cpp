#include "_globals.h"

// Confirmed module globals.
// GLOBAL: LEMBALL 0x004a6410
unsigned char g_abObjectIdBitmap[32];

// GLOBAL: LEMBALL 0x004a6510
GameObject* g_pObjects[256];

// GLOBAL: LEMBALL 0x004a74bc
word g_wObjectCount;

// GLOBAL: LEMBALL 0x0049d108
unsigned char g_abBitMasks[8];

// GLOBAL: LEMBALL 0x0049cf4c
word g_wNetworkLemmingIndex;

// GLOBAL: LEMBALL 0x0049cf50
word g_wLocalLemmingIndex;

// GLOBAL: LEMBALL 0x004a640c
word g_wLemmingCount;
