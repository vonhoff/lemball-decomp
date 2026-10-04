#include "CPlatformServices.h"

#include "Visos/Input/CMasterInput.h"
#include "Visos/Streams/CVSIOs.h"
#include "Visos/Queues/Message.h"
#include "Visos/File/VsFile.h"
#include "Platform/Windows/Input/InputTranslationEntry.h"

#include <new.h>
#include <string.h>

struct _Filet;

#pragma intrinsic(strlen, strcpy, strcat)

enum ePlatformApiValue {
	REGISTRY_HIVE_LOCAL_MACHINE = 0x80000002,
	REGISTRY_ACCESS_ALL = 0xf003f,
	REGISTRY_VALUE_STRING = 1,
	REGISTRY_VALUE_TYPE_UNSET = 0xffffffff,
	REGISTRY_API_SUCCESS = 0,
	PLATFORM_DRIVE_TYPE_CDROM = 5,
	LOGICAL_DRIVE_BITMAP_BITS = 32,
	LOGICAL_DRIVE_BITMAP_CURRENT_DRIVE_MASK = 1,
	LOGICAL_DRIVE_BITMAP_NEXT_DRIVE_SHIFT = 1
};

extern "C" __declspec(dllimport) unsigned int __stdcall GetCurrentDirectoryA(unsigned int p_length, char* p_buffer);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLogicalDrives();
extern "C" __declspec(dllimport) unsigned int __stdcall GetDriveTypeA(const char* p_root);
extern "C" __declspec(dllimport) long __stdcall RegOpenKeyExA(void* p_key,
															  const char* p_subkey,
															  unsigned int p_options,
															  unsigned int p_access,
															  void** p_result);
extern "C" __declspec(dllimport) long __stdcall RegSetValueExA(void* p_key,
															   const char* p_name,
															   unsigned int p_reserved,
															   unsigned int p_type,
															   const unsigned char* p_data,
															   unsigned int p_size);
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(void* p_key);
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(void* p_key,
																 const char* p_name,
																 unsigned int* p_reserved,
																 unsigned int* p_type,
																 unsigned char* p_data,
																 unsigned int* p_size);

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
	void* storage;
	unsigned int length;

	storage = operator new(sizeof(CPlatformServices));
	if (storage == NULL) {
		g_pTargetPlatformServices = NULL;
	}
	else {
		g_pTargetPlatformServices = new (storage) CPlatformServices();
	}
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
	void* key;
	const char* value;
	unsigned int valueSize;
	long status;

	strcpy(keyPath, g_szRegistrySoftwarePrefix);
	strcat(keyPath, p_subkey);
	status = RegOpenKeyExA((void*) REGISTRY_HIVE_LOCAL_MACHINE, keyPath, 0, REGISTRY_ACCESS_ALL, &key);
	if (status != REGISTRY_API_SUCCESS) {
		return false;
	}
	value = g_szRegistryRunning;
	if (p_enabled == 0) {
		value = g_szRegistryNotRunning;
	}
	valueSize = strlen(value) + 1;
	status = RegSetValueExA(key,
							g_szRegistryValueRunning,
							0,
							REGISTRY_VALUE_STRING,
							(const unsigned char*) value,
							valueSize);
	RegCloseKey(key);
	return status == REGISTRY_API_SUCCESS;
}

// FUNCTION: LEMBALL 0x0045eda0
char* CPlatformServices::GetCDDir(const char* p_requiredFile)
{
	char candidate[256];
	unsigned int drives;
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
			if (GetDriveTypeA(g_szCDRootPath) == PLATFORM_DRIVE_TYPE_CDROM) {
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
	void* key;
	unsigned int size;
	unsigned int type;

	g_szSourceDiskPath[0] = 0;
	key = NULL;
	if (RegOpenKeyExA((void*) REGISTRY_HIVE_LOCAL_MACHINE,
					  "SOFTWARE\\Visual Sciences\\Lemmings Paintball",
					  0,
					  REGISTRY_ACCESS_ALL,
					  &key) != REGISTRY_API_SUCCESS) {
		return g_szSourceDiskPath;
	}
	type = REGISTRY_VALUE_TYPE_UNSET;
	size = sizeof(g_szSourceDiskPath);
	if (RegQueryValueExA(key, "SrcDisk", NULL, &type, (unsigned char*) g_szSourceDiskPath, &size) !=
		REGISTRY_API_SUCCESS) {
		g_szSourceDiskPath[0] = 0;
		return g_szSourceDiskPath;
	}
	RegCloseKey(key);
	return g_szSourceDiskPath;
}
