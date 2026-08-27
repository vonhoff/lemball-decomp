#include "PvGdiBitmap.h"

#include <new.h>

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
// STUB: LEMBALL 0x004722d0
PvGdiBitmap::~PvGdiBitmap()
{
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
// STUB: LEMBALL 0x00472310
void PvGdiBitmap::Initialise()
{
}

// 68K 0x102129b4 CreateLinePtrs__12CPVGDIBitmapFv
// STUB: LEMBALL 0x00472340
void PvGdiBitmap::CreateLinePtrs()
{
	if ((int) m_lineCapacity < (int) m_height) {
		if (m_lines != 0) {
			operator delete(m_lines);
			m_lines = 0;
		}
		if (0 < m_height) {
			m_lines = (void**) operator new((unsigned int) m_height * 4);
			ResetLinePtrs();
		}
		m_lineCapacity = (unsigned int) m_height;
		return;
	}
	ResetLinePtrs();
}

// 68K 0x10212a46 ResetLinePtrs__12CPVGDIBitmapFv
// STUB: LEMBALL 0x004723a0
void PvGdiBitmap::ResetLinePtrs()
{
	m_xOffset = 0;
	m_firstLine = 0;
	SetLinePtrs();
}

// 68K 0x10212ae8 SetLinePtrs__12CPVGDIBitmapFv
// STUB: LEMBALL 0x00472400
void PvGdiBitmap::SetLinePtrs()
{
	unsigned char* bits;
	unsigned int line;
	int row;

	if (m_lines == 0) {
		return;
	}
	bits = m_bits;
	line = m_firstLine;
	row = 0;
	while (row < m_height) {
		m_lines[line] = bits + m_xOffset;
		line = line + 1;
		bits = bits + m_stride;
		if ((int) m_height <= (int) line) {
			line = line - (unsigned int) m_height;
		}
		row = row + 1;
	}
}

// 68K 0x10212b5e Scroll__12CPVGDIBitmapFRC7CVSRectRC8CVSPoint
// STUB: LEMBALL 0x00472440
void PvGdiBitmap::Scroll(const VsRect& p_rect, const VsPoint& p_destination)
{
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
// STUB: LEMBALL 0x00472670
void PvGdiBitmap::SetBitsBase(unsigned char* p_bits, int p_stride)
{
	m_bitsBase = p_bits;
	m_bits = p_bits + m_rowPadding;
	m_stride = p_stride;
	if (p_stride < 0) {
		m_bits = p_bits + m_rowPadding + (1 - m_height) * p_stride;
	}
	CreateLinePtrs();
}

// 68K 0x10212d8a GetRects__12CPVGDIBitmapFRC7CVSRectRP7CVSRectRP7CVSRect
// STUB: LEMBALL 0x004726b0
void PvGdiBitmap::GetRects(const VsRect& p_rect, VsRect*& p_rect0, VsRect*& p_rect1)
{
	m_rect0.m_width = p_rect.m_width;
	m_rect0.m_height = p_rect.m_height;
	m_rect0.m_x = p_rect.m_x;
	m_rect0.m_y = p_rect.m_y;
	p_rect0 = &m_rect0;
	p_rect1 = 0;
	if ((int) m_firstLine < (int) (short) (p_rect.m_height + p_rect.m_y) && (int) p_rect.m_y < (int) m_firstLine) {
		m_rect0.m_height = (short) ((int) m_firstLine - (int) m_rect0.m_y);
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
// STUB: LEMBALL 0x00472760
void PvGdiBitmap::ResetScroll()
{
}
