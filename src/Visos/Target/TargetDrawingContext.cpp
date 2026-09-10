#include "TargetDrawingContext.h"

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

// FUNCTION: LEMBALL 0x00458250
void TargetDrawingContext::SetDC(void* p_hDC)
{
	m_hDC = p_hDC;
}

TargetDrawingContext::~TargetDrawingContext()
{
}
