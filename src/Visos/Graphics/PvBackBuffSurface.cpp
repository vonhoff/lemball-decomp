#include "PvBackBuffSurface.h"

#include "../Foundation/VsMem.h"
#include "../Foundation/VsSize.h"
#include "GdiDevice.h"
#include "VsGdi.h"

#include <new.h>

// 68K 0x10215ebc __ct__18CPVBackBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466440
PvBackBuffSurface::PvBackBuffSurface()
{
	m_allocatedWidth = 0;
	m_buffer = 0;
	m_allocatedHeight = 0;
	m_enabled = 0;
}

// 68K 0x10215f38 __dt__18CPVBackBuffSurfaceFv
// FUNCTION: LEMBALL 0x004664b0
PvBackBuffSurface::~PvBackBuffSurface()
{
}

// 68K 0x10215fb6 HasBackBuff__18CPVBackBuffSurfaceFv
// FUNCTION: LEMBALL 0x004664e0
bool PvBackBuffSurface::HasBackBuff()
{
	if (m_parentSurface != (Surface*) g_pGdiHelperTarget) {
		return m_parentSurface->HasBackBuff();
	}
	return m_enabled;
}

// 68K 0x1021601a FreeBackBuff__18CPVBackBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466510
void PvBackBuffSurface::FreeBackBuff()
{
	if (m_buffer != 0) {
		operator delete(m_buffer);
		m_buffer = 0;
		m_allocatedHeight = 0;
		m_allocatedWidth = 0;
	}
	m_bitmap.Free();
}

// 68K 0x1021607c AllocateBackBuff__18CPVBackBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466540
void PvBackBuffSurface::AllocateBackBuff()
{
	VsSize size;
	int allocatedArea;
	int neededArea;

	size.m_width = m_windowRect.m_width;
	size.m_height = m_windowRect.m_height;
	m_bitmap.SetSize(size, (int) m_reserved40);
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
				(unsigned int) (unsigned short) size.m_width*(unsigned int) (unsigned short) size.m_height);
		}
		if (m_buffer == 0) {
			m_enabled = 0;
		}
		m_bitmap.SetBitsBase(m_buffer, (int) size.m_width);
	}
}

// 68K 0x102161a2 EnableBackBuff__18CPVBackBuffSurfaceFUc
// FUNCTION: LEMBALL 0x00466630
void PvBackBuffSurface::EnableBackBuff(unsigned int p_enabled)
{
	if (p_enabled == 0) {
		FreeBackBuff();
		m_enabled = 0;
		return;
	}
	AllocateBackBuff();
	m_enabled = 1;
}

// 68K 0x10216200 ResizeBackBuff__18CPVBackBuffSurfaceFv
// FUNCTION: LEMBALL 0x00466660
void PvBackBuffSurface::ResizeBackBuff()
{
	AllocateBackBuff();
}
