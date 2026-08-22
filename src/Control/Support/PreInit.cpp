#include "PreInit.h"

#include "../../Visos/Foundation/Arena.h"

// GLOBAL: LEMBALL 0x004a27a8
PreInit g_preInitActive = {64, 8, 0, 7, (unsigned int*) ((char*) &g_pMasterArena + 8), 3, 0};

// GLOBAL: LEMBALL 0x004a6258
PreInit g_preInit = {0, 0, 0, 0, 0, 0, 0};
