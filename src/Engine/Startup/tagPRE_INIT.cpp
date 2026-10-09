#include "tagPRE_INIT.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x004aa108
unsigned int g_anPreInitCapabilities[SMALL_MEMORY_BUCKET_COUNT];

// GLOBAL: LEMBALL 0x004a27a8
tagPRE_INIT g_preInitActive = {64, 8, 0, SMALL_MEMORY_BUCKET_COUNT, g_anPreInitCapabilities, 3, NULL};

// GLOBAL: LEMBALL 0x004a6258
tagPRE_INIT g_preInit = {0, 0, 0, 0, NULL, 0, NULL};
