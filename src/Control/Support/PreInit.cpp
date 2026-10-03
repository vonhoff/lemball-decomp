#include "PreInit.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x004aa108
unsigned int g_anPreInitCapabilities[7];

// GLOBAL: LEMBALL 0x004a27a8
PreInit g_preInitActive = {64, 8, 0, 7, g_anPreInitCapabilities, 3, NULL};

// GLOBAL: LEMBALL 0x004a6258
PreInit g_preInit = {0, 0, 0, 0, NULL, 0, NULL};
