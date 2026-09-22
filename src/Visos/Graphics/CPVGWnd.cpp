#include "CPVGWnd.h"

#include "CGDI.h"
#include "CSurface.h"
#include "Visos/Graphics/CPVSurface.h"
#include "Visos/Graphics/CPVWnd.h"
#include "Visos/Graphics/CWnd.h"

// FUNCTION: LEMBALL 0x00432530
void CPVGWnd::OnVisibilityChange()
{
	if (m_parent != 0) {
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
}

// FUNCTION: LEMBALL 0x00432560
void CPVGWnd::SetDontUpdateRect(const CVsRect& p_rect)
{
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		CPVSurface& target = *m_gdi->m_renderTarget;
		target.SetDontUpdateRect(p_rect);
	}
}

// FUNCTION: LEMBALL 0x004453a0
unsigned int CPVGWnd::GetStyle()
{
	return 2147483648;
}

// STUB: LEMBALL 0x00465aa0
void CPVGWnd::Clear(int p_color)
{
}

// FUNCTION: LEMBALL 0x00465c70
void CPVGWnd::SetInnerWindow(const CVsRect& p_rect)
{
	CPVWnd::SetInnerWindow(p_rect);
}

// FUNCTION: LEMBALL 0x00465c80
void CPVGWnd::_OnZoom(int p_oldZoom)
{
	CWnd::_OnZoom(p_oldZoom);
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0 && m_parent == 0) {
		m_gdi->m_renderTarget->CPVSurface::m_zoom = m_zoom;
	}
}
