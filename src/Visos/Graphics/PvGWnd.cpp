#include "PvGWnd.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x1010b56c OnVisibilityChange__7CPVGWndFv
// FUNCTION: LEMBALL 0x00432530
void PvGWnd::OnVisibilityChange()
{
	if (m_parent != 0) {
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
}

// 68K 0x1010b5e0 SetDontUpdateRect__7CPVGWndFRC7CVSRect
// FUNCTION: LEMBALL 0x00432560
void PvGWnd::SetDontUpdateRect(const VsRect& p_rect)
{
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		m_gdi->m_renderTarget->SetDontUpdateRect(p_rect);
	}
}

// 68K 0x1010b5ba GetStyle__7CPVGWndFv
// FUNCTION: LEMBALL 0x004453a0
unsigned int PvGWnd::GetStyle()
{
	return 2147483648;
}

// 68K 0x10216626 Clear__7CPVGWndFi
// STUB: LEMBALL 0x00465aa0
void PvGWnd::Clear(int p_color)
{
}

// 68K 0x102168f0 SetInnerWindow__7CPVGWndFRC7CVSRect
// FUNCTION: LEMBALL 0x00465c70
void PvGWnd::SetInnerWindow(const VsRect& p_rect)
{
	PvWnd::SetInnerWindow(p_rect);
}

// 68K 0x1021692e _OnZoom__7CPVGWndFi
// FUNCTION: LEMBALL 0x00465c80
void PvGWnd::InternalOnZoom(int p_oldZoom)
{
	Wnd::InternalOnZoom(p_oldZoom);
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0 && m_parent == 0) {
		m_gdi->m_renderTarget->PvSurface::m_zoom = m_zoom;
	}
}
