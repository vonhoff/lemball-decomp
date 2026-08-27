#ifndef LEMBALL_VISOS_TARGET_TARGETPLATFORMSERVICES_H
#define LEMBALL_VISOS_TARGET_TARGETPLATFORMSERVICES_H

#include "../../Common.h"
#include "TargetInputTranslationEntry.h" // complete type

// SIZE 0x01
class TargetPlatformServices {
public:
	TargetPlatformServices();
	~TargetPlatformServices();
	char* GetCDDir(const char* p_requiredFile);
	bool WriteRegistryFlag(const char* p_subkey, int p_enabled);
};

bool TargetInputInit();
bool TargetInputQuit();
bool TargetPlatformServicesInit();
bool TargetPlatformServicesQuit();

extern TargetPlatformServices* g_pTargetPlatformServices;
extern unsigned int g_dwInputQuitRequested;
extern TargetInputTranslationEntry g_dwInputTranslationPairs[61];
extern unsigned int g_dwWindowQuitRequested;
extern char g_szCDRootPath[4];
extern char g_szRegistrySoftwarePrefix[28];
extern char g_szRegistryRunning[8];
extern char g_szRegistryNotRunning[4];
extern char g_szRegistryValueRunning[8];
extern char g_szFileModeRead[4];
#endif
