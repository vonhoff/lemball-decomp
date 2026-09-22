#include "CGdi.h"

#include "../Foundation/CVsRect.h"
#include "../Foundation/VsMem.h"
#include "CGdiDevice.h"
#include "CPrimitive.h"
#include "CSurface.h"

// GLOBAL: LEMBALL 0x004a1ff4
CPrimitive* g_pCurrentPrimitive = 0;

// FUNCTION: LEMBALL 0x00467060
CGdi::CGdi(const CVsRect& p_rect, int p_primitiveCapacity, CSurface* p_parentSurface)
{
	int i;
	CSurface* target;

	if ((int) p_rect.m_height * (int) p_rect.m_width > 1) {
		m_primitiveCapacity = p_primitiveCapacity + 3;
		m_primitives = (CPrimitive**) operator new(m_primitiveCapacity * 4);
		target = g_pGdiDevice->AllocateSurface(p_rect, p_parentSurface);
		i = 0;
		m_renderTarget = target;
		m_primitiveCount = i;
		if (0 < m_primitiveCapacity) {
			do {
				m_primitives[i] = 0;
				i = i + 1;
			} while (i < m_primitiveCapacity);
		}
	}
}

// FUNCTION: LEMBALL 0x004670d0
CGdi::~CGdi()
{
	operator delete(m_primitives);
	g_pGdiDevice->FreeSurface(m_renderTarget);
}

// FUNCTION: LEMBALL 0x004670f0
void CGdi::AddToList(CPrimitive* p_primitive)
{
	m_primitives[m_primitiveCount] = p_primitive;
	m_primitiveCount = m_primitiveCount + 1;
}

// FUNCTION: LEMBALL 0x00467110
void CGdi::Render()
{
	int i;

	if (m_renderTarget->BeginRender()) {
		i = 0;
		if (0 < m_primitiveCount) {
			do {
				g_pCurrentPrimitive = m_primitives[i];
				if (CheckValidPointer(m_primitives[i])) {
					m_primitives[i]->Render(this);
				}
				i++;
			} while (i < m_primitiveCount);
		}
		m_renderTarget->EndRender();
	}
}
