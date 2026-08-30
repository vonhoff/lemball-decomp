#include "PvZBuffSurface.h"

#include "VsGdi.h"

#include <new.h>

// 68K 0x1021623a __ct__15CPVZBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466670
PvZBuffSurface::PvZBuffSurface()
{
	m_allocatedWidth = 0;
	m_enabled = 0;
	m_allocatedHeight = 0;
	m_buffer = 0;
}

// 68K 0x102162b2 __dt__15CPVZBuffSurfaceFv
// FUNCTION: LEMBALL 0x004666e0
PvZBuffSurface::~PvZBuffSurface()
{
}

// 68K 0x1021632c FreeZBuff__15CPVZBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466710
void PvZBuffSurface::FreeZBuff()
{
	if (m_buffer != 0) {
		operator delete(m_buffer);
		m_buffer = 0;
		m_allocatedHeight = 0;
		m_allocatedWidth = 0;
	}
	m_bitmap.Free();
}

// 68K 0x10216388 AllocateZBuff__15CPVZBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466740
void PvZBuffSurface::AllocateZBuff()
{
	VsSize size;
	int allocatedArea;
	int neededArea;

	size.m_width = (short) (m_windowRect.m_width * 2);
	size.m_height = m_windowRect.m_height;
	m_bitmap.SetSize(size, (int) m_reserved40 * 2);
	allocatedArea = (int) m_allocatedWidth * (int) m_allocatedHeight * 2;
	neededArea = (int) size.m_height * (int) size.m_width;
	if (allocatedArea < neededArea) {
		FreeZBuff();
	}
	if ((int) m_windowRect.m_width * (int) m_windowRect.m_height != 0) {
		if (m_buffer == 0) {
			m_allocatedHeight = (unsigned short) size.m_height;
			m_allocatedWidth = (unsigned short) ((unsigned short) size.m_width / 2);
			m_buffer = (unsigned short*) operator new((unsigned int) ((unsigned short) size.m_width / 2) *
													  (unsigned int) (unsigned short) size.m_height * 2);
		}
		if (m_buffer == 0) {
			m_enabled = 0;
		}
		m_bitmap.SetBitsBase((unsigned char*) m_buffer, (int) size.m_width);
	}
}

// 68K 0x102164b6 EnableZBuff__15CPVZBuffSurfaceFUc
// FUNCTION: LEMBALL 0x00466840
void PvZBuffSurface::EnableZBuff(unsigned int p_enabled)
{
	if (p_enabled == 0) {
		FreeZBuff();
		m_enabled = 0;
		return;
	}
	AllocateZBuff();
	m_enabled = 1;
}

// 68K 0x1021650e ResizeZBuff__15CPVZBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466870
void PvZBuffSurface::ResizeZBuff()
{
	AllocateZBuff();
}

// 68K 0x10101386 HasZBuff__15CPVZBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466990
bool PvZBuffSurface::HasZBuff()
{
	return m_enabled;
}
