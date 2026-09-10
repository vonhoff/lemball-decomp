#include "TargetGDIDriver.h"

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

// FUNCTION: LEMBALL 0x00456a90
TargetGDIDriver::TargetGDIDriver()
{
	m_ready = 1;
	m_screenSize.m_width = (short) GetSystemMetrics(0);
	m_screenSize.m_height = (short) GetSystemMetrics(1);
}

// FUNCTION: LEMBALL 0x00456ae0
TargetDrawingContext* TargetGDIDriver::CreateDrawingContext()
{
	HDC hdc;
	void* storage;
	TargetDrawingContext* context;

	hdc = CreateCompatibleDC(0);
	if (hdc != 0) {
		storage = operator new(0xc);
		if (storage != 0) {
			context = new (storage) TargetDrawingContext(hdc);
			return context;
		}
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00456b20
int TargetGDIDriver::DestroyDrawingContext(TargetDrawingContext* p_drawingContext)
{
	int deleted;

	if (p_drawingContext == 0) {
		return 0;
	}
	deleted = DeleteDC((HDC) p_drawingContext->m_hDC);
	delete p_drawingContext;
	return deleted;
}

// FUNCTION: LEMBALL 0x00456b50
bool TargetGDIDriver::InitializeBitmapInfo(void* p_bitmapInfo)
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

// FUNCTION: LEMBALL 0x00456b90
TargetDibContext* TargetGDIDriver::CreateDIBContext(TargetDrawingContext* p_drawingContext, void* p_bitmapInfo)
{
	HBITMAP bitmap;
	void* bits;
	TargetDibContext* context;
	BITMAPINFO* info;

	info = (BITMAPINFO*) p_bitmapInfo;
	bitmap = CreateDIBSection((HDC) p_drawingContext->m_hDC, info, 0, &bits, 0, 0);
	if (bitmap == 0) {
		return 0;
	}
	context = new TargetDibContext();
	context->m_hBitmap = bitmap;
	context->m_width = info->bmiHeader.biWidth;
	context->m_bits = (unsigned char*) bits;
	p_drawingContext->m_hBitmap = context;
	return context;
}

// FUNCTION: LEMBALL 0x00456c10
int TargetGDIDriver::DestroyDIBContext(TargetDibContext* p_dibContext)
{
	int deleted;

	if (p_dibContext == 0) {
		return 1;
	}
	deleted = DeleteObject((HGDIOBJ) p_dibContext->m_hBitmap);
	delete p_dibContext;
	return deleted;
}

// FUNCTION: LEMBALL 0x00456c50
void TargetGDIDriver::UpdateDIBColourTable(TargetDrawingContext* p_drawingContext,
										   unsigned int p_startIndex,
										   unsigned int p_entryCount,
										   void* p_colours)
{
	SetDIBColorTable((HDC) p_drawingContext->m_hDC, p_startIndex, p_entryCount, (RGBQUAD*) p_colours);
}

// FUNCTION: LEMBALL 0x00456c70
void TargetGDIDriver::BitBltContexts(TargetDrawingContext* p_destination,
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
void TargetGDIDriver::StretchBltContexts(TargetDrawingContext* p_destination,
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
TargetDibContext* TargetGDIDriver::SelectDIBContext(TargetDrawingContext* p_drawingContext,
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

// FUNCTION: LEMBALL 0x00456d40
TargetDibContext* TargetGDIDriver::RestoreDIBContext(TargetDrawingContext* p_drawingContext,
													 TargetDibContext* p_dibContext)
{
	HGDIOBJ prior;

	prior = SelectObject((HDC) p_drawingContext->m_hDC, (HGDIOBJ) p_dibContext->m_previousBitmap);
	if (prior == 0) {
		return 0;
	}
	return p_dibContext;
}
