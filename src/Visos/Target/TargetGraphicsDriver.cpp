#include "TargetGraphicsDriver.h"

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

// FUNCTION: LEMBALL 0x00456790
TargetGraphicsDriver::~TargetGraphicsDriver()
{
	if (m_driverModule != 0) {
		FreeLibrary((HMODULE) m_driverModule);
		m_driverModule = 0;
	}
	if (m_palette != 0) {
		DeleteObject((HGDIOBJ) m_palette);
	}
}

// FUNCTION: LEMBALL 0x004567c0
bool TargetGraphicsDriver::CreatePalette(void* p_paletteDescription)
{
	HPALETTE palette;

	if (m_palette != 0) {
		DeleteObject((HGDIOBJ) m_palette);
	}
	palette = ::CreatePalette((LOGPALETTE*) p_paletteDescription);
	m_palette = palette;
	return palette != 0;
}

// FUNCTION: LEMBALL 0x004567f0
bool TargetGraphicsDriver::RealizePalette(TargetDrawingContext* p_drawingContext)
{
	if (m_palette != 0) {
		SelectPalette((HDC) p_drawingContext->m_hDC, (HPALETTE) m_palette, 0);
		::RealizePalette((HDC) p_drawingContext->m_hDC);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00456970
bool TargetGraphicsDriver::BlitWrappedBitmap(TargetDrawingContext* p_destination,
											 VsRect* p_destinationRect,
											 TargetDrawingContext* p_source,
											 VsRect* p_sourceRect,
											 PvGdiBitmap* p_bitmap)
{
	short scale;
	VsRect* rect0;
	VsRect* rect1;
	unsigned char copied;
	VsRect destRect;

	copied = 0;
	rect0 = 0;
	rect1 = 0;
	m_currentBitmap = p_bitmap;
	scale = p_destinationRect->m_width / p_sourceRect->m_width;
	p_bitmap->GetRects(*p_sourceRect, rect0, rect1);
	if (rect0 != 0) {
		destRect.m_width = (short) (rect0->m_width * scale);
		destRect.m_height = (short) (rect0->m_height * scale);
		destRect.m_x = p_destinationRect->m_x;
		destRect.m_y = p_destinationRect->m_y;
		StretchBltContexts(p_destination, &destRect, p_source, rect0);
		copied = 1;
	}
	if (rect1 != 0) {
		destRect.m_width = (short) (rect1->m_width * scale);
		destRect.m_height = (short) (rect1->m_height * scale);
		destRect.m_x = p_destinationRect->m_x;
		destRect.m_y = (short) (rect0->m_height * scale + p_destinationRect->m_y);
		StretchBltContexts(p_destination, &destRect, p_source, rect1);
		copied = 1;
	}
	return copied != 0;
}

// FUNCTION: LEMBALL 0x00458200
bool TargetGraphicsDriver::HasPalette()
{
	return m_palette != 0;
}

// FUNCTION: LEMBALL 0x00478fb0
long __stdcall TargetWinGDrawCodec_DriverProc(unsigned int p_driverId,
											  void* p_driverHandle,
											  unsigned int p_message,
											  long p_param1,
											  long p_param2)
{
	TargetWinGDrawCodecState* state;

	state = (TargetWinGDrawCodecState*) p_driverId;
	if (p_message <= 0x400f) {
		if (p_message == 0x400f) {
			return TargetWinGDrawCodec_Begin(state, (void*) p_param1, p_param2);
		}
		switch (p_message) {
		case 1:
		case 2:
		case 5:
		case 6:
		case 7:
		case 9:
		case 10:
			return 1;
		case 3:
			if (p_param2 != 0) {
				return (long) TargetWinGDrawCodec_Open((void*) p_param2);
			}
			return 1;
		case 4:
			return TargetWinGDrawCodec_Close(state);
		case 8:
			return 0;
		}
	}
	else if (p_message <= 0x401f) {
		if (p_message == 0x401f) {
			return TargetWinGDrawCodec_QueryFormat(state, (void*) p_param1);
		}
		if (p_message == 0x4015) {
			return TargetWinGDrawCodec_End(state);
		}
	}
	else if (p_message <= 0x4024) {
		if (p_message == 0x4024) {
			state->m_targetDC = (void*) p_param1;
		}
		else if (p_message == 0x4021) {
			return TargetWinGDrawCodec_Draw(state, (void*) p_param1, p_param2);
		}
	}
	else if (p_message <= 0x5001) {
		if (p_message > 0x4fff) {
			return 0;
		}
		if (p_message == 0x4032) {
			return TargetWinGDrawCodec_SuggestFormat(state, (void*) p_param1, p_param2);
		}
		if (p_message == 0x4033) {
			return TargetWinGDrawCodec_ChangePalette(state, (void*) p_param1);
		}
	}
	else {
		if (p_message == 0x5002) {
			return (long) TargetWinGDrawCodec_GetInfo((void*) p_param1, (unsigned int) p_param2);
		}
		if (p_message > 0x5009 && p_message < 0x500c) {
			return -1;
		}
	}
	if (p_message > 0x3fff) {
		return -1;
	}
	return DefDriverProc(p_driverId, p_driverHandle, p_message, p_param1, p_param2);
}

// FUNCTION: LEMBALL 0x00479190
TargetWinGDrawCodecState* __stdcall TargetWinGDrawCodec_Open(void* p_openInfo)
{
	void* mem;
	TargetWinGDrawCodecState* state;
	IcOpen* openInfo;

	openInfo = (IcOpen*) p_openInfo;
	if (openInfo->fccType != 0x73646976) {
		return 0;
	}
	if (openInfo->dwFlags == 1) {
		return 0;
	}
	if (openInfo->dwFlags == 2) {
		return 0;
	}
	mem = GlobalAlloc(0x42, 0x474);
	state = (TargetWinGDrawCodecState*) GlobalLock(mem);
	if (state == 0) {
		openInfo->dwError = 0xfffffffd;
		return 0;
	}
	state->m_window = (GWnd*) g_pAnimWnd;
	state->m_surface = ((GWnd*) g_pAnimWnd)->m_gdi->m_renderTarget;
	openInfo->dwError = 0;
	return state;
}

// FUNCTION: LEMBALL 0x00479210
int __stdcall TargetWinGDrawCodec_Close(TargetWinGDrawCodecState* p_state)
{
	void* handle;

	if (p_state->m_memoryDC != 0) {
		if (p_state->m_previousDibBitmap != 0) {
			SelectObject((HDC) p_state->m_memoryDC, (HGDIOBJ) p_state->m_previousDibBitmap);
		}
		if (p_state->m_previousAuxBitmap != 0) {
			SelectObject((HDC) p_state->m_memoryDC, (HGDIOBJ) p_state->m_previousAuxBitmap);
		}
		DeleteDC((HDC) p_state->m_memoryDC);
	}
	if (p_state->m_dibBitmap != 0) {
		DeleteObject((HGDIOBJ) p_state->m_dibBitmap);
	}
	if (p_state->m_auxBitmap != 0) {
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
unsigned int __stdcall TargetWinGDrawCodec_GetInfo(void* p_info, unsigned int p_size)
{
	IcInfo* info;
	WCHAR* description;

	info = (IcInfo*) p_info;
	if (info == 0) {
		return 0x238;
	}
	if (p_size < 0x238) {
		return 0;
	}
	info->dwSize = 0x238;
	info->fccType = 0x63646976;
	info->fccHandler = 0x4e415356;
	info->dwFlags = 0x10;
	info->dwVersion = 0x10000;
	info->dwVersionICM = 0x104;
	description = info->szDescription;
	MultiByteToWideChar(0, 0, g_szVisualSciencesWinGDrawHandler, -1, description, 0x100);
	MultiByteToWideChar(0, 0, g_szVsWinGAnim, -1, description, 0x100);
	return 0x238;
}

// FUNCTION: LEMBALL 0x00479330
int __stdcall TargetWinGDrawCodec_QueryFormat(TargetWinGDrawCodecState* p_state, void* p_format)
{
	BITMAPINFOHEADER* format;

	format = (BITMAPINFOHEADER*) p_format;
	if (format == 0) {
		return -2;
	}
	if (format->biCompression != 0) {
		return -2;
	}
	return (unsigned short) (format->biBitCount - 8) < 1 ? 0 : -2;
}

// FUNCTION: LEMBALL 0x00479370
int __stdcall TargetWinGDrawCodec_SuggestFormat(TargetWinGDrawCodecState* p_state, void* p_request, long p_param2)
{
	IcDrawSuggest* request;
	BITMAPINFOHEADER* source;
	BITMAPINFOHEADER* dest;

	request = (IcDrawSuggest*) p_request;
	dest = request->lpbiSuggest;
	if (dest == 0) {
		return 0x428;
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
	dest->biCompression = 0;
	dest->biSizeImage = 0;
	return (int) (source->biClrUsed * 4 + 0x28);
}

// FUNCTION: LEMBALL 0x004793e0
int __stdcall TargetWinGDrawCodec_Begin(TargetWinGDrawCodecState* p_state, void* p_request, long p_param2)
{
	int result;
	int copyBytes;
	IcDrawBegin* request;
	BITMAPINFO* format;
	(void) p_param2;

	request = (IcDrawBegin*) p_request;
	result = TargetWinGDrawCodec_QueryFormat(p_state, request->lpbi);
	if (result == 0 && (request->dwFlags & 1) == 0) {
		p_state->m_destinationX = request->xDst;
		p_state->m_destinationY = request->yDst;
		p_state->m_destinationWidth = request->dxDst;
		p_state->m_destinationHeight = request->dyDst;
		p_state->m_sourceX = request->xSrc;
		p_state->m_sourceY = request->ySrc;
		p_state->m_sourceWidth = request->dxSrc;
		p_state->m_sourceHeight = request->dySrc;
		SetStretchBltMode((HDC) p_state->m_targetDC, 3);
		format = (BITMAPINFO*) request->lpbi;
		copyBytes = format->bmiHeader.biClrUsed;
		copyBytes = copyBytes * 4 - 4;
		if (0 < copyBytes) {
			memcpy(&g_dwWinGDrawColourTable[1], &format->bmiColors[1], (unsigned int) copyBytes);
			p_state->m_surface->SetDefaultCtable();
		}
		result = 0;
	}
	return result;
}

// FUNCTION: LEMBALL 0x00479480
int __stdcall TargetWinGDrawCodec_Draw(TargetWinGDrawCodecState* p_state, void* p_request, long p_param2)
{
	IcDraw* request;
	PvWnd* window;
	(void) p_param2;

	request = (IcDraw*) p_request;
	window = (PvWnd*) p_state->m_window;
	if (window->m_lifecycleRefs == 1) {
		p_state->m_surface->CopyDIBBits(request->lpFormat, (unsigned char*) request->lpData);
		p_state->m_window->GWnd::Refresh(0);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004794c0
int __stdcall TargetWinGDrawCodec_ChangePalette(TargetWinGDrawCodecState* p_state, void* p_request)
{
	return 0;
}

// FUNCTION: LEMBALL 0x004794d0
int __stdcall TargetWinGDrawCodec_End(TargetWinGDrawCodecState* p_state)
{
	return 0;
}

// GLOBAL: LEMBALL 0x004a076c
TargetGraphicsDriver* g_pTargetGraphicsDriver = 0;

// GLOBAL: LEMBALL 0x004a0770
unsigned int g_dwFullScreenGdi = 0;

// GLOBAL: LEMBALL 0x004a0768
TargetGraphicsSystemState* g_pTargetGraphicsSystem = 0;

// GLOBAL: LEMBALL 0x004a9bf8
unsigned int g_dwWinGDrawColourTable[256];
