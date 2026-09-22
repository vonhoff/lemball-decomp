#include "CPVZBuffSurface.h"

#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Graphics/CPVGDIBitmap.h"

// FUNCTION: LEMBALL 0x00466670
CPVZBuffSurface::CPVZBuffSurface()
{
	m_allocatedWidth = 0;
	m_enabled = 0;
	m_allocatedHeight = 0;
	m_buffer = 0;
}

// FUNCTION: LEMBALL 0x004666e0
CPVZBuffSurface::~CPVZBuffSurface()
{
}

// FUNCTION: LEMBALL 0x00466710
void CPVZBuffSurface::FreeZBuff()
{
	if (m_buffer != 0) {
		operator delete(m_buffer);
		m_buffer = 0;
		m_allocatedHeight = 0;
		m_allocatedWidth = 0;
	}
	m_bitmap.Free();
}

// FUNCTION: LEMBALL 0x00466740
void CPVZBuffSurface::AllocateZBuff()
{
	CVsSize size;
	unsigned int allocatedArea;
	unsigned int neededArea;

	size.m_width = (short) (m_windowRect.m_width * 2);
	size.m_height = m_windowRect.m_height;
	size = m_bitmap.SetSize(size, m_reserved40 * 2);
	allocatedArea = (unsigned int) m_allocatedWidth * (unsigned int) m_allocatedHeight * 2;
	neededArea = (unsigned int) ((int) size.m_height * (int) size.m_width);
	if (allocatedArea < neededArea) {
		FreeZBuff();
	}
	if ((int) m_windowRect.m_width * (int) m_windowRect.m_height != 0) {
		if (m_buffer == 0) {
			m_allocatedHeight = (unsigned short) size.m_height;
			m_allocatedWidth = (unsigned short) ((unsigned int) (int) size.m_width >> 1);
			m_buffer = new unsigned short[(unsigned int) m_allocatedWidth * (unsigned int) m_allocatedHeight];
		}
		if (m_buffer == 0) {
			m_enabled = 0;
		}
		m_bitmap.SetBitsBase((unsigned char*) m_buffer, (int) size.m_width);
	}
}

// FUNCTION: LEMBALL 0x00466840
void CPVZBuffSurface::EnableZBuff(int p_enabled)
{
	if (p_enabled == 0) {
		FreeZBuff();
		m_enabled = 0;
		return;
	}

	AllocateZBuff();
	m_enabled = 1;
}

// FUNCTION: LEMBALL 0x00466870
void CPVZBuffSurface::ResizeZBuff()
{
	AllocateZBuff();
}

// FUNCTION: LEMBALL 0x00466990
bool CPVZBuffSurface::HasZBuff()
{
	return m_enabled;
}
