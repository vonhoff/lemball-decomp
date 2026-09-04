#include "TargetPlatformServices.h"

#include "../Foundation/MasterInput.h"
#include "../Foundation/VsFile.h"
#include "../Foundation/VsIOs.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strlen, strcpy, strcat)

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

// FUNCTION: LEMBALL 0x00456660
bool TargetInputInit()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state | 3;
	return 1;
}

// FUNCTION: LEMBALL 0x00456670
bool TargetInputQuit()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state & 0xfffffffc;
	return 1;
}

// FUNCTION: LEMBALL 0x00456680
bool TargetPlatformServicesInit()
{
	void* storage;
	unsigned int length;

	storage = operator new(1);
	if (storage == 0) {
		g_pTargetPlatformServices = 0;
	}
	else {
		g_pTargetPlatformServices = new (storage) TargetPlatformServices();
	}
	GetCurrentDirectoryA(0x100, g_szCurrentDirectory);
	length = strlen(g_szCurrentDirectory);
	if (g_szCurrentDirectory[length - 1] == '\\') {
		g_szCurrentDirectory[length - 1] = 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004566f0
bool TargetPlatformServicesQuit()
{
	TargetPlatformServices* services;

	services = g_pTargetPlatformServices;
	if (g_pTargetPlatformServices != 0) {
		delete services;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0045ec90
TargetPlatformServices::TargetPlatformServices()
{
}

// FUNCTION: LEMBALL 0x0045eca0
TargetPlatformServices::~TargetPlatformServices()
{
}

// FUNCTION: LEMBALL 0x0045ecb0
bool TargetPlatformServices::WriteRegistryFlag(const char* p_subkey, int p_enabled)
{
	char keyPath[256];
	void* key;
	const char* value;
	unsigned int valueSize;
	long status;

	strcpy(keyPath, g_szRegistrySoftwarePrefix);
	strcat(keyPath, p_subkey);
	status = RegOpenKeyExA((void*) 0x80000002, keyPath, 0, 0xf003f, &key);
	if (status != 0) {
		return 0;
	}
	value = g_szRegistryRunning;
	if (p_enabled == 0) {
		value = g_szRegistryNotRunning;
	}
	valueSize = strlen(value) + 1;
	status = RegSetValueExA(key, g_szRegistryValueRunning, 0, 1, (const unsigned char*) value, valueSize);
	RegCloseKey(key);
	return status == 0;
}

// 68K 0x1010c30e GetCDDir__FPCc
// FUNCTION: LEMBALL 0x0045eda0
char* TargetPlatformServices::GetCDDir(const char* p_requiredFile)
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
	while (i < 0x20) {
		if ((drives & 1) != 0) {
			candidate[0] = letter;
			g_szCDRootPath[0] = letter;
			if (GetDriveTypeA(g_szCDRootPath) == 5) {
				file = VsOpen(candidate, g_szFileModeRead);
				if (file != 0) {
					VsClose(file);
					return g_szCDRootPath;
				}
			}
		}
		drives = drives >> 1;
		letter = letter + 1;
		i = i + 1;
	}
	return 0;
}

// GLOBAL: LEMBALL 0x004a1dcc
TargetPlatformServices* g_pTargetPlatformServices = 0;

// GLOBAL: LEMBALL 0x004a27a0
unsigned int g_dwInputQuitRequested = 0;

// GLOBAL: LEMBALL 0x004a2808
TargetInputTranslationEntry g_dwInputTranslationPairs[61] = {
	{0x20, 0x1f}, {0xbe, 0x20}, {0xbc, 0x21}, {0x73, 0x25}, {0x1b, 0x23}, {0x41, 0x05}, {0x42, 0x06}, {0x43, 0x07},
	{0x44, 0x08}, {0x45, 0x09}, {0x46, 0x0a}, {0x47, 0x0b}, {0x48, 0x0c}, {0x49, 0x0d}, {0x4a, 0x0e}, {0x4b, 0x0f},
	{0x4c, 0x10}, {0x4d, 0x11}, {0x4e, 0x12}, {0x4f, 0x13}, {0x50, 0x14}, {0x51, 0x15}, {0x52, 0x16}, {0x53, 0x17},
	{0x54, 0x18}, {0x55, 0x19}, {0x56, 0x1a}, {0x57, 0x1b}, {0x58, 0x1c}, {0x59, 0x1d}, {0x5a, 0x1e}, {0x30, 0x39},
	{0x31, 0x3a}, {0x32, 0x3b}, {0x33, 0x3c}, {0x34, 0x3d}, {0x35, 0x3e}, {0x36, 0x3f}, {0x37, 0x40}, {0x38, 0x41},
	{0x39, 0x42}, {0x60, 0x39}, {0x61, 0x3a}, {0x62, 0x3b}, {0x63, 0x3c}, {0x64, 0x3d}, {0x65, 0x3e}, {0x66, 0x3f},
	{0x67, 0x40}, {0x68, 0x41}, {0x69, 0x42}, {0x26, 0x01}, {0x28, 0x02}, {0x25, 0x03}, {0x27, 0x04}, {0x0d, 0x4c},
	{0x2e, 0x4d}, {0x2e, 0x4d}, {0x08, 0x4e}, {0x10, 0x49}, {0xa0, 0x4a}};

// GLOBAL: LEMBALL 0x004a818c
unsigned int g_dwWindowQuitRequested;

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
