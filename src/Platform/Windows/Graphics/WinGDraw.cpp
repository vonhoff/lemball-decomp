#include "WinGDraw.h"

#include "Engine/Graphics/Primitives/CGDI.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"
#include "Engine/Windows/CPVWnd.h"
#include "Engine/Graphics/Surfaces/CSurface.h"
#include "WinGDrawState.h"

#include <string.h>

#define WIN32_LEAN_AND_MEAN
// clang-format off
#include <windows.h>
#define NOAVIFILE
#include <vfw.h>
// clang-format on

enum {
	WING_DRAW_API_MAJOR_VERSION = 1,
	WING_DRAW_API_MAJOR_VERSION_SHIFT = 16
};
#define WING_DRAW_VERSION (WING_DRAW_API_MAJOR_VERSION << WING_DRAW_API_MAJOR_VERSION_SHIFT)
#define WING_DRAW_TEXT_CAPACITY 256

class CAnimWnd;
extern CAnimWnd* g_pAnimWnd;
WinGDrawState* __stdcall WinGDrawOpen(void* p_openInfo);
int __stdcall WinGDrawClose(WinGDrawState* p_state);
int __stdcall WinGDrawFrame(WinGDrawState* p_state, void* p_request, long p_param2);
int __stdcall WinGDrawBegin(WinGDrawState* p_state, void* p_request, long p_param2);
int __stdcall WinGDrawEnd(WinGDrawState* p_state);
int __stdcall WinGDrawQueryFormat(WinGDrawState* p_state, void* p_format);
int __stdcall WinGDrawSuggestFormat(WinGDrawState* p_state, void* p_request, long p_param2);
int __stdcall WinGDrawChangePalette(WinGDrawState* p_state, void* p_request);
unsigned int __stdcall WinGDrawGetInfo(void* p_info, unsigned int p_size);

extern unsigned int g_dwWinGDrawColourTable[256];

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

// FUNCTION: LEMBALL 0x00478fb0
long __stdcall WinGDrawDriverProc(unsigned int p_driverId,
								  void* p_driverHandle,
								  unsigned int p_message,
								  long p_param1,
								  long p_param2)
{
	WinGDrawState* state = (WinGDrawState*) p_driverId;
	switch (p_message) {
	case DRV_LOAD:
	case DRV_FREE:
		return 1;
	case DRV_ENABLE:
	case DRV_DISABLE:
		return 1;
	case DRV_OPEN:
		if (p_param2 == 0) {
			return 1;
		}
		return (long) WinGDrawOpen((void*) p_param2);
	case DRV_CLOSE:
		return WinGDrawClose(state);
	case DRV_CONFIGURE:
		return 1;
	case DRV_QUERYCONFIGURE:
		return 0;
	case DRV_INSTALL:
	case DRV_REMOVE:
		return 1;
	case ICM_DRAW_BEGIN:
		return WinGDrawBegin(state, (void*) p_param1, p_param2);
	case ICM_DRAW_END:
		return WinGDrawEnd(state);
	case ICM_DRAW_QUERY:
		return WinGDrawQueryFormat(state, (void*) p_param1);
	case ICM_DRAW:
		return WinGDrawFrame(state, (void*) p_param1, p_param2);
	case ICM_DRAW_REALIZE:
		state->m_targetDC = (void*) p_param1;
		break;
	case ICM_DRAW_SUGGESTFORMAT:
		return WinGDrawSuggestFormat(state, (void*) p_param1, p_param2);
	case ICM_DRAW_CHANGEPALETTE:
		return WinGDrawChangePalette(state, (void*) p_param1);
	case ICM_GETSTATE:
	case ICM_SETSTATE:
		return 0;
	case ICM_GETINFO:
		return (long) WinGDrawGetInfo((void*) p_param1, (unsigned int) p_param2);
	case ICM_CONFIGURE:
	case ICM_ABOUT:
		return ICERR_UNSUPPORTED;
	}
	if (p_message < ICM_USER) {
		return DefDriverProc(p_driverId, (HDRVR) p_driverHandle, p_message, p_param1, p_param2);
	}
	return ICERR_UNSUPPORTED;
}

// FUNCTION: LEMBALL 0x00479190
WinGDrawState* __stdcall WinGDrawOpen(void* p_openInfo)
{
	void* mem;
	WinGDrawState* state;
	IcOpen* openInfo;

	openInfo = (IcOpen*) p_openInfo;
	if (openInfo->fccType != streamtypeVIDEO) {
		return NULL;
	}
	if (openInfo->dwFlags == ICMODE_COMPRESS) {
		return NULL;
	}
	if (openInfo->dwFlags == ICMODE_DECOMPRESS) {
		return NULL;
	}
	mem = GlobalAlloc(GHND, sizeof(WinGDrawState));
	state = (WinGDrawState*) GlobalLock(mem);
	if (state == NULL) {
		openInfo->dwError = ICERR_MEMORY;
		return NULL;
	}
	state->m_window = (CGWnd*) g_pAnimWnd;
	state->m_surface = ((CGWnd*) g_pAnimWnd)->m_gdi->m_renderTarget;
	openInfo->dwError = ICERR_OK;
	return state;
}

// FUNCTION: LEMBALL 0x00479210
int __stdcall WinGDrawClose(WinGDrawState* p_state)
{
	void* handle;

	if (p_state->m_memoryDC != NULL) {
		if (p_state->m_previousDibBitmap != NULL) {
			SelectObject((HDC) p_state->m_memoryDC, (HGDIOBJ) p_state->m_previousDibBitmap);
		}
		if (p_state->m_previousAuxBitmap != NULL) {
			SelectObject((HDC) p_state->m_memoryDC, (HGDIOBJ) p_state->m_previousAuxBitmap);
		}
		DeleteDC((HDC) p_state->m_memoryDC);
	}
	if (p_state->m_dibBitmap != NULL) {
		DeleteObject((HGDIOBJ) p_state->m_dibBitmap);
	}
	if (p_state->m_auxBitmap != NULL) {
		DeleteObject((HGDIOBJ) p_state->m_auxBitmap);
	}
	handle = GlobalHandle(p_state);
	GlobalUnlock(handle);
	handle = GlobalHandle(p_state);
	GlobalFree(handle);
	return 1;
}

// GLOBAL: LEMBALL 0x004a2da8
char g_szVisualSciencesWinGDrawHandler[] = "Visual Sciences WinG Draw Handler";

// GLOBAL: LEMBALL 0x004a2dd0
char g_szVsWinGAnim[] = "VS - WinG Anim";

// FUNCTION: LEMBALL 0x004792a0
unsigned int __stdcall WinGDrawGetInfo(void* p_info, unsigned int p_size)
{
	IcInfo* info;
	WCHAR* description;

	info = (IcInfo*) p_info;
	if (info == NULL) {
		return sizeof(IcInfo);
	}
	if (p_size < sizeof(IcInfo)) {
		return 0;
	}
	info->fccType = ICTYPE_VIDEO;
	info->dwSize = sizeof(IcInfo);
	info->fccHandler = mmioFOURCC('V', 'S', 'A', 'N');
	info->dwFlags = VIDCF_DRAW;
	info->dwVersion = WING_DRAW_VERSION;
	info->dwVersionICM = ICVERSION;
	description = info->szDescription;
	MultiByteToWideChar(CP_ACP, 0, g_szVisualSciencesWinGDrawHandler, -1, description, WING_DRAW_TEXT_CAPACITY);
	MultiByteToWideChar(CP_ACP, 0, g_szVsWinGAnim, -1, description, WING_DRAW_TEXT_CAPACITY);
	return sizeof(IcInfo);
}

// FUNCTION: LEMBALL 0x00479330
int __stdcall WinGDrawQueryFormat(WinGDrawState* p_state, void* p_format)
{
	BITMAPINFOHEADER* format;

	format = (BITMAPINFOHEADER*) p_format;
	if (format == NULL) {
		return ICERR_BADFORMAT;
	}
	if (format->biCompression != BI_RGB) {
		return ICERR_BADFORMAT;
	}
	return (unsigned short) (format->biBitCount - 8) < 1 ? ICERR_OK : ICERR_BADFORMAT;
}

// FUNCTION: LEMBALL 0x00479370
int __stdcall WinGDrawSuggestFormat(WinGDrawState* p_state, void* p_request, long p_param2)
{
	IcDrawSuggest* request;
	BITMAPINFOHEADER* source;
	BITMAPINFOHEADER* dest;

	request = (IcDrawSuggest*) p_request;
	dest = request->lpbiSuggest;
	if (dest == NULL) {
		return sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	}
	source = request->lpbiIn;
	dest->biClrUsed = source->biClrUsed;
	dest->biClrImportant = source->biClrImportant;
	dest->biSize = source->biSize;
	dest->biWidth = source->biWidth;
	dest->biHeight = source->biHeight;
	dest->biSizeImage = source->biWidth * source->biHeight;
	dest->biPlanes = 1;
	dest->biBitCount = 8;
	dest->biCompression = BI_RGB;
	dest->biSizeImage = 0;
	return (int) (source->biClrUsed * sizeof(RGBQUAD) + sizeof(BITMAPINFOHEADER));
}

// FUNCTION: LEMBALL 0x004793e0
int __stdcall WinGDrawBegin(WinGDrawState* p_state, void* p_request, long p_param2)
{
	int result;
	int copyBytes;
	IcDrawBegin* request;
	BITMAPINFO* format;
	(void) p_param2;

	request = (IcDrawBegin*) p_request;
	result = WinGDrawQueryFormat(p_state, request->lpbi);
	if (result == 0 && (request->dwFlags & ICDRAW_QUERY) == 0) {
		p_state->m_destinationX = request->xDst;
		p_state->m_destinationY = request->yDst;
		p_state->m_destinationWidth = request->dxDst;
		p_state->m_destinationHeight = request->dyDst;
		p_state->m_sourceX = request->xSrc;
		p_state->m_sourceY = request->ySrc;
		p_state->m_sourceWidth = request->dxSrc;
		p_state->m_sourceHeight = request->dySrc;
		SetStretchBltMode((HDC) p_state->m_targetDC, COLORONCOLOR);
		format = (BITMAPINFO*) request->lpbi;
		copyBytes = (format->bmiHeader.biClrUsed - 1) * sizeof(format->bmiColors[0]);
		if (0 < copyBytes) {
			memcpy(&g_dwWinGDrawColourTable[1], &format->bmiColors[1], (unsigned int) copyBytes);
			p_state->m_surface->SetDefaultCtable();
		}
		result = 0;
	}
	return result;
}

// FUNCTION: LEMBALL 0x00479480
int __stdcall WinGDrawFrame(WinGDrawState* p_state, void* p_request, long p_param2)
{
	IcDraw* request;
	CPVWnd* window;
	(void) p_param2;

	request = (IcDraw*) p_request;
	window = (CPVWnd*) p_state->m_window;
	if (window->m_lifecycleRefs == 1) {
		p_state->m_surface->CopyDibBits(request->lpFormat, (unsigned char*) request->lpData);
		p_state->m_window->CGWnd::Refresh(NULL);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004794c0
int __stdcall WinGDrawChangePalette(WinGDrawState* p_state, void* p_request)
{
	return 0;
}

// FUNCTION: LEMBALL 0x004794d0
int __stdcall WinGDrawEnd(WinGDrawState* p_state)
{
	return 0;
}

// GLOBAL: LEMBALL 0x004a9bf8
unsigned int g_dwWinGDrawColourTable[256];
