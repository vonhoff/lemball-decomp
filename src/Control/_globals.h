#ifndef LEMBALL_SCAFFOLD_CONTROL__GLOBALS_H
#define LEMBALL_SCAFFOLD_CONTROL__GLOBALS_H

#include "../Common.h"
#include "PreInit.h" // complete type

// Confirmed module globals.
extern unsigned int g_anFacingDirectionYFlip[8];
extern int g_nDemoMode;
extern int* g_pSentinel;
extern unsigned int g_dwGameTick;
extern int g_anPasswordPermutation[8];
extern char g_szUnlockPassword[11];
extern char g_szReadBinaryMode[3];
extern PreInit g_preInitActive;
extern PreInit g_preInit;
extern int g_nEditLevelMode;
extern int g_nPlayLevelMode;
extern char g_szCommandLineLevelFile[232];
extern char* g_pActiveLevelFile;
extern void* g_pLevelFileData;
#endif
