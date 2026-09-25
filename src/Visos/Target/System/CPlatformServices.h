#ifndef LEMBALL_VISOS_TARGET_SYSTEM_CPLATFORMSERVICES_H
#define LEMBALL_VISOS_TARGET_SYSTEM_CPLATFORMSERVICES_H

#include "../Input/InputTranslationEntry.h"

// SIZE 0x01
class CPlatformServices {
public:
	CPlatformServices();
	~CPlatformServices();
	char* GetCDDir(const char* p_requiredFile);
	bool WriteRegistryFlag(const char* p_subkey, int p_enabled);
};

bool InitInput();
bool QuitInput();
bool InitPlatformServices();
bool QuitPlatformServices();

extern CPlatformServices* g_pTargetPlatformServices;
extern unsigned int g_dwInputQuitRequested;
extern InputTranslationEntry g_dwInputTranslationPairs[61];
extern unsigned int g_dwWindowQuitRequested;
extern char g_szCDRootPath[4];
extern char g_szRegistrySoftwarePrefix[28];
extern char g_szRegistryRunning[8];
extern char g_szRegistryNotRunning[4];
extern char g_szRegistryValueRunning[8];
extern char g_szFileModeRead[4];
#endif
