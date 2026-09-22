#include "CPvBackBuffSurface.h"

#include "../Foundation/CVsSize.h"
#include "CGdiDevice.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Graphics/CPvGdiBitmap.h"
#include "VsGdi.h"

// FUNCTION: LEMBALL 0x00466440
CPvBackBuffSurface::CPvBackBuffSurface()
{
	m_allocatedWidth = 0;
	m_buffer = 0;
	m_allocatedHeight = 0;
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x004664b0
CPvBackBuffSurface::~CPvBackBuffSurface()
{
}

// FUNCTION: LEMBALL 0x004664e0
bool CPvBackBuffSurface::HasBackBuff()
{
	if (m_parentSurface != (CSurface*) g_pGdiHelperTarget) {
		return m_parentSurface->HasBackBuff();
	}
	return m_enabled;
}

// FUNCTION: LEMBALL 0x00466510
void CPvBackBuffSurface::FreeBackBuff()
{
	if (m_buffer != 0) {
		operator delete(m_buffer);
		m_buffer = 0;
		m_allocatedHeight = 0;
		m_allocatedWidth = 0;
	}
	m_bitmap.Free();
}

// FUNCTION: LEMBALL 0x00466540
void CPvBackBuffSurface::AllocateBackBuff()
{
	const CVsSize& dimensions = m_windowRect;
	short height = dimensions.m_height;
	short width = dimensions.m_width;
	CVsRect size(0, 0, width, height);
	int allocatedArea;
	int neededArea;

	const CVsSize& actualSize = m_bitmap.SetSize(size, m_reserved40);
	size.m_width = actualSize.m_width;
	size.m_height = actualSize.m_height;
	allocatedArea = (int) m_allocatedWidth * (int) m_allocatedHeight;
	neededArea = (int) size.m_height * (int) size.m_width;
	if (allocatedArea < neededArea) {
		FreeBackBuff();
	}
	if ((int) m_windowRect.m_width * (int) m_windowRect.m_height != 0) {
		if (m_buffer == 0) {
			m_allocatedWidth = (unsigned short) size.m_width;
			m_allocatedHeight = (unsigned short) size.m_height;
			m_buffer = (unsigned char*) operator new(
				(unsigned int) (unsigned short) size.m_height*(unsigned int) (unsigned short) size.m_width);
		}
		if (m_buffer == 0) {
			m_enabled = 0;
		}
		m_bitmap.SetBitsBase(m_buffer, (int) size.m_width);
	}
}

// FUNCTION: LEMBALL 0x00466630
void CPvBackBuffSurface::EnableBackBuff(unsigned int p_enabled)
{
	if (p_enabled == 0) {
		FreeBackBuff();
		m_enabled = 0;
		return;
	}
	AllocateBackBuff();
	m_enabled = 1;
}

// FUNCTION: LEMBALL 0x00466660
void CPvBackBuffSurface::ResizeBackBuff()
{
	AllocateBackBuff();
}
