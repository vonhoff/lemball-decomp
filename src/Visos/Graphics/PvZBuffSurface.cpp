#include "PvZBuffSurface.h"

// 68K 0x1021623a __ct__15CPVZBuffSurfaceFv
// STUB: LEMBALL 0x00466670
PvZBuffSurface::PvZBuffSurface()
{
	m_allocatedWidth = 0;
	m_enabled = 0;
	m_allocatedHeight = 0;
	m_buffer = 0;
}

// 68K 0x102162b2 __dt__15CPVZBuffSurfaceFv
// STUB: LEMBALL 0x004666e0
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
// STUB: LEMBALL 0x00466740
void PvZBuffSurface::AllocateZBuff()
{
}

// 68K 0x102164b6 EnableZBuff__15CPVZBuffSurfaceFUc
// FUNCTION: LEMBALL 0x00466840
void PvZBuffSurface::EnableZBuff(unsigned char p_enabled)
{
	if (*(unsigned int*) &p_enabled == 0) {
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
