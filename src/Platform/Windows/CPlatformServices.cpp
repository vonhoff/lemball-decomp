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

enum eWindowsVirtualKey {
	WINDOWS_VK_BACK = 0x08,
	WINDOWS_VK_RETURN = 0x0d,
	WINDOWS_VK_SHIFT = 0x10,
	WINDOWS_VK_SPACE = 0x20,
	WINDOWS_VK_LEFT = 0x25,
	WINDOWS_VK_UP = 0x26,
	WINDOWS_VK_RIGHT = 0x27,
	WINDOWS_VK_DOWN = 0x28,
	WINDOWS_VK_DELETE = 0x2e,
	WINDOWS_VK_NUMPAD0 = 0x60,
	WINDOWS_VK_NUMPAD1 = 0x61,
	WINDOWS_VK_NUMPAD2 = 0x62,
	WINDOWS_VK_NUMPAD3 = 0x63,
	WINDOWS_VK_NUMPAD4 = 0x64,
	WINDOWS_VK_NUMPAD5 = 0x65,
	WINDOWS_VK_NUMPAD6 = 0x66,
	WINDOWS_VK_NUMPAD7 = 0x67,
	WINDOWS_VK_NUMPAD8 = 0x68,
	WINDOWS_VK_NUMPAD9 = 0x69,
	WINDOWS_VK_ESCAPE = 0x1b,
	WINDOWS_VK_F4 = 0x73,
	WINDOWS_VK_LSHIFT = 0xa0,
	WINDOWS_VK_OEM_COMMA = 0xbc,
	WINDOWS_VK_OEM_PERIOD = 0xbe
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

// FUNCTION: LEMBALL 0x00456660
bool InitInput()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state | MASTER_INPUT_ACTIVE_STATE_MASK;
	return true;
}

// FUNCTION: LEMBALL 0x00456670
bool QuitInput()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state & ~MASTER_INPUT_ACTIVE_STATE_MASK;
	return true;
}

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

// FUNCTION: LEMBALL 0x00472220
bool __stdcall HandleInputQuitEvent(const Message* p_event)
{
	switch ((unsigned int) p_event->m_type) {
	case MESSAGE_KEY_UP:
		if (p_event->m_payload == NULL &&
			(p_event->m_code == INPUT_KEY_ACTIVATE || p_event->m_code == INPUT_KEY_DELETE)) {
			g_dwInputQuitRequested = 1;
			return false;
		}
		break;
	case MESSAGE_MOUSE_BUTTON_UP:
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
InputTranslationEntry g_dwInputTranslationPairs[61] = {{WINDOWS_VK_SPACE, INPUT_KEY_SPACE},
													   {WINDOWS_VK_OEM_PERIOD, INPUT_KEY_PERIOD},
													   {WINDOWS_VK_OEM_COMMA, INPUT_KEY_COMMA},
													   {WINDOWS_VK_F4, INPUT_KEY_F4},
													   {WINDOWS_VK_ESCAPE, INPUT_KEY_ESCAPE},
													   {'A', INPUT_KEY_A},
													   {'B', INPUT_KEY_B},
													   {'C', INPUT_KEY_C},
													   {'D', INPUT_KEY_D},
													   {'E', INPUT_KEY_E},
													   {'F', INPUT_KEY_F},
													   {'G', INPUT_KEY_G},
													   {'H', INPUT_KEY_H},
													   {'I', INPUT_KEY_I},
													   {'J', INPUT_KEY_J},
													   {'K', INPUT_KEY_K},
													   {'L', INPUT_KEY_L},
													   {'M', INPUT_KEY_M},
													   {'N', INPUT_KEY_N},
													   {'O', INPUT_KEY_O},
													   {'P', INPUT_KEY_P},
													   {'Q', INPUT_KEY_Q},
													   {'R', INPUT_KEY_R},
													   {'S', INPUT_KEY_S},
													   {'T', INPUT_KEY_T},
													   {'U', INPUT_KEY_U},
													   {'V', INPUT_KEY_V},
													   {'W', INPUT_KEY_W},
													   {'X', INPUT_KEY_X},
													   {'Y', INPUT_KEY_Y},
													   {'Z', INPUT_KEY_Z},
													   {'0', INPUT_KEY_0},
													   {'1', INPUT_KEY_1},
													   {'2', INPUT_KEY_2},
													   {'3', INPUT_KEY_3},
													   {'4', INPUT_KEY_4},
													   {'5', INPUT_KEY_5},
													   {'6', INPUT_KEY_6},
													   {'7', INPUT_KEY_7},
													   {'8', INPUT_KEY_8},
													   {'9', INPUT_KEY_9},
													   {WINDOWS_VK_NUMPAD0, INPUT_KEY_0},
													   {WINDOWS_VK_NUMPAD1, INPUT_KEY_1},
													   {WINDOWS_VK_NUMPAD2, INPUT_KEY_2},
													   {WINDOWS_VK_NUMPAD3, INPUT_KEY_3},
													   {WINDOWS_VK_NUMPAD4, INPUT_KEY_4},
													   {WINDOWS_VK_NUMPAD5, INPUT_KEY_5},
													   {WINDOWS_VK_NUMPAD6, INPUT_KEY_6},
													   {WINDOWS_VK_NUMPAD7, INPUT_KEY_7},
													   {WINDOWS_VK_NUMPAD8, INPUT_KEY_8},
													   {WINDOWS_VK_NUMPAD9, INPUT_KEY_9},
													   {WINDOWS_VK_UP, INPUT_KEY_UP},
													   {WINDOWS_VK_DOWN, INPUT_KEY_DOWN},
													   {WINDOWS_VK_LEFT, INPUT_KEY_LEFT},
													   {WINDOWS_VK_RIGHT, INPUT_KEY_RIGHT},
													   {WINDOWS_VK_RETURN, INPUT_KEY_RETURN},
													   {WINDOWS_VK_DELETE, INPUT_KEY_DELETE},
													   {WINDOWS_VK_DELETE, INPUT_KEY_DELETE},
													   {WINDOWS_VK_BACK, INPUT_KEY_BACKSPACE},
													   {WINDOWS_VK_SHIFT, INPUT_KEY_SHIFT},
													   {WINDOWS_VK_LSHIFT, INPUT_KEY_LEFT_SHIFT}};

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
