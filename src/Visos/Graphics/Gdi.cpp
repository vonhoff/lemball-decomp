#include "Gdi.h"

#include "../Foundation/VsMem.h"
#include "../Foundation/VsRect.h"
#include "GdiDevice.h"
#include "Primitive.h"
#include "VsGdi.h"

#include <new.h>

// GLOBAL: LEMBALL 0x004a1ff4
Primitive* g_pCurrentPrimitive = 0;

// 68K 0x1021184c __ct__4CGDIFRC7CVSRectiP8CSurface
// STUB: LEMBALL 0x00467060
Gdi::Gdi(const VsRect& p_arg0, int p_arg1, Surface* p_arg2)
{
	int i;
	Surface* target;

	if ((int) p_arg0.m_height * (int) p_arg0.m_width > 1) {
		m_primitiveCapacity = p_arg1 + 3;
		m_primitives = (Primitive**) operator new(m_primitiveCapacity * 4);
		target = g_pGdiDevice->AllocateSurface(p_arg0, p_arg2);
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

// 68K 0x102118e6 __dt__4CGDIFv
// FUNCTION: LEMBALL 0x004670d0
Gdi::~Gdi()
{
	operator delete(m_primitives);
	g_pGdiDevice->FreeSurface(m_renderTarget);
}

// 68K 0x1021193c AddToList__4CGDIFP10CPrimitive
// FUNCTION: LEMBALL 0x004670f0
void Gdi::AddToList(Primitive* p_primitive)
{
	m_primitives[m_primitiveCount] = p_primitive;
	m_primitiveCount = m_primitiveCount + 1;
}

// 68K 0x1021197a Render__4CGDIFv
// STUB: LEMBALL 0x00467110
void Gdi::Render()
{
	int offset;
	int i;
	Primitive* primitive;

	if (m_renderTarget->BeginRender()) {
		i = 0;
		offset = 0;
		if (0 < m_primitiveCount) {
			do {
				primitive = *(Primitive**) ((char*) m_primitives + offset);
				g_pCurrentPrimitive = primitive;
				if (CheckValidPointer(primitive)) {
					primitive->Render(this);
				}
				offset += 4;
				i++;
			} while (i < m_primitiveCount);
		}
		m_renderTarget->EndRender();
	}
}
