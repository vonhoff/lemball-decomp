
#include "CGdiDriver.h"

#include "CGdiContext.h"
#include "CGdiDibContext.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Target/Graphics/CDibContext.h"
#include "Visos/Target/Graphics/CDrawingContext.h"

#include <windows.h>

// FUNCTION: LEMBALL 0x00456a90
CGdiDriver::CGdiDriver()
{
	m_ready = 1;
	m_screenSize.m_width = (short) GetSystemMetrics(0);
	m_screenSize.m_height = (short) GetSystemMetrics(1);
}

// FUNCTION: LEMBALL 0x00456ae0
CDrawingContext* CGdiDriver::CreateDrawingContext()
{
	HDC hdc;
	void* storage;
	CDrawingContext* context;

	hdc = CreateCompatibleDC(0);
	if (hdc != 0) {
		storage = operator new(0xc);
		if (storage != 0) {
			context = new (storage) CGdiContext(hdc);
			return context;
		}
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00456b20
int CGdiDriver::DestroyDrawingContext(CDrawingContext* p_drawingContext)
{
	int deleted;

	if (p_drawingContext == 0) {
		return 0;
	}
	deleted = DeleteDC((HDC) ((CGdiContext*) p_drawingContext)->m_hDC);
	delete p_drawingContext;
	return deleted;
}

// FUNCTION: LEMBALL 0x00456b50
bool CGdiDriver::InitializeBitmapInfo(void* p_bitmapInfo)
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
CDibContext* CGdiDriver::CreateDibContext(CDrawingContext* p_drawingContext, void* p_bitmapInfo)
{
	HBITMAP bitmap;
	void* bits;
	CDibContext* context;
	BITMAPINFO* info;

	info = (BITMAPINFO*) p_bitmapInfo;
	bitmap = CreateDIBSection((HDC) ((CGdiContext*) p_drawingContext)->m_hDC, info, 0, &bits, 0, 0);
	if (bitmap == 0) {
		return 0;
	}
	context = new CGdiDibContext(bitmap);
	context->m_width = info->bmiHeader.biWidth;
	context->m_bits = (unsigned char*) bits;
	((CGdiContext*) p_drawingContext)->m_hBitmap = context;
	return context;
}

// FUNCTION: LEMBALL 0x00456c10
int CGdiDriver::DestroyDibContext(CDibContext* p_dibContext)
{
	int deleted;

	if (p_dibContext == 0) {
		return 1;
	}
	deleted = DeleteObject((HGDIOBJ) ((CGdiDibContext*) p_dibContext)->m_hBitmap);
	delete p_dibContext;
	return deleted;
}

// FUNCTION: LEMBALL 0x00456c50
unsigned int CGdiDriver::UpdateDibColourTable(CDrawingContext* p_drawingContext,
											  unsigned int p_startIndex,
											  unsigned int p_entryCount,
											  void* p_colours)
{
	return SetDIBColorTable((HDC) ((CGdiContext*) p_drawingContext)->m_hDC,
							p_startIndex,
							p_entryCount,
							(RGBQUAD*) p_colours);
}

// FUNCTION: LEMBALL 0x00456c70
int CGdiDriver::BitBltContexts(CDrawingContext* p_destination,
							   CVsRect* p_destinationRect,
							   CDrawingContext* p_source,
							   CVsPoint* p_sourcePosition)
{
	return BitBlt((HDC) ((CGdiContext*) p_destination)->m_hDC,
				  (int) p_destinationRect->m_x,
				  (int) p_destinationRect->m_y,
				  (int) p_destinationRect->m_width,
				  (int) p_destinationRect->m_height,
				  (HDC) ((CGdiContext*) p_source)->m_hDC,
				  (int) p_sourcePosition->m_x,
				  (int) p_sourcePosition->m_y,
				  0xcc0020);
}

// FUNCTION: LEMBALL 0x00456cc0
int CGdiDriver::StretchBltContexts(CDrawingContext* p_destination,
								   CVsRect* p_destinationRect,
								   CDrawingContext* p_source,
								   CVsRect* p_sourceRect)
{
	return StretchBlt((HDC) ((CGdiContext*) p_destination)->m_hDC,
					  (int) p_destinationRect->m_x,
					  (int) p_destinationRect->m_y,
					  (int) p_destinationRect->m_width,
					  (int) p_destinationRect->m_height,
					  (HDC) ((CGdiContext*) p_source)->m_hDC,
					  (int) p_sourceRect->m_x,
					  (int) p_sourceRect->m_y,
					  (int) p_sourceRect->m_width,
					  (int) p_sourceRect->m_height,
					  0xcc0020);
}

// FUNCTION: LEMBALL 0x00456d10
CDibContext* CGdiDriver::SelectDibContext(CDrawingContext* p_drawingContext, CDibContext* p_dibContext)
{
	HGDIOBJ prior;

	prior = SelectObject((HDC) ((CGdiContext*) p_drawingContext)->m_hDC,
						 (HGDIOBJ) ((CGdiDibContext*) p_dibContext)->m_hBitmap);
	if (prior == 0) {
		return 0;
	}
	((CGdiDibContext*) p_dibContext)->m_previousBitmap = prior;
	return p_dibContext;
}

// FUNCTION: LEMBALL 0x00456d40
CDibContext* CGdiDriver::RestoreDibContext(CDrawingContext* p_drawingContext, CDibContext* p_dibContext)
{
	HGDIOBJ prior;

	prior = SelectObject((HDC) ((CGdiContext*) p_drawingContext)->m_hDC,
						 (HGDIOBJ) ((CGdiDibContext*) p_dibContext)->m_previousBitmap);
	if (prior == 0) {
		return 0;
	}
	return p_dibContext;
}
