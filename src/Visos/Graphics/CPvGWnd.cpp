#include "CPvGWnd.h"

#include "CGdi.h"
#include "Visos/Graphics/CPvSurface.h"
#include "Visos/Graphics/CPvWnd.h"
#include "Visos/Graphics/CWnd.h"
#include "VsGdi.h"

// FUNCTION: LEMBALL 0x00432530
void CPvGWnd::OnVisibilityChange()
{
	if (m_parent != 0) {
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
}

// FUNCTION: LEMBALL 0x00432560
void CPvGWnd::SetDontUpdateRect(const CVsRect& p_rect)
{
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		CPvSurface& target = *m_gdi->m_renderTarget;
		target.SetDontUpdateRect(p_rect);
	}
}

// FUNCTION: LEMBALL 0x004453a0
unsigned int CPvGWnd::GetStyle()
{
	return 2147483648;
}

// STUB: LEMBALL 0x00465aa0
void CPvGWnd::Clear(int p_color)
{
}

// FUNCTION: LEMBALL 0x00465c70
void CPvGWnd::SetInnerWindow(const CVsRect& p_rect)
{
	CPvWnd::SetInnerWindow(p_rect);
}

// FUNCTION: LEMBALL 0x00465c80
void CPvGWnd::InternalOnZoom(int p_oldZoom)
{
	CWnd::InternalOnZoom(p_oldZoom);
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0 && m_parent == 0) {
		m_gdi->m_renderTarget->CPvSurface::m_zoom = m_zoom;
	}
}
