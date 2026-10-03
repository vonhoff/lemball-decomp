#include "CPlatformServices.h"

#include "../../Foundation/CMasterInput.h"
#include "../../Foundation/CVSIOs.h"
#include "../../Foundation/Message.h"
#include "../../Foundation/VsFile.h"
#include "Visos/Target/Input/InputTranslationEntry.h"

#include <new.h>
#include <string.h>

struct _Filet;

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
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(void* p_key,
																 const char* p_name,
																 unsigned int* p_reserved,
																 unsigned int* p_type,
																 unsigned char* p_data,
																 unsigned int* p_size);

// FUNCTION: LEMBALL 0x00456660
bool InitInput()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state | 3;
	return true;
}

// FUNCTION: LEMBALL 0x00456670
bool QuitInput()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state & 0xfffffffc;
	return true;
}

// FUNCTION: LEMBALL 0x00456680
bool InitPlatformServices()
{
	void* storage;
	unsigned int length;

	storage = operator new(1);
	if (storage == NULL) {
		g_pTargetPlatformServices = NULL;
	}
	else {
		g_pTargetPlatformServices = new (storage) CPlatformServices();
	}
	GetCurrentDirectoryA(0x100, g_szCurrentDirectory);
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
	status = RegOpenKeyExA((void*) 0x80000002, keyPath, 0, 0xf003f, &key);
	if (status != 0) {
		return false;
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
	while (i < 0x20) {
		if ((drives & 1) != 0) {
			candidate[0] = letter;
			g_szCDRootPath[0] = letter;
			if (GetDriveTypeA(g_szCDRootPath) == 5) {
				file = vsOpen(candidate, g_szFileModeRead);
				if (file != NULL) {
					vsClose(file);
					return g_szCDRootPath;
				}
			}
		}
		drives = drives >> 1;
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
	if (RegOpenKeyExA((void*) 0x80000002, "SOFTWARE\\Visual Sciences\\Lemmings Paintball", 0, 0xf003f, &key) != 0) {
		return g_szSourceDiskPath;
	}
	type = 0xffffffff;
	size = 0x100;
	if (RegQueryValueExA(key, "SrcDisk", NULL, &type, (unsigned char*) g_szSourceDiskPath, &size) != 0) {
		g_szSourceDiskPath[0] = 0;
		return g_szSourceDiskPath;
	}
	RegCloseKey(key);
	return g_szSourceDiskPath;
}

// FUNCTION: LEMBALL 0x00472220
bool __stdcall HandleInputQuitEvent(const Message* p_event)
{
	switch ((unsigned int) p_event->m_type) {
	case 3:
		if (p_event->m_payload == NULL && (p_event->m_code == 0x22 || p_event->m_code == 0x2e)) {
			g_dwInputQuitRequested = 1;
			return false;
		}
		break;
	case 5:
		g_dwInputQuitRequested = 1;
		break;
	}
	return false;
}

// GLOBAL: LEMBALL 0x004a1dcc
CPlatformServices* g_pTargetPlatformServices = NULL;

// GLOBAL: LEMBALL 0x004a27a0
unsigned int g_dwInputQuitRequested = 0;

// GLOBAL: LEMBALL 0x004a2808
InputTranslationEntry g_dwInputTranslationPairs[61] = {
	{0x20, INPUT_KEY_SPACE},     {0xbe, INPUT_KEY_PERIOD}, {0xbc, INPUT_KEY_COMMA},     {0x73, INPUT_KEY_F4},
	{0x1b, INPUT_KEY_ESCAPE},    {0x41, INPUT_KEY_A},      {0x42, INPUT_KEY_B},         {0x43, INPUT_KEY_C},
	{0x44, INPUT_KEY_D},         {0x45, INPUT_KEY_E},      {0x46, INPUT_KEY_F},         {0x47, INPUT_KEY_G},
	{0x48, INPUT_KEY_H},         {0x49, INPUT_KEY_I},      {0x4a, INPUT_KEY_J},         {0x4b, INPUT_KEY_K},
	{0x4c, INPUT_KEY_L},         {0x4d, INPUT_KEY_M},      {0x4e, INPUT_KEY_N},         {0x4f, INPUT_KEY_O},
	{0x50, INPUT_KEY_P},         {0x51, INPUT_KEY_Q},      {0x52, INPUT_KEY_R},         {0x53, INPUT_KEY_S},
	{0x54, INPUT_KEY_T},         {0x55, INPUT_KEY_U},      {0x56, INPUT_KEY_V},         {0x57, INPUT_KEY_W},
	{0x58, INPUT_KEY_X},         {0x59, INPUT_KEY_Y},      {0x5a, INPUT_KEY_Z},         {0x30, INPUT_KEY_0},
	{0x31, INPUT_KEY_1},         {0x32, INPUT_KEY_2},      {0x33, INPUT_KEY_3},         {0x34, INPUT_KEY_4},
	{0x35, INPUT_KEY_5},         {0x36, INPUT_KEY_6},      {0x37, INPUT_KEY_7},         {0x38, INPUT_KEY_8},
	{0x39, INPUT_KEY_9},         {0x60, INPUT_KEY_0},      {0x61, INPUT_KEY_1},         {0x62, INPUT_KEY_2},
	{0x63, INPUT_KEY_3},         {0x64, INPUT_KEY_4},      {0x65, INPUT_KEY_5},         {0x66, INPUT_KEY_6},
	{0x67, INPUT_KEY_7},         {0x68, INPUT_KEY_8},      {0x69, INPUT_KEY_9},         {0x26, INPUT_KEY_UP},
	{0x28, INPUT_KEY_DOWN},      {0x25, INPUT_KEY_LEFT},   {0x27, INPUT_KEY_RIGHT},     {0x0d, INPUT_KEY_RETURN},
	{0x2e, INPUT_KEY_DELETE},    {0x2e, INPUT_KEY_DELETE}, {0x08, INPUT_KEY_BACKSPACE}, {0x10, INPUT_KEY_SHIFT},
	{0xa0, INPUT_KEY_LEFT_SHIFT}};

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
