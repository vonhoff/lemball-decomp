#include "CPlatformServices.h"

#include "Engine/Files/VsFile.h"
#include "WindowsTypes.h"

#include <string.h>

struct _Filet;

enum ePlatformApiValue {
	REGISTRY_VALUE_TYPE_UNSET = 0xffffffff,
	LOGICAL_DRIVE_BITMAP_BITS = 32,
	LOGICAL_DRIVE_BITMAP_CURRENT_DRIVE_MASK = 1,
	LOGICAL_DRIVE_BITMAP_NEXT_DRIVE_SHIFT = 1
};

// GLOBAL: LEMBALL 0x004a1dcc
CPlatformServices* g_pTargetPlatformServices = NULL;

// GLOBAL: LEMBALL 0x004a1dd0
char g_szCDRootPath[4] = "X:\\";

// GLOBAL: LEMBALL 0x004a1dd4
char g_szRegistrySoftwarePrefix[28] = "SOFTWARE\\Visual Sciences\\";

// GLOBAL: LEMBALL 0x004a1df0
char g_szRegistryRunning[8] = "running";

// GLOBAL: LEMBALL 0x004a1df8
char g_szRegistryNotRunning[4] = "";

// GLOBAL: LEMBALL 0x004a1dfc
char g_szRegistryValueRunning[8] = "Running";

// GLOBAL: LEMBALL 0x004a1e04
char g_szFileModeRead[4] = "r";

// FUNCTION: LEMBALL 0x00456680
bool InitPlatformServices()
{
	unsigned int length;

	g_pTargetPlatformServices = new CPlatformServices();
	GetCurrentDirectoryA(sizeof(g_szCurrentDirectory), g_szCurrentDirectory);
	length = strlen(g_szCurrentDirectory) - 1;
	if (g_szCurrentDirectory[length] == '\\') {
		g_szCurrentDirectory[length] = 0;
	}
	return true;
}

// FUNCTION: LEMBALL 0x004566f0
bool QuitPlatformServices()
{
	CPlatformServices* services;

	services = g_pTargetPlatformServices;
	if (g_pTargetPlatformServices != NULL) {
		delete services;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045ec90
CPlatformServices::CPlatformServices()
{
}

// FUNCTION: LEMBALL 0x0045eca0
CPlatformServices::~CPlatformServices()
{
}

// FUNCTION: LEMBALL 0x0045ecb0
bool CPlatformServices::WriteRegistryFlag(const char* p_subkey, int p_enabled)
{
	char keyPath[256];
	HKEY key;
	const char* value;
	DWORD valueSize;
	long status;

	strcpy(keyPath, g_szRegistrySoftwarePrefix);
	strcat(keyPath, p_subkey);
	status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &key);
	if (status != ERROR_SUCCESS) {
		return false;
	}
	value = g_szRegistryRunning;
	if (p_enabled == 0) {
		value = g_szRegistryNotRunning;
	}
	valueSize = strlen(value) + 1;
	status = RegSetValueExA(key, g_szRegistryValueRunning, 0, REG_SZ, (const unsigned char*) value, valueSize);
	RegCloseKey(key);
	return status == ERROR_SUCCESS;
}

// FUNCTION: LEMBALL 0x0045eda0
char* CPlatformServices::GetCDDir(const char* p_requiredFile)
{
	char candidate[256];
	DWORD drives;
	char letter;
	int i;
	_Filet* file;

	letter = 'A';
	drives = GetLogicalDrives();
	strcpy(candidate, g_szCDRootPath);
	strcat(candidate, p_requiredFile);
	i = 0;
	while (i < LOGICAL_DRIVE_BITMAP_BITS) {
		if ((drives & LOGICAL_DRIVE_BITMAP_CURRENT_DRIVE_MASK) != 0) {
			candidate[0] = letter;
			g_szCDRootPath[0] = letter;
			if (GetDriveTypeA(g_szCDRootPath) == DRIVE_CDROM) {
				file = vsOpen(candidate, g_szFileModeRead);
				if (file != NULL) {
					vsClose(file);
					return g_szCDRootPath;
				}
			}
		}
		drives = drives >> LOGICAL_DRIVE_BITMAP_NEXT_DRIVE_SHIFT;
		letter = letter + 1;
		i = i + 1;
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0046dcd0
char* ReadSourceDiskRegistryPath()
{
	HKEY key;
	DWORD size;
	DWORD type;

	g_szSourceDiskPath[0] = 0;
	key = NULL;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Visual Sciences\\Lemmings Paintball", 0, KEY_ALL_ACCESS, &key) !=
		ERROR_SUCCESS) {
		return g_szSourceDiskPath;
	}
	type = REGISTRY_VALUE_TYPE_UNSET;
	size = sizeof(g_szSourceDiskPath);
	if (RegQueryValueExA(key, "SrcDisk", NULL, &type, (unsigned char*) g_szSourceDiskPath, &size) != ERROR_SUCCESS) {
		g_szSourceDiskPath[0] = 0;
		return g_szSourceDiskPath;
	}
	RegCloseKey(key);
	return g_szSourceDiskPath;
}

// GLOBAL: LEMBALL 0x004a8088
char g_szCurrentDirectory[256];

// GLOBAL: LEMBALL 0x004a9ff8
char g_szSourceDiskPath[256];
