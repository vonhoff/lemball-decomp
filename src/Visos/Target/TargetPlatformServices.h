#ifndef LEMBALL_VISOS_TARGET_TARGETPLATFORMSERVICES_H
#define LEMBALL_VISOS_TARGET_TARGETPLATFORMSERVICES_H

#include "../../Common.h"
#include "TargetInputTranslationEntry.h" // complete type

// SIZE 0x01
class TargetPlatformServices {
public:
	// No functions mapped yet.
};

// Confirmed class-scoped globals.
extern TargetPlatformServices* g_pTargetPlatformServices;
extern unsigned int g_dwInputQuitRequested;
extern TargetInputTranslationEntry g_dwInputTranslationPairs[61];
extern unsigned int g_dwWindowQuitRequested;
#endif
