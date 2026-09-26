#include "CPVGDIBitmap.h"

#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"

#include <stdlib.h>
#include <string.h>

// FUNCTION: LEMBALL 0x00472290
CPVGDIBitmap::CPVGDIBitmap() : m_height(0)
{
	m_width = 0;
	Initialise();
}

// FUNCTION: LEMBALL 0x004722d0
CPVGDIBitmap::~CPVGDIBitmap()
{
	Free();
}

// FUNCTION: LEMBALL 0x004722e0
void CPVGDIBitmap::Free()
{
	if (m_lines != 0) {
		operator delete(m_lines);
		m_lines = 0;
		m_lineCapacity = 0;
	}
	m_bits = 0;
	m_bitsBase = 0;
}

// FUNCTION: LEMBALL 0x00472310
void CPVGDIBitmap::Initialise()
{
	m_directScroll = 0;
	m_bits = 0;
	m_bitsBase = 0;
	m_xOffset = 0;
	m_firstLine = 0;
	m_lines = 0;
	m_stride = 0;
	m_extraRows = 0;
	m_rowPadding = 0;
	m_lineCapacity = 0;
}

// FUNCTION: LEMBALL 0x00472340
void CPVGDIBitmap::CreateLinePtrs()
{
	if ((int) m_lineCapacity < (int) (short) m_height) {
		if (m_lines != 0) {
			operator delete(m_lines);
			m_lines = 0;
		}
		if ((short) m_height > 0) {
			short heightWord;
			unsigned int size;

			heightWord = m_height;
			size = (unsigned int) (int) heightWord;
			size = size << 2;
			m_lines = (void**) operator new(size);
			ResetLinePtrs();
		}
		m_lineCapacity = (unsigned int) (int) (short) m_height;
		return;
	}
	ResetLinePtrs();
}

// FUNCTION: LEMBALL 0x004723a0
void CPVGDIBitmap::ResetLinePtrs()
{
	m_xOffset = 0;
	m_firstLine = 0;
	SetLinePtrs();
	if ((int) m_rowPadding > 0) {
		memset(m_bitsBase, 0, m_rowPadding);
		memset(m_bitsBase + abs(m_stride) * (int) (short) m_height + m_rowPadding, 0, m_rowPadding);
	}
}

// FUNCTION: LEMBALL 0x00472400
void CPVGDIBitmap::SetLinePtrs()
{
	unsigned char* bits;
	unsigned int line;
	int row;

	bits = m_bits;
	line = m_firstLine;
	row = 0;
	if ((short) m_height <= 0) {
		return;
	}
	do {
		m_lines[line] = bits + m_xOffset;
		line = line + 1;
		bits = bits + m_stride;
		if ((int) line >= (int) (short) m_height) {
			line = line - (unsigned int) (short) m_height;
		}
		row = row + 1;
	} while (row < (int) (short) m_height);
}

// FUNCTION: LEMBALL 0x00472440
void CPVGDIBitmap::Scroll(const CVsRect* p_rect, const CVsPoint* p_destination)
{
	int width;
	int height;
	const short* xy;
	short deltaY;
	unsigned int count;
	unsigned int* srcPtr;
	unsigned int* destPtr;

	width = (int) p_rect->m_width;
	height = (int) p_rect->m_height;
	if (height * width == 0) {
		return;
	}
	if (m_directScroll == 0) {
		xy = &p_rect->m_x;
		if (p_rect == 0) {
			xy = 0;
		}
		deltaY = (short) (xy[1] - p_destination->m_y);
		m_xOffset = m_xOffset - (unsigned int) (short) (xy[0] - p_destination->m_x);
		height = (int) (short) deltaY + (int) m_firstLine;
		m_firstLine = (unsigned int) height;
		width = (unsigned int) (int) (short) m_height;
		if ((int) m_firstLine < (int) width) {
			if ((int) m_firstLine < 0) {
				m_firstLine = (unsigned int) ((int) width + (int) m_firstLine);
			}
		}
		else {
			m_firstLine = (unsigned int) ((int) m_firstLine - (int) width);
		}
		SetLinePtrs();
		return;
	}
	if (p_destination->m_y == p_rect->m_y) {
		if (p_destination->m_x != p_rect->m_x && 0 < height) {
			int srcY = p_rect->m_y;
			int dstY = p_destination->m_y;
			do {
				memmove((unsigned char*) m_lines[srcY] + p_rect->m_x,
						(unsigned char*) m_lines[dstY] + p_destination->m_x,
						width);
				height = height - 1;
				srcY = srcY + 1;
				dstY = dstY + 1;
			} while (height != 0);
		}
		return;
	}
	if (p_rect->m_y < p_destination->m_y) {
		if (0 < height) {
			int srcY = p_rect->m_y;
			int dstY = p_destination->m_y;
			do {
				destPtr = (unsigned int*) ((unsigned char*) m_lines[srcY] + p_rect->m_x);
				srcPtr = (unsigned int*) ((unsigned char*) m_lines[dstY] + p_destination->m_x);
				for (count = width >> 2; count != 0; count = count - 1) {
					*destPtr = *srcPtr;
					destPtr = destPtr + 1;
					srcPtr = srcPtr + 1;
				}
				for (count = width & 3; count != 0; count = count - 1) {
					*(unsigned char*) destPtr = *(unsigned char*) srcPtr;
					destPtr = (unsigned int*) ((int) destPtr + 1);
					srcPtr = (unsigned int*) ((int) srcPtr + 1);
				}
				srcY = srcY + 1;
				dstY = dstY + 1;
				height = height - 1;
			} while (height != 0);
		}
		return;
	}
	if (0 < height) {
		int srcY = height - 1 + p_rect->m_y;
		int dstY = height - 1 + p_destination->m_y;
		do {
			destPtr = (unsigned int*) ((unsigned char*) m_lines[srcY] + p_rect->m_x);
			srcPtr = (unsigned int*) ((unsigned char*) m_lines[dstY] + p_destination->m_x);
			for (count = width >> 2; count != 0; count = count - 1) {
				*destPtr = *srcPtr;
				destPtr = destPtr + 1;
				srcPtr = srcPtr + 1;
			}
			for (count = width & 3; count != 0; count = count - 1) {
				*(unsigned char*) destPtr = *(unsigned char*) srcPtr;
				destPtr = (unsigned int*) ((int) destPtr + 1);
				srcPtr = (unsigned int*) ((int) srcPtr + 1);
			}
			srcY = srcY - 1;
			dstY = dstY - 1;
			height = height - 1;
		} while (height != 0);
	}
}

// FUNCTION: LEMBALL 0x004725f0
CVsSize CPVGDIBitmap::SetSize(const CVsSize& p_size, int p_pitch)
{
	m_width = p_size.m_width;
	m_height = p_size.m_height;
	if (p_pitch == 0 || m_directScroll != 0) {
		m_rowPadding = 0;
	}
	else {
		m_rowPadding = p_pitch - m_width;
	}
	if (p_size.m_width == 0) {
		m_extraRows = 0;
	}
	else {
		m_extraRows = (int) (p_size.m_width - 1 + m_rowPadding * 2) / (int) p_size.m_width;
	}
	return CVsSize(m_width, (short) (m_height + m_extraRows));
}

// FUNCTION: LEMBALL 0x00472670
void CPVGDIBitmap::SetBitsBase(unsigned char* p_bits, int p_stride)
{
	m_bitsBase = p_bits;
	m_bits = m_rowPadding + p_bits;
	m_stride = p_stride;
	if (p_stride < 0) {
		m_bits += (1 - (int) (short) m_height) * p_stride;
	}
	CreateLinePtrs();
}

// FUNCTION: LEMBALL 0x004726b0
void CPVGDIBitmap::GetRects(const CVsRect& p_rect, CVsRect*& p_rect0, CVsRect*& p_rect1)
{
	const short* position;

	m_rect0.m_width = p_rect.m_width;
	m_rect0.m_height = p_rect.m_height;
	if (&p_rect != 0) {
		position = &p_rect.m_x;
	}
	else {
		position = 0;
	}
	m_rect0.m_x = position[0];
	m_rect0.m_y = position[1];
	p_rect0 = &m_rect0;
	p_rect1 = 0;
	if ((int) m_firstLine < (int) (short) (p_rect.m_height + p_rect.m_y) && (int) p_rect.m_y < (int) m_firstLine) {
		m_rect0.m_height = (short) m_firstLine - m_rect0.m_y;
		m_rect1.m_width = p_rect.m_width;
		m_rect1.m_height = p_rect.m_height;
		if (&p_rect != 0) {
			position = &p_rect.m_x;
		}
		else {
			position = 0;
		}
		m_rect1.m_x = position[0];
		m_rect1.m_y = position[1];
		m_rect1.m_height = (short) ((p_rect.m_height - (short) m_firstLine) + p_rect.m_y);
		m_rect1.m_y = (short) m_firstLine;
		p_rect1 = &m_rect1;
	}
}

// FUNCTION: LEMBALL 0x00472760
void CPVGDIBitmap::ResetScroll()
{
	ResetLinePtrs();
}

// FUNCTION: LEMBALL 0x00475c80
void CPVGDIBitmap::DrawCircleSymmetricPoints(int p_centerX,
											 int p_centerY,
											 int p_xOffset,
											 int p_yOffset,
											 unsigned char p_color)
{
	int rowPos;
	int rowNeg;
	unsigned char color;

	color = p_color;
	rowPos = p_yOffset;
	rowPos = rowPos + p_centerY;
	rowNeg = p_centerY - p_yOffset;
	*((unsigned char*) m_lines[rowPos] + p_centerX + p_xOffset) = color;
	*((unsigned char*) m_lines[rowPos] + p_centerX - p_xOffset) = color;
	*((unsigned char*) m_lines[rowNeg] + p_centerX - p_xOffset) = color;
	*((unsigned char*) m_lines[rowNeg] + p_centerX + p_xOffset) = color;
}
