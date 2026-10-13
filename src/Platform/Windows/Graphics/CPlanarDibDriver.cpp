#include "CPlanarDibDriver.h"

#include "CGdiContext.h"
#include "CPVGDIBitmap.h"

#define WIN32_LEAN_AND_MEAN
#include "CDibContext.h"
#include "CGraphicsDriver.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"

#include <conio.h>
#include <windows.h>

enum {
	PLANAR_DIB_PLANE_INDEX_MASK = 3,
	VGA_SEQUENCER_INDEX_PORT = 0x3c4,
	VGA_SEQUENCER_MAP_MASK_REGISTER_INDEX = 2
};

// FUNCTION: LEMBALL 0x00457070
void CPlanarDibDriver::SetPlaneWriteMask(unsigned char p_mask)
{
	_outpw(VGA_SEQUENCER_INDEX_PORT, MAKEWORD(VGA_SEQUENCER_MAP_MASK_REGISTER_INDEX, p_mask));
}

// FUNCTION: LEMBALL 0x00457080
void CPlanarDibDriver::ExtractPlaneBytes(unsigned char* p_destination, unsigned char* p_source, int p_count)
{
	int remaining = p_count;
	if (remaining > 0) {
		unsigned char* source = p_source;
		unsigned char* destination = p_destination;
		do {
			*destination++ = *source;
			source += 4;
		} while (--remaining != 0);
	}
}

// FUNCTION: LEMBALL 0x004570b0
int CPlanarDibDriver::BitBltContexts(CDrawingContext* p_destination,
									 CVSRect* p_rect,
									 CDrawingContext* p_source,
									 CVSPoint* p_position)
{
	if (g_dwFullScreenGdi != 0) {
		return BitBlt(((CGdiContext*) p_destination)->m_hDC,
					  p_rect->m_x,
					  p_rect->m_y,
					  p_rect->m_width,
					  p_rect->m_height,
					  ((CGdiContext*) p_source)->m_hDC,
					  p_position->m_x,
					  p_position->m_y,
					  SRCCOPY);
	}
	CVSRect clipped;
	clipped.m_width = p_rect->m_width;
	clipped.m_height = p_rect->m_height;
	CVSPoint* point = p_rect;
	clipped.m_x = point->m_x;
	clipped.m_y = point->m_y;
	CVSSize limits;
	limits.m_width = m_screenSize.m_width;
	limits.m_height = m_screenSize.m_height;
	clipped.ClipToSize(limits);
	int width;
	int rows;
	rows = clipped.m_height;
	width = clipped.m_width;
	if (rows * width == 0) {
		return 1;
	}
	((CGdiContext*) p_source)->m_dibContext->GetStride();
	int destinationX = p_rect->m_x;
	int sourceX = p_position->m_x;
	int sourceEnd = width - 1 + sourceX;
	int stride = m_screenSize.m_width / 4;
	int row = p_position->m_y;
	unsigned char* destination = m_frameBuffer + p_rect->m_y * stride;
	int i = 0;
	for (;;) {
		if (i >= rows) {
			break;
		}
		unsigned char* source = m_currentBitmap->m_lines[row];
		int plane = 0;
		do {
			int x = destinationX + plane;
			int offset = sourceX + plane;
			if (offset <= sourceEnd) {
				int count = (sourceEnd - offset) / 4 + 1;
				SetPlaneWriteMask(1 << (x & PLANAR_DIB_PLANE_INDEX_MASK));
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
int CPlanarDibDriver::StretchBltContexts(CDrawingContext* p_destination,
										 CVSRect* p_rect,
										 CDrawingContext* p_source,
										 CVSRect* p_sourceRect)
{
	CVSPoint* point = p_sourceRect;
	CVSPoint position(*point);
	return BitBltContexts(p_destination, p_rect, p_source, &position);
}
