#include "CGraphicsDriver.h"

#include "../Graphics/CPvGdiBitmap.h"
#include "CGdiContext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x00456790
CGraphicsDriver::~CGraphicsDriver()
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
bool CGraphicsDriver::CreatePalette(void* p_paletteDescription)
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
bool CGraphicsDriver::RealizePalette(CDrawingContext* p_drawingContext)
{
	if (m_palette != 0) {
		SelectPalette((HDC) ((CGdiContext*) p_drawingContext)->m_hDC, (HPALETTE) m_palette, 0);
		::RealizePalette((HDC) ((CGdiContext*) p_drawingContext)->m_hDC);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00456970
bool CGraphicsDriver::BlitWrappedBitmap(CDrawingContext* p_destination,
										CVsRect* p_destinationRect,
										CDrawingContext* p_source,
										CVsRect* p_sourceRect,
										CPvGdiBitmap* p_bitmap)
{
	int scale;
	CVsRect* rect0;
	CVsRect* rect1;
	bool copied;

	copied = 0;
	rect0 = 0;
	rect1 = 0;
	m_currentBitmap = p_bitmap;
	scale = (short) (p_destinationRect->m_width / p_sourceRect->m_width);
	p_bitmap->GetRects(*p_sourceRect, rect0, rect1);
	bool copiedSecond = 0;
	if (rect0 != 0) {
		CVsPoint* point = p_destinationRect;
		short height = (short) (rect0->m_height * scale);
		short width = (short) (rect0->m_width * scale);
		CVsRect destRect(point->m_x, point->m_y, width, height);
		copied = StretchBltContexts(p_destination, &destRect, p_source, rect0);
	}
	if (rect1 != 0) {
		short height = (short) (rect1->m_height * scale);
		short y = (short) (rect0->m_height * scale + p_destinationRect->m_y);
		short width = (short) (rect1->m_width * scale);
		CVsRect destRect(p_destinationRect->m_x, y, width, height);
		copiedSecond = StretchBltContexts(p_destination, &destRect, p_source, rect1);
	}
	return copied | copiedSecond;
}

// FUNCTION: LEMBALL 0x00458200
bool CGraphicsDriver::HasPalette()
{
	return m_palette != 0;
}

// GLOBAL: LEMBALL 0x004a076c
CGraphicsDriver* g_pTargetGraphicsDriver = 0;

// GLOBAL: LEMBALL 0x004a0770
unsigned int g_dwFullScreenGdi = 0;

// GLOBAL: LEMBALL 0x004a0768
CGraphicsState* g_pTargetGraphicsSystem = 0;
