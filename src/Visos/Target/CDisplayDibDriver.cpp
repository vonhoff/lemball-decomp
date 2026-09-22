#include "CDisplayDibDriver.h"

#include "CGdiContext.h"

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Target/CDibContext.h"
#include "Visos/Target/CGraphicsDriver.h"

#include <conio.h>
#include <string.h>
#include <windows.h>

#pragma intrinsic(_outp)

typedef int(__stdcall* DisplayDibProc)(void*, void*, unsigned int);

// GLOBAL: LEMBALL 0x004a8290
DisplayDibProc g_pDisplayDib = 0;

// FUNCTION: LEMBALL 0x00456d70
CDisplayDibDriver::CDisplayDibDriver(const CVsSize& p_size)
{
	m_ready = 0;
	m_screenSize.m_width = p_size.m_width;
	m_screenSize.m_height = p_size.m_height;
	m_displayDibStart = 0;
	m_displayDibEnd = 0;
	m_displayDibGetAddress = 0;
	g_pDisplayDib = 0;
	m_driverModule = LoadLibraryA("DSPDIB32.DLL");
	if (m_driverModule != 0) {
		m_displayDibStart = (void(__stdcall*)()) GetProcAddress((HMODULE) m_driverModule, "DisplayDibStart32");
		m_displayDibEnd = (void(__stdcall*)()) GetProcAddress((HMODULE) m_driverModule, "DisplayDibEnd32");
		m_displayDibGetAddress =
			(unsigned char*(__stdcall*) ()) GetProcAddress((HMODULE) m_driverModule, "DisplayDibGetAddress32");
		g_pDisplayDib =
			(int(__stdcall*)(void*, void*, unsigned int)) GetProcAddress((HMODULE) m_driverModule, "DisplayDib32");
		m_frameBuffer = m_displayDibGetAddress();
		m_displayDibStart();
		m_ready = 1;
	}
}

// FUNCTION: LEMBALL 0x00456e20
CDisplayDibDriver::~CDisplayDibDriver()
{
	if (m_displayDibEnd != 0) {
		g_pDisplayDib(0, 0, 0x4000);
		m_displayDibEnd();
	}
}

// FUNCTION: LEMBALL 0x00456e50
unsigned int CDisplayDibDriver::UpdateDibColourTable(CDrawingContext* p_context,
													 unsigned int p_start,
													 unsigned int p_count,
													 void* p_colours)
{
	SetDIBColorTable((HDC) ((CGdiContext*) p_context)->m_hDC, p_start, p_count, (RGBQUAD*) p_colours);
	_outp(0x3c8, p_start);
	RGBQUAD* colour = (RGBQUAD*) p_colours;
	do {
		_outp(0x3c9, colour->rgbRed >> 2);
		_outp(0x3c9, colour->rgbGreen >> 2);
		_outp(0x3c9, colour->rgbBlue >> 2);
		colour++;
		p_count--;
	} while (p_count != 0);
	return 256;
}

// FUNCTION: LEMBALL 0x00456eb0
int CDisplayDibDriver::BitBltContexts(CDrawingContext* p_destination,
									  CVsRect* p_rect,
									  CDrawingContext* p_source,
									  CVsPoint* p_position)
{
	if (g_dwFullScreenGdi != 0) {
		return BitBlt((HDC) ((CGdiContext*) p_destination)->m_hDC,
					  p_rect->m_x,
					  p_rect->m_y,
					  p_rect->m_width,
					  p_rect->m_height,
					  (HDC) ((CGdiContext*) p_source)->m_hDC,
					  p_position->m_x,
					  p_position->m_y,
					  SRCCOPY);
	}
	CVsRect clipped;
	clipped.m_width = p_rect->m_width;
	clipped.m_height = p_rect->m_height;
	CVsPoint* point = p_rect;
	clipped.m_x = point->m_x;
	clipped.m_y = point->m_y;
	CVsSize limits;
	limits.m_width = m_screenSize.m_width;
	limits.m_height = m_screenSize.m_height;
	if (clipped.m_x < 0) {
		clipped.m_width += clipped.m_x;
		clipped.m_x = 0;
	}
	if ((short) (clipped.m_width + clipped.m_x) > limits.m_width) {
		clipped.m_width = limits.m_width - clipped.m_x;
	}
	if (clipped.m_y < 0) {
		clipped.m_height += clipped.m_y;
		clipped.m_y = 0;
	}
	if ((short) (clipped.m_y + clipped.m_height) > limits.m_height) {
		clipped.m_height = limits.m_height - clipped.m_y;
	}
	if (clipped.m_width <= 0 || clipped.m_height <= 0) {
		clipped.m_height = 0;
		clipped.m_x = 0;
		clipped.m_width = 0;
		clipped.m_y = 0;
	}
	int rows = clipped.m_height;
	int bytes = clipped.m_width;
	if (rows * bytes == 0) {
		return 1;
	}
	int stride = ((CDibContext*) ((CGdiContext*) p_source)->m_hBitmap)->GetStride();
	unsigned char* destination = m_frameBuffer + clipped.m_y * m_screenSize.m_width + clipped.m_x;
	unsigned char* source = ((CDibContext*) ((CGdiContext*) p_source)->m_hBitmap)->GetBits();
	source += p_position->m_y * stride;
	source += p_position->m_x;
	int i = 0;
	while (1) {
		if (i >= rows) {
			break;
		}
		memcpy(destination, source, bytes);
		source += stride;
		destination += m_screenSize.m_width;
		i++;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00457020
int CDisplayDibDriver::StretchBltContexts(CDrawingContext* p_destination,
										  CVsRect* p_rect,
										  CDrawingContext* p_source,
										  CVsRect* p_sourceRect)
{
	CVsPoint* point = p_sourceRect;
	CVsPoint position(*point);
	return BitBltContexts(p_destination, p_rect, p_source, &position);
}
