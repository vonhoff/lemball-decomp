#include "TargetGraphicsDriver.h"

#include "../Foundation/VsInit.h"
#include "../Foundation/VsOStream.h"
#include "../Graphics/GWnd.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/PvGdiBitmap.h"
#include "../Graphics/PvWnd.h"
#include "../Graphics/VsGdi.h"
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

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);
extern "C" __declspec(dllimport) long __stdcall DefDriverProc(unsigned int p_driverId,
															  void* p_driverHandle,
															  unsigned int p_message,
															  long p_param1,
															  long p_param2);

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

// STUB: LEMBALL 0x00456970
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

// STUB: LEMBALL 0x00456a90
TargetGraphicsDriver::TargetGraphicsDriver()
{
	m_driverModule = 0;
	m_palette = 0;
	m_ready = 1;
	m_window = 0;
	m_screenSize.m_width = (short) GetSystemMetrics(0);
	m_screenSize.m_height = (short) GetSystemMetrics(1);
	m_currentBitmap = 0;
}

TargetGraphicsDriver::~TargetGraphicsDriver()
{
}

// STUB: LEMBALL 0x00456ae0
TargetDrawingContext* TargetGraphicsDriver::CreateDrawingContext()
{
	HDC hdc;
	void* storage;
	TargetDrawingContext* context;

	hdc = CreateCompatibleDC(0);
	if (hdc == 0) {
		return 0;
	}
	storage = operator new(0xc);
	if (storage == 0) {
		return 0;
	}
	context = new (storage) TargetDrawingContext();
	context->m_hDC = hdc;
	context->m_hBitmap = 0;
	return context;
}

// STUB: LEMBALL 0x00456b20
bool TargetGraphicsDriver::DestroyDrawingContext(TargetDrawingContext* p_drawingContext)
{
	int deleted;

	if (p_drawingContext == 0) {
		return 0;
	}
	deleted = 0;
	if (p_drawingContext->m_hDC != 0) {
		deleted = DeleteDC((HDC) p_drawingContext->m_hDC);
	}
	delete p_drawingContext;
	return deleted != 0;
}

// STUB: LEMBALL 0x00456b50
bool TargetGraphicsDriver::InitializeBitmapInfo(void* p_bitmapInfo)
{
	BITMAPINFO* info;

	info = (BITMAPINFO*) p_bitmapInfo;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biSize = 0x28;
	info->bmiHeader.biCompression = 0;
	info->bmiHeader.biSizeImage = 0;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	info->bmiHeader.biClrUsed = 0;
	info->bmiHeader.biHeight = -1;
	info->bmiHeader.biBitCount = 8;
	info->bmiHeader.biClrImportant = 0;
	return 1;
}

// STUB: LEMBALL 0x00456b90
TargetDibContext* TargetGraphicsDriver::CreateDIBContext(TargetDrawingContext* p_drawingContext, void* p_bitmapInfo)
{
	HBITMAP bitmap;
	void* bits;
	void* storage;
	TargetDibContext* context;
	BITMAPINFO* info;

	info = (BITMAPINFO*) p_bitmapInfo;
	bits = 0;
	bitmap = CreateDIBSection((HDC) p_drawingContext->m_hDC, info, 0, &bits, 0, 0);
	if (bitmap == 0) {
		return 0;
	}
	storage = operator new(0x14);
	if (storage == 0) {
		DeleteObject(bitmap);
		return 0;
	}
	context = new (storage) TargetDibContext();
	context->m_bits = 0;
	context->m_width = 0;
	context->m_hBitmap = bitmap;
	context->m_width = info->bmiHeader.biWidth;
	context->m_bits = (unsigned char*) bits;
	p_drawingContext->m_hBitmap = context;
	return context;
}

// STUB: LEMBALL 0x00456c10
bool TargetGraphicsDriver::DestroyDIBContext(TargetDibContext* p_dibContext)
{
	int deleted;

	if (p_dibContext == 0) {
		return 1;
	}
	deleted = DeleteObject((HGDIOBJ) p_dibContext->m_hBitmap);
	if (p_dibContext != 0) {
		p_dibContext->~TargetDibContext();
		operator delete(p_dibContext);
	}
	return deleted != 0;
}

// FUNCTION: LEMBALL 0x00456c50
void TargetGraphicsDriver::UpdateDIBColourTable(TargetDrawingContext* p_drawingContext,
												unsigned int p_startIndex,
												unsigned int p_entryCount,
												void* p_colours)
{
	SetDIBColorTable((HDC) p_drawingContext->m_hDC, p_startIndex, p_entryCount, (RGBQUAD*) p_colours);
}

// FUNCTION: LEMBALL 0x00456c70
void TargetGraphicsDriver::BitBltContexts(TargetDrawingContext* p_destination,
										  VsRect* p_destinationRect,
										  TargetDrawingContext* p_source,
										  VsPoint* p_sourcePosition)
{
	BitBlt((HDC) p_destination->m_hDC,
		   (int) p_destinationRect->m_x,
		   (int) p_destinationRect->m_y,
		   (int) p_destinationRect->m_width,
		   (int) p_destinationRect->m_height,
		   (HDC) p_source->m_hDC,
		   (int) p_sourcePosition->m_x,
		   (int) p_sourcePosition->m_y,
		   0xcc0020);
}

// FUNCTION: LEMBALL 0x00456cc0
void TargetGraphicsDriver::StretchBltContexts(TargetDrawingContext* p_destination,
											  VsRect* p_destinationRect,
											  TargetDrawingContext* p_source,
											  VsRect* p_sourceRect)
{
	StretchBlt((HDC) p_destination->m_hDC,
			   (int) p_destinationRect->m_x,
			   (int) p_destinationRect->m_y,
			   (int) p_destinationRect->m_width,
			   (int) p_destinationRect->m_height,
			   (HDC) p_source->m_hDC,
			   (int) p_sourceRect->m_x,
			   (int) p_sourceRect->m_y,
			   (int) p_sourceRect->m_width,
			   (int) p_sourceRect->m_height,
			   0xcc0020);
}

// FUNCTION: LEMBALL 0x00456d10
TargetDibContext* TargetGraphicsDriver::SelectDIBContext(TargetDrawingContext* p_drawingContext,
														 TargetDibContext* p_dibContext)
{
	HGDIOBJ prior;

	prior = SelectObject((HDC) p_drawingContext->m_hDC, (HGDIOBJ) p_dibContext->m_hBitmap);
	if (prior == 0) {
		return 0;
	}
	p_dibContext->m_previousBitmap = prior;
	return p_dibContext;
}

// STUB: LEMBALL 0x00456d40
TargetDibContext* TargetGraphicsDriver::RestoreDIBContext(TargetDrawingContext* p_drawingContext,
														  TargetDibContext* p_dibContext)
{
	HGDIOBJ prior;

	prior = SelectObject((HDC) p_drawingContext->m_hDC, (HGDIOBJ) p_dibContext->m_previousBitmap);
	if (prior == 0) {
		return 0;
	}
	return p_dibContext;
}

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
		g_pTargetGraphicsDriver = new (storage) TargetGraphicsDriver();
	}

	if (g_pTargetGraphicsDriver == 0 || g_pTargetGraphicsDriver->m_ready == 0) {
		*g_pErrorOutput << "No valid driver available\n";
		return 0;
	}

	m_driverMode = 1;
	return 1;
}

// FUNCTION: LEMBALL 0x00458250
void TargetDrawingContext::SetDC(void* p_hDC)
{
	m_hDC = p_hDC;
}

// STUB: LEMBALL 0x00458260
TargetDrawingContext::~TargetDrawingContext()
{
}

TargetDibContext::~TargetDibContext()
{
}

// FUNCTION: LEMBALL 0x004582b0
bool TargetDibContext::Lock()
{
	return 1;
}

// FUNCTION: LEMBALL 0x004582c0
bool TargetDibContext::Unlock()
{
	return 1;
}

// FUNCTION: LEMBALL 0x004582d0
unsigned char* TargetDibContext::GetBits()
{
	return m_bits;
}

// FUNCTION: LEMBALL 0x004582e0
int TargetDibContext::GetStride()
{
	return m_width;
}

// STUB: LEMBALL 0x00478fb0
long __stdcall TargetWinGDrawCodec_DriverProc(unsigned int p_driverId,
											  void* p_driverHandle,
											  unsigned int p_message,
											  long p_param1,
											  long p_param2)
{
	if (p_message < 0x4010) {
		if (p_message == 0x400f) {
			return TargetWinGDrawCodec_Begin((TargetWinGDrawCodecState*) p_driverId, (void*) p_param1);
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
			return TargetWinGDrawCodec_Close((TargetWinGDrawCodecState*) p_driverId);
		case 8:
			return 0;
		}
	}
	else if (p_message < 0x4020) {
		if (p_message == 0x401f) {
			return TargetWinGDrawCodec_QueryFormat((TargetWinGDrawCodecState*) p_driverId, (void*) p_param1);
		}
		if (p_message == 0x4015) {
			return TargetWinGDrawCodec_End((TargetWinGDrawCodecState*) p_driverId);
		}
	}
	else if (p_message < 0x4025) {
		if (p_message == 0x4024) {
			((TargetWinGDrawCodecState*) p_driverId)->m_targetDC = (void*) p_param1;
		}
		else if (p_message == 0x4021) {
			return TargetWinGDrawCodec_Draw((TargetWinGDrawCodecState*) p_driverId, (void*) p_param1);
		}
	}
	else if (p_message < 0x5002) {
		if (p_message > 0x4fff) {
			return 0;
		}
		if (p_message == 0x4032) {
			return TargetWinGDrawCodec_SuggestFormat((TargetWinGDrawCodecState*) p_driverId, (void*) p_param1);
		}
		if (p_message == 0x4033) {
			return TargetWinGDrawCodec_ChangePalette((TargetWinGDrawCodecState*) p_driverId, (void*) p_param1);
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

// STUB: LEMBALL 0x00479190
TargetWinGDrawCodecState* TargetWinGDrawCodec_Open(void* p_openInfo)
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
	state->m_surface = state->m_window->m_gdi->m_renderTarget;
	openInfo->dwError = 0;
	return state;
}

// STUB: LEMBALL 0x00479210
int TargetWinGDrawCodec_Close(TargetWinGDrawCodecState* p_state)
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

// STUB: LEMBALL 0x004792a0
unsigned int TargetWinGDrawCodec_GetInfo(void* p_info, unsigned int p_size)
{
	IcInfo* info;

	info = (IcInfo*) p_info;
	if (info == 0) {
		return 0x238;
	}
	if (p_size < 0x238) {
		return 0;
	}
	info->fccType = 0x63646976;
	info->fccHandler = 0x4e415356;
	info->dwSize = 0x238;
	info->dwFlags = 0x10;
	info->dwVersion = 0x10000;
	info->dwVersionICM = 0x104;
	MultiByteToWideChar(0, 0, g_szVisualSciencesWinGDrawHandler, -1, info->szDescription, 0x100);
	MultiByteToWideChar(0, 0, g_szVsWinGAnim, -1, info->szDescription, 0x100);
	return 0x238;
}

// STUB: LEMBALL 0x00479330
int TargetWinGDrawCodec_QueryFormat(TargetWinGDrawCodecState* p_state, void* p_format)
{
	BITMAPINFOHEADER* format;

	format = (BITMAPINFOHEADER*) p_format;
	if (format == 0) {
		return -2;
	}
	if (format->biCompression != 0) {
		return -2;
	}
	return (format->biBitCount == 8) - 1 & 0xfffffffe;
}

// STUB: LEMBALL 0x00479370
int TargetWinGDrawCodec_SuggestFormat(TargetWinGDrawCodecState* p_state, void* p_request)
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
int TargetWinGDrawCodec_Begin(TargetWinGDrawCodecState* p_state, void* p_request)
{
	int result;
	int copyBytes;
	IcDrawBegin* request;
	BITMAPINFO* format;

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
		if (copyBytes == 0) {
			copyBytes = 0x100;
		}
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
int TargetWinGDrawCodec_Draw(TargetWinGDrawCodecState* p_state, void* p_request)
{
	IcDraw* request;
	PvWnd* window;

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
