#include "_globals.h"

// Confirmed module globals.
// GLOBAL: LEMBALL 0x0049d040
unsigned int g_anFacingDirectionYFlip[8];

// GLOBAL: LEMBALL 0x004a62a4
int g_nDemoMode;

// GLOBAL: LEMBALL 0x004a1bcc
int* g_pSentinel;

// GLOBAL: LEMBALL 0x0049ce04
unsigned int g_dwGameTick;

// GLOBAL: LEMBALL 0x0049cb70
int g_anPasswordPermutation[8];

// GLOBAL: LEMBALL 0x0049cb9c
char g_szUnlockPassword[11];

// GLOBAL: LEMBALL 0x0049cedc
char g_szReadBinaryMode[3];

// GLOBAL: LEMBALL 0x004a27a8
PreInit g_preInitActive;

// GLOBAL: LEMBALL 0x004a6258
PreInit g_preInit;

// GLOBAL: LEMBALL 0x004a6304
int g_nEditLevelMode;

// GLOBAL: LEMBALL 0x004a6308
int g_nPlayLevelMode;

// GLOBAL: LEMBALL 0x004a6314
char g_szCommandLineLevelFile[232];

// GLOBAL: LEMBALL 0x004a63fc
char* g_pActiveLevelFile;

// GLOBAL: LEMBALL 0x004a6400
void* g_pLevelFileData;
