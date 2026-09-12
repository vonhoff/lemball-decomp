#include "TargetGraphicsSystemState.h"

#include "../Foundation/VsInit.h"
#include "../Foundation/VsOStream.h"
#include "../Graphics/GWnd.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/PvGdiBitmap.h"
#include "../Graphics/PvWnd.h"
#include "../Graphics/VsGdi.h"
#include "TargetGDIDriver.h"
#include "TargetGraphicsSystemState.h"
#include "TargetWinGDrawCodecState.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct IcOpen {
	unsigned int dwSize;
	unsigned int fccType;
	unsigned int fccHandler;
	unsigned int dwVersion;
	unsigned int dwFlags;
	int dwError;
};

struct IcInfo {
	unsigned int dwSize;
	unsigned int fccType;
	unsigned int fccHandler;
	unsigned int dwFlags;
	unsigned int dwVersion;
	unsigned int dwVersionICM;
	WCHAR szName[16];
	WCHAR szDescription[128];
	WCHAR szDriver[128];
};

struct IcDrawBegin {
	unsigned int dwFlags;
	void* hpal;
	void* hwnd;
	void* hdc;
	int xDst;
	int yDst;
	int dxDst;
	int dyDst;
	BITMAPINFOHEADER* lpbi;
	int xSrc;
	int ySrc;
	int dxSrc;
	int dySrc;
};

struct IcDraw {
	unsigned int dwFlags;
	void* lpFormat;
	void* lpData;
};

struct IcDrawSuggest {
	BITMAPINFOHEADER* lpbiIn;
	BITMAPINFOHEADER* lpbiSuggest;
};

unsigned int __stdcall TargetWinGDrawCodec_GetInfo(void* p_info, unsigned int p_size);

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);
extern "C" __declspec(dllimport) long __stdcall DefDriverProc(unsigned int p_driverId,
															  void* p_driverHandle,
															  unsigned int p_message,
															  long p_param1,
															  long p_param2);

// STUB: LEMBALL 0x00457e10
bool TargetGraphicsSystemState::SelectDriver(int p_driverMode)
{
	int mode;
	void* storage;

	mode = p_driverMode;
	if (p_driverMode == 8) {
		if (g_nGraphicsDriverGdk == 0) {
			if (g_nFullscreen == 0) {
				mode = 1;
			}
			else {
				mode = 3;
			}
		}
		else if (g_nFullscreen == 0) {
			mode = 6;
		}
		else {
			mode = 4;
		}
	}

	if (mode != 1 && mode != 2 && mode != 3 && mode != 4 && mode != 6) {
		mode = 1;
	}

	storage = operator new(0x1c);
	if (storage == 0) {
		g_pTargetGraphicsDriver = 0;
	}
	else {
		g_pTargetGraphicsDriver = new (storage) TargetGDIDriver();
	}

	if (g_pTargetGraphicsDriver == 0 || g_pTargetGraphicsDriver->m_ready == 0) {
		*g_pErrorOutput << "No valid driver available\n";
		return 0;
	}

	m_driverMode = 1;
	return 1;
}

// FUNCTION: LEMBALL 0x00458180
bool TargetGraphicsSystemState::IsFullscreenDriver()
{
	switch (m_driverMode) {
	case 2:
	case 3:
	case 4:
	case 5:
		return 1;
	default:
		return 0;
	}
}
