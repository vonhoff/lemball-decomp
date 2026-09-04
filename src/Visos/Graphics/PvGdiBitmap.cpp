#include "PvGdiBitmap.h"

#include <new.h>
#include <string.h>

// 68K 0x1021284a __ct__12CPVGDIBitmapFv
// STUB: LEMBALL 0x00472290
PvGdiBitmap::PvGdiBitmap()
{
	m_lines = 0;
	m_bits = 0;
	m_bitsBase = 0;
	m_directScroll = 0;
	m_firstLine = 0;
	m_xOffset = 0;
	m_stride = 0;
	m_rowPadding = 0;
	m_extraRows = 0;
	m_lineCapacity = 0;
	m_width = 0;
	m_height = 0;
}

// 68K 0x102128ca __dt__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x004722d0
PvGdiBitmap::~PvGdiBitmap()
{
	Free();
}

// 68K 0x1021291c Free__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x004722e0
void PvGdiBitmap::Free()
{
	if (m_lines != 0) {
		operator delete(m_lines);
		m_lines = 0;
		m_lineCapacity = 0;
	}
	m_bits = 0;
	m_bitsBase = 0;
}

// 68K 0x10212962 Initialise__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x00472310
void PvGdiBitmap::Initialise()
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

// 68K 0x102129b4 CreateLinePtrs__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x00472340
void PvGdiBitmap::CreateLinePtrs()
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

// 68K 0x10212a46 ResetLinePtrs__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x004723a0
void PvGdiBitmap::ResetLinePtrs()
{
	unsigned int pad;
	unsigned char* dest;
	unsigned int count;
	unsigned int rowPad;
	int stride;
	int strideAbs;
	short heightWord;
	int sign;

	m_xOffset = 0;
	m_firstLine = 0;
	SetLinePtrs();
	pad = m_rowPadding;
	if ((int) pad <= 0) {
		return;
	}
	dest = m_bitsBase;
	count = pad >> 2;
	while (count != 0) {
		*(unsigned int*) dest = 0;
		dest = dest + 4;
		count = count - 1;
	}
	count = pad & 3;
	while (count != 0) {
		*dest = 0;
		dest = dest + 1;
		count = count - 1;
	}
	rowPad = m_rowPadding;
	stride = m_stride;
	sign = stride >> 31;
	strideAbs = (stride ^ sign) - sign;
	heightWord = m_height;
	dest = m_bitsBase + rowPad + strideAbs * (int) heightWord;
	count = rowPad >> 2;
	while (count != 0) {
		*(unsigned int*) dest = 0;
		dest = dest + 4;
		count = count - 1;
	}
	count = rowPad & 3;
	while (count != 0) {
		*dest = 0;
		dest = dest + 1;
		count = count - 1;
	}
}

// 68K 0x10212ae8 SetLinePtrs__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x00472400
void PvGdiBitmap::SetLinePtrs()
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

// 68K 0x10212b5e Scroll__12CPVGDIBitmapFRC7CVSRectRC8CVSPoint
// FUNCTION: LEMBALL 0x00472440
void PvGdiBitmap::Scroll(const VsRect* p_rect, const VsPoint* p_destination)
{
	unsigned char frameSpace[0xc];
	int width;
	int height;
	const short* xy;
	short deltaY;
	unsigned int count;
	unsigned int* srcPtr;
	unsigned int* destPtr;

	*(PvGdiBitmap**) frameSpace = this;
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
				memcpy((unsigned char*) m_lines[srcY] + p_rect->m_x,
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

// 68K 0x10212c50 SetSize__12CPVGDIBitmapFRC7CVSSizei
// STUB: LEMBALL 0x004725f0
VsSize PvGdiBitmap::SetSize(const VsSize& p_size, int p_pitch)
{
	VsSize size;

	m_width = p_size.m_width;
	m_height = p_size.m_height;
	if (p_pitch != 0) {
		m_stride = p_pitch;
	}
	size.m_width = m_width;
	size.m_height = m_height;
	return size;
}

// 68K 0x10212d18 SetBitsBase__12CPVGDIBitmapFPUci
// FUNCTION: LEMBALL 0x00472670
void PvGdiBitmap::SetBitsBase(unsigned char* p_bits, int p_stride)
{
	unsigned char* bits;
	unsigned char* padded;
	int stride;

	bits = p_bits;
	padded = bits + m_rowPadding;
	m_bitsBase = bits;
	stride = p_stride;
	m_bits = padded;
	m_stride = stride;
	if (stride < 0) {
		int adjust;

		adjust = 1;
		adjust = adjust - (int) (short) m_height;
		adjust = adjust * stride;
		m_bits = padded + adjust;
	}
	CreateLinePtrs();
}

// 68K 0x10212d8a GetRects__12CPVGDIBitmapFRC7CVSRectRP7CVSRectRP7CVSRect
// FUNCTION: LEMBALL 0x004726b0
void PvGdiBitmap::GetRects(const VsRect& p_rect, VsRect*& p_rect0, VsRect*& p_rect1)
{
	m_rect0.m_width = p_rect.m_width;
	m_rect0.m_height = p_rect.m_height;
	m_rect0.m_x = p_rect.m_x;
	m_rect0.m_y = p_rect.m_y;
	p_rect0 = &m_rect0;
	p_rect1 = 0;
	if ((int) m_firstLine < (int) (short) (p_rect.m_height + p_rect.m_y) && (int) p_rect.m_y < (int) m_firstLine) {
		m_rect0.m_height = (short) m_firstLine - m_rect0.m_y;
		m_rect1.m_width = p_rect.m_width;
		m_rect1.m_height = p_rect.m_height;
		m_rect1.m_x = p_rect.m_x;
		m_rect1.m_y = p_rect.m_y;
		m_rect1.m_height = (short) ((p_rect.m_height - (short) m_firstLine) + p_rect.m_y);
		m_rect1.m_y = (short) m_firstLine;
		p_rect1 = &m_rect1;
	}
}

// 68K 0x10212e96 ResetScroll__12CPVGDIBitmapFv
// FUNCTION: LEMBALL 0x00472760
void PvGdiBitmap::ResetScroll()
{
	ResetLinePtrs();
}

// FUNCTION: LEMBALL 0x00475c80
void PvGdiBitmap::DrawCircleSymmetricPoints(int p_centerX,
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
