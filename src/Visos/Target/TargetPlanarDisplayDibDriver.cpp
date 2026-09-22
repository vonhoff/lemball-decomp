#include "TargetPlanarDisplayDibDriver.h"

#include "../Graphics/CPvGdiBitmap.h"
#include "TargetGdiDrawingContext.h"

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Target/TargetDibContext.h"
#include "Visos/Target/TargetGraphicsDriver.h"

#include <conio.h>
#include <windows.h>

#pragma intrinsic(_outpw)

// FUNCTION: LEMBALL 0x00457070
void TargetPlanarDisplayDibDriver::SetPlaneWriteMask(unsigned char p_mask)
{
	_outpw(0x3c4, ((unsigned short) p_mask << 8) | 2);
}

// FUNCTION: LEMBALL 0x00457080
void TargetPlanarDisplayDibDriver::ExtractPlaneBytes(unsigned char* p_destination, unsigned char* p_source, int p_count)
{
	while (p_count > 0) {
		*p_destination++ = *p_source;
		p_source += 4;
		p_count--;
	}
}

// FUNCTION: LEMBALL 0x004570b0
int TargetPlanarDisplayDibDriver::BitBltContexts(TargetDrawingContext* p_destination,
												 CVsRect* p_rect,
												 TargetDrawingContext* p_source,
												 CVsPoint* p_position)
{
	if (g_dwFullScreenGdi != 0) {
		return BitBlt((HDC) ((TargetGdiDrawingContext*) p_destination)->m_hDC,
					  p_rect->m_x,
					  p_rect->m_y,
					  p_rect->m_width,
					  p_rect->m_height,
					  (HDC) ((TargetGdiDrawingContext*) p_source)->m_hDC,
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
		clipped.m_width = 0;
	}
	int rows = clipped.m_height;
	int width = clipped.m_width;
	if (rows * width == 0) {
		return 1;
	}
	((TargetDibContext*) ((TargetGdiDrawingContext*) p_source)->m_hBitmap)->GetStride();
	int destinationX = p_rect->m_x;
	int sourceX = p_position->m_x;
	int sourceEnd = width - 1 + sourceX;
	int stride = (short) (m_screenSize.m_width / 4);
	int row = p_position->m_y;
	unsigned char* destination = m_frameBuffer + p_rect->m_y * stride;
	int i = 0;
	while (i < rows) {
		unsigned char* source = (unsigned char*) m_currentBitmap->m_lines[row];
		int plane = 0;
		do {
			int x = destinationX + plane;
			int offset = sourceX + plane;
			if (offset <= sourceEnd) {
				int count = (sourceEnd - offset) / 4 + 1;
				SetPlaneWriteMask((unsigned char) (1 << (x & 3)));
				ExtractPlaneBytes(destination + x / 4, source + offset, count);
			}
			plane++;
		} while (plane < 4);
		i++;
		row++;
		destination += stride;
	}
	SetPlaneWriteMask(15);
	return 1;
}

// FUNCTION: LEMBALL 0x004572c0
int TargetPlanarDisplayDibDriver::StretchBltContexts(TargetDrawingContext* p_destination,
													 CVsRect* p_rect,
													 TargetDrawingContext* p_source,
													 CVsRect* p_sourceRect)
{
	CVsPoint* point = p_sourceRect;
	CVsPoint position(*point);
	return BitBltContexts(p_destination, p_rect, p_source, &position);
}
