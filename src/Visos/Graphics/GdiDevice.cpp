#include "GdiDevice.h"

#include "../Foundation/VsMem.h"
#include "../Foundation/VsOStream.h"
#include "VsGdi.h"

#include <new.h>

// GLOBAL: LEMBALL 0x004a2008
GdiDevice* g_pGdiDevice = 0;

// GLOBAL: LEMBALL 0x004a200c
void* g_pGdiHelperTarget = 0;

// 68K 0x10107d0e __ct__10CGDIDeviceFi
// FUNCTION: LEMBALL 0x0046bc00
GdiDevice::GdiDevice(int p_arg0)
{
	int i;
	GdiSurfaceSlot* slot;

	m_surfaceSlots = (GdiSurfaceSlot*) operator new((unsigned int) (p_arg0 * sizeof(GdiSurfaceSlot)));
	m_surfaceCapacity = p_arg0;
	m_reserved08 = 0;
	m_primarySurfaceCount = 0;
	if (0 < p_arg0) {
		i = 0;
		do {
			slot = &m_surfaceSlots[i];
			slot->m_surface = 0;
			slot->m_timer = 0;
			slot->m_parent = 0;
			slot->m_isPrimary = 0;
			slot->m_flushed = 0;
			slot->m_available = 1;
			slot->m_reserved18 = 0;
			++i;
			--p_arg0;
		} while (p_arg0 != 0);
	}
	g_pGdiHelperTarget = 0;
}

// 68K 0x10107dc4 __dt__10CGDIDeviceFv
// FUNCTION: LEMBALL 0x0046bc90
GdiDevice::~GdiDevice()
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

// 68K 0x10107e48 FindFreeSurface__10CGDIDeviceFv
// FUNCTION: LEMBALL 0x0046bce0
int GdiDevice::FindFreeSurface()
{
	int i;
	GdiSurfaceSlot* slot;

	i = 0;
	if (0 < m_surfaceCapacity) {
		slot = m_surfaceSlots;
		do {
			if (slot->m_available != 0) {
				return i;
			}
			++slot;
			++i;
		} while (i < m_surfaceCapacity);
	}
	return -1;
}

// 68K 0x10107e98 AllocateSurface__10CGDIDeviceFRC7CVSRectP8CSurface
// FUNCTION: LEMBALL 0x0046bd10
Surface* GdiDevice::AllocateSurface(const VsRect& p_rect, Surface* p_parentSurface)
{
	int i;
	GdiSurfaceSlot* slot;
	void* storage;
	Surface* surface;

	i = FindFreeSurface();
	if (i < 0) {
		return 0;
	}

	slot = &m_surfaceSlots[i];
	storage = operator new(0x5a0);
	surface = 0;
	if (storage != 0) {
		surface = new (storage) Surface(p_rect, p_parentSurface);
	}
	slot->m_surface = surface;
	slot->m_parent = p_parentSurface;
	slot->m_timer = 0;
	slot->m_flushed = 0;
	slot->m_isPrimary = (void*) p_parentSurface == g_pGdiHelperTarget;
	slot->m_available = 0;
	if (slot->m_isPrimary != 0) {
		++m_primarySurfaceCount;
	}
	return slot->m_surface;
}

// 68K 0x101080fa FreeSurface__10CGDIDeviceFP8CSurface
// FUNCTION: LEMBALL 0x0046bed0
void GdiDevice::FreeSurface(Surface* p_surface)
{
	int i;
	GdiSurfaceSlot* slot;

	i = FindSurface(p_surface);
	slot = &m_surfaceSlots[i];
	if (slot->m_surface != 0) {
		slot->m_surface->~Surface();
		operator delete(slot->m_surface);
	}
	if (slot->m_isPrimary != 0) {
		--m_primarySurfaceCount;
	}
	slot->m_surface = 0;
	slot->m_timer = 0;
	slot->m_parent = 0;
	slot->m_isPrimary = 0;
	slot->m_flushed = 0;
	slot->m_available = 1;
}

// 68K 0x101081bc Sync__10CGDIDeviceFv
// FUNCTION: LEMBALL 0x0046bf60
void GdiDevice::Sync()
{
}

// 68K 0x101081dc FindSurface__10CGDIDeviceFP8CSurface
// FUNCTION: LEMBALL 0x0046bf70
int GdiDevice::FindSurface(Surface* p_surface)
{
	for (int i = 0; i < m_surfaceCapacity; i++) {
		if (m_surfaceSlots[i].m_surface == p_surface) {
			return i;
		}
	}
	return -1;
}

// 68K 0x1010823a Flush__10CGDIDeviceFP8CSurface
// STUB: LEMBALL 0x0046bfd0
void GdiDevice::Flush(Surface* p_surface)
{
	int i;
	GdiSurfaceSlot* slot;

	if (p_surface == 0) {
		return;
	}
	i = FindSurface(p_surface);
	if (i < 0) {
		p_surface->ToScreen((Surface*) g_pGdiHelperTarget);
		return;
	}
	slot = &m_surfaceSlots[i];
	slot->m_flushed = 1;
	p_surface->ToScreen((Surface*) g_pGdiHelperTarget);
}
