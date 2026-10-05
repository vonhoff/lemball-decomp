#include "CPVGDIBitmap.h"

#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"

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
	if (m_lines != NULL) {
		operator delete(m_lines);
		m_lines = NULL;
		m_lineCapacity = 0;
	}
	m_bits = NULL;
	m_bitsBase = NULL;
}

// FUNCTION: LEMBALL 0x00472310
void CPVGDIBitmap::Initialise()
{
	m_directScroll = 0;
	m_bits = NULL;
	m_bitsBase = NULL;
	m_xOffset = 0;
	m_firstLine = 0;
	m_lines = NULL;
	m_stride = 0;
	m_extraRows = 0;
	m_rowPadding = 0;
	m_lineCapacity = 0;
}

// FUNCTION: LEMBALL 0x00472340
void CPVGDIBitmap::CreateLinePtrs()
{
	if ((int) m_lineCapacity < m_height) {
		if (m_lines != NULL) {
			operator delete(m_lines);
			m_lines = NULL;
		}
		if (m_height > 0) {
			short heightWord;
			unsigned int size;

			heightWord = m_height;
			size = (unsigned int) heightWord;
			size = size * sizeof(*m_lines);
			m_lines = (void**) operator new(size);
			ResetLinePtrs();
		}
		m_lineCapacity = (unsigned int) m_height;
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
		memset(m_bitsBase + abs(m_stride) * m_height + m_rowPadding, 0, m_rowPadding);
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
	if (m_height <= 0) {
		return;
	}
	do {
		m_lines[line] = bits + m_xOffset;
		line = line + 1;
		bits = bits + m_stride;
		if ((int) line >= m_height) {
			line = line - (unsigned int) m_height;
		}
		row = row + 1;
	} while (row < m_height);
}

// FUNCTION: LEMBALL 0x00472440
void CPVGDIBitmap::Scroll(const CVSRect* p_rect, const CVSPoint* p_destination)
{
	int width;
	int height;
	const CVSPoint* position;
	short deltaY;

	width = (int) p_rect->m_width;
	height = (int) p_rect->m_height;
	if (height * width == 0) {
		return;
	}
	if (m_directScroll == 0) {
		position = p_rect;
		deltaY = (short) (position->m_y - p_destination->m_y);
		m_xOffset = m_xOffset - (unsigned int) (short) (position->m_x - p_destination->m_x);
		height = deltaY + (int) m_firstLine;
		m_firstLine = (unsigned int) height;
		width = m_height;
		if ((int) m_firstLine < width) {
			if ((int) m_firstLine < 0) {
				m_firstLine = (unsigned int) (width + (int) m_firstLine);
			}
		}
		else {
			m_firstLine = (unsigned int) ((int) m_firstLine - width);
		}
		SetLinePtrs();
		return;
	}
	int rectX = p_rect->m_x;
	int rectY = p_rect->m_y;
	int destinationX = p_destination->m_x;
	int destinationY = p_destination->m_y;
	if (destinationY == rectY) {
		if (destinationX != rectX && 0 < height) {
			int srcY = rectY;
			int dstY = destinationY;
			do {
				memmove((unsigned char*) m_lines[srcY] + rectX, (unsigned char*) m_lines[dstY] + destinationX, width);
				height = height - 1;
				srcY = srcY + 1;
				dstY = dstY + 1;
			} while (height != 0);
		}
		return;
	}
	if (rectY < destinationY) {
		if (0 < height) {
			int srcY = rectY;
			int dstY = destinationY;
			do {
				memcpy((unsigned char*) m_lines[srcY] + rectX, (unsigned char*) m_lines[dstY] + destinationX, width);
				srcY = srcY + 1;
				dstY = dstY + 1;
				height = height - 1;
			} while (height != 0);
		}
		return;
	}
	int srcY = height - 1 + rectY;
	int dstY = height - 1 + destinationY;
	if (0 < height) {
		do {
			memcpy((unsigned char*) m_lines[srcY] + rectX, (unsigned char*) m_lines[dstY] + destinationX, width);
			srcY = srcY - 1;
			dstY = dstY - 1;
			height = height - 1;
		} while (height != 0);
	}
}

// FUNCTION: LEMBALL 0x004725f0
CVSSize CPVGDIBitmap::SetSize(const CVSSize& p_size, int p_pitch)
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
	return CVSSize(m_width, (short) (m_height + m_extraRows));
}

// FUNCTION: LEMBALL 0x00472670
void CPVGDIBitmap::SetBitsBase(unsigned char* p_bits, int p_stride)
{
	unsigned char* bits = m_rowPadding + p_bits;
	unsigned char*& bitsBase = m_bitsBase;
	bitsBase = p_bits;
	m_bits = bits;
	m_stride = p_stride;
	if (p_stride < 0) {
		m_bits += (1 - m_height) * p_stride;
	}
	CreateLinePtrs();
}

// FUNCTION: LEMBALL 0x004726b0
void CPVGDIBitmap::GetRects(const CVSRect& p_rect, CVSRect*& p_rect0, CVSRect*& p_rect1)
{
	const short* position;

	m_rect0.m_width = p_rect.m_width;
	m_rect0.m_height = p_rect.m_height;
	if (&p_rect != NULL) {
		position = &p_rect.m_x;
	}
	else {
		position = NULL;
	}
	m_rect0.m_x = position[0];
	m_rect0.m_y = position[1];
	p_rect0 = &m_rect0;
	p_rect1 = NULL;
	if ((int) m_firstLine < (int) (short) (p_rect.m_height + p_rect.m_y) && (int) p_rect.m_y < (int) m_firstLine) {
		m_rect0.m_height = (short) m_firstLine - m_rect0.m_y;
		m_rect1.m_width = p_rect.m_width;
		m_rect1.m_height = p_rect.m_height;
		if (&p_rect != NULL) {
			position = &p_rect.m_x;
		}
		else {
			position = NULL;
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
void CPVGDIBitmap::DrawCircleSymmetricPoints(int p_centreX, int p_centreY, int p_xOffset, int p_yOffset, int p_colour)
{
	*((unsigned char*) m_lines[p_yOffset + p_centreY] + p_centreX + p_xOffset) = (unsigned char) p_colour;
	*((unsigned char*) m_lines[p_yOffset + p_centreY] + p_centreX - p_xOffset) = (unsigned char) p_colour;
	*((unsigned char*) m_lines[p_centreY - p_yOffset] + p_centreX - p_xOffset) = (unsigned char) p_colour;
	*((unsigned char*) m_lines[p_centreY - p_yOffset] + p_centreX + p_xOffset) = (unsigned char) p_colour;
}
