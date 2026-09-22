#include "CGdiDevice.h"

#include "../Animation/CTimeStat.h"
#include "../Foundation/CDebugOStream.h"
#include "../Foundation/VsTime.h"
#include "Visos/Foundation/CVsOStream.h"
#include "VsGdi.h"

// GLOBAL: LEMBALL 0x004a2008
CGdiDevice* g_pGdiDevice = 0;

// GLOBAL: LEMBALL 0x004a200c
void* g_pGdiHelperTarget = 0;

// FUNCTION: LEMBALL 0x0046bc00
CGdiDevice::CGdiDevice(int p_arg0)
{
	int i;

	m_surfaceSlots = (GdiSurfaceSlot*) operator new((unsigned int) (p_arg0 * sizeof(GdiSurfaceSlot)));
	m_surfaceCapacity = p_arg0;
	m_reserved08 = 0;
	m_primarySurfaceCount = 0;
	if (0 < p_arg0) {
		i = 0;
		do {
			m_surfaceSlots[i].m_surface = 0;
			m_surfaceSlots[i].m_timer = 0;
			m_surfaceSlots[i].m_parent = 0;
			m_surfaceSlots[i].m_isPrimary = 0;
			m_surfaceSlots[i].m_flushed = 0;
			m_surfaceSlots[i].m_available = 1;
			m_surfaceSlots[i].m_reserved18 = 0;
			++i;
			--p_arg0;
		} while (p_arg0 != 0);
	}
	g_pGdiHelperTarget = 0;
}

// FUNCTION: LEMBALL 0x0046bc90
CGdiDevice::~CGdiDevice()
{
	int i;

	i = 0;
	if (0 < m_surfaceCapacity) {
		do {
			if (m_surfaceSlots[i].m_available == 0) {
				*g_pErrorOutput << "Trying to delete device when a surface has not been free'd\n";
			}
			++i;
		} while (i < m_surfaceCapacity);
	}
	operator delete(m_surfaceSlots);
}

// FUNCTION: LEMBALL 0x0046bce0
int CGdiDevice::FindFreeSurface()
{
	for (int i = 0; i < m_surfaceCapacity; ++i) {
		if (m_surfaceSlots[i].m_available != 0) {
			return i;
		}
	}
	return -1;
}

// FUNCTION: LEMBALL 0x0046bd10
CSurface* CGdiDevice::AllocateSurface(const CVsRect& p_rect, CSurface* p_parentSurface)
{
	int i;

	i = FindFreeSurface();
	if (i == -1) {
		return 0;
	}

	char buffer[0x20];
	CDebugOStream stream(buffer, sizeof(buffer));
	stream << "Surface" << i;

	m_surfaceSlots[i].m_surface = new CSurface(p_rect, p_parentSurface);
	m_surfaceSlots[i].m_parent = p_parentSurface;
	m_surfaceSlots[i].m_isPrimary = (void*) p_parentSurface == g_pGdiHelperTarget;
	m_surfaceSlots[i].m_flushed = 0;
	m_surfaceSlots[i].m_available = 0;
	if (m_surfaceSlots[i].m_isPrimary != 0) {
		m_surfaceSlots[i].m_timer = 0;
		++m_primarySurfaceCount;
	}
	else {
		m_surfaceSlots[i].m_timer = 0;
	}
	return m_surfaceSlots[i].m_surface;
}

// FUNCTION: LEMBALL 0x0046bed0
void CGdiDevice::FreeSurface(CSurface* p_surface)
{
	int i;
	CSurface* surface;

	i = FindSurface(p_surface);
	surface = m_surfaceSlots[i].m_surface;
	if (surface != 0) {
		delete surface;
	}
	if (m_surfaceSlots[i].m_isPrimary != 0) {
		--m_primarySurfaceCount;
	}
	m_surfaceSlots[i].m_surface = 0;
	m_surfaceSlots[i].m_timer = 0;
	m_surfaceSlots[i].m_parent = 0;
	m_surfaceSlots[i].m_isPrimary = 0;
	m_surfaceSlots[i].m_flushed = 0;
	m_surfaceSlots[i].m_available = 1;
}

// FUNCTION: LEMBALL 0x0046bf60
void CGdiDevice::Sync()
{
}

// FUNCTION: LEMBALL 0x0046bf70
int CGdiDevice::FindSurface(CSurface* p_surface)
{
	for (int i = 0; i < m_surfaceCapacity; i++) {
		if (m_surfaceSlots[i].m_surface == p_surface) {
			return i;
		}
	}
	return -1;
}

// FUNCTION: LEMBALL 0x0046bfd0
void CGdiDevice::Flush(CSurface* p_surface)
{
	int i;
	CTimeStat* timer;

	i = FindSurface(p_surface);
	m_surfaceSlots[i].m_flushed = 1;
	timer = m_surfaceSlots[i].m_timer;
	if (timer != 0) {
		if (timer->m_timingActive != 0) {
			timer->Update(CurrentMilliTimer() - timer->m_timingStart);
			timer->m_timingActive = 0;
		}
		timer = m_surfaceSlots[i].m_timer;
		timer->m_timingStart = CurrentMilliTimer();
		timer->m_timingActive = 1;
	}
	m_surfaceSlots[i].m_surface->ToScreen((CSurface*) g_pGdiHelperTarget);
}
