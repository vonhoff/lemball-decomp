#include "CPVZBuffSurface.h"

#include "CPVGDIBitmap.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00466670
CPVZBuffSurface::CPVZBuffSurface()
{
	m_allocatedWidth = 0;
	m_enabled = 0;
	m_allocatedHeight = 0;
	m_buffer = NULL;
}

// FUNCTION: LEMBALL 0x004666e0
CPVZBuffSurface::~CPVZBuffSurface()
{
}

// FUNCTION: LEMBALL 0x00466710
void CPVZBuffSurface::FreeZBuff()
{
	if (m_buffer != NULL) {
		delete[] m_buffer;
		m_buffer = NULL;
		m_allocatedHeight = 0;
		m_allocatedWidth = 0;
	}
	m_bitmap.Free();
}

// FUNCTION: LEMBALL 0x00466740
void CPVZBuffSurface::AllocateZBuff()
{
	unsigned int allocatedArea;
	unsigned int neededArea;

	CVSSize size = m_bitmap.SetSize(CVSRect(0, 0, (short) (m_windowRect.m_width * 2), m_windowRect.m_height),
									(unsigned int) m_worldWidth * 2);
	allocatedArea = (unsigned int) m_allocatedWidth * m_allocatedHeight * 2;
	neededArea = size.m_height * size.m_width;
	if (allocatedArea < neededArea) {
		FreeZBuff();
	}
	if (m_windowRect.m_width * m_windowRect.m_height != 0) {
		if (m_buffer == NULL) {
			m_allocatedHeight = size.m_height;
			m_allocatedWidth = (unsigned short) ((unsigned int) size.m_width >> 1);
			m_buffer = new unsigned short[(unsigned int) m_allocatedWidth * m_allocatedHeight];
		}
		if (m_buffer == NULL) {
			m_enabled = 0;
		}
		m_bitmap.SetBitsBase((unsigned char*) m_buffer, size.m_width);
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
