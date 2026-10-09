#include "CPVBackBuffSurface.h"

#include "CGDIDevice.h"
#include "CPVGDIBitmap.h"
#include "CSurface.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00466440
CPVBackBuffSurface::CPVBackBuffSurface()
{
	m_allocatedWidth = 0;
	m_buffer = NULL;
	m_allocatedHeight = 0;
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x004664b0
CPVBackBuffSurface::~CPVBackBuffSurface()
{
}

// FUNCTION: LEMBALL 0x004664e0
bool CPVBackBuffSurface::HasBackBuff()
{
	if (m_parentSurface != g_pGdiHelperTarget) {
		return m_parentSurface->HasBackBuff();
	}
	return m_enabled;
}

// FUNCTION: LEMBALL 0x00466510
void CPVBackBuffSurface::FreeBackBuff()
{
	if (m_buffer != NULL) {
		delete[] m_buffer;
		m_buffer = NULL;
		m_allocatedHeight = 0;
		m_allocatedWidth = 0;
	}
	m_bitmap.Free();
}

// FUNCTION: LEMBALL 0x00466540
void CPVBackBuffSurface::AllocateBackBuff()
{
	const CVSSize& dimensions = m_windowRect;
	short height = dimensions.m_height;
	short width = dimensions.m_width;
	CVSRect size(0, 0, width, height);
	int allocatedArea;
	int neededArea;

	const CVSSize& actualSize = m_bitmap.SetSize(size, m_reserved40);
	size.m_width = actualSize.m_width;
	size.m_height = actualSize.m_height;
	allocatedArea = (unsigned int) m_allocatedWidth * m_allocatedHeight;
	neededArea = size.m_height * size.m_width;
	if (allocatedArea < neededArea) {
		FreeBackBuff();
	}
	if (m_windowRect.m_width * m_windowRect.m_height != 0) {
		if (m_buffer == NULL) {
			m_allocatedWidth = size.m_width;
			m_allocatedHeight = size.m_height;
			m_buffer = new unsigned char[(unsigned int) (unsigned short) size.m_height *
										 (unsigned int) (unsigned short) size.m_width];
		}
		if (m_buffer == NULL) {
			m_enabled = 0;
		}
		m_bitmap.SetBitsBase(m_buffer, size.m_width);
	}
}

// FUNCTION: LEMBALL 0x00466630
void CPVBackBuffSurface::EnableBackBuff(unsigned int p_enabled)
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
void CPVBackBuffSurface::ResizeBackBuff()
{
	AllocateBackBuff();
}
