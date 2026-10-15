#include "CPVGWnd.h"

#include <stddef.h>

#include "CPVWnd.h"
#include "CWnd.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Platform/Windows/Graphics/CPVSurface.h"
#include "Platform/Windows/Graphics/CSurface.h"
#include "Engine/Graphics/CChangeList.h"
#include "Engine/Graphics/Primitives/CDrawingMark.h"
#include "Platform/Windows/CPlatformServices.h"

// FUNCTION: LEMBALL 0x00432530
void CPVGWnd::OnVisibilityChange()
{
	if (m_parent != NULL) {
		m_gdi->m_renderTarget->m_fullUpdatePending = true;
	}
}

// FUNCTION: LEMBALL 0x00432560
void CPVGWnd::SetDontUpdateRect(const CVSRect& p_rect)
{
	if (m_gdi != NULL && m_gdi->m_renderTarget != NULL) {
		CPVSurface& target = *m_gdi->m_renderTarget;
		target.SetDontUpdateRect(p_rect);
	}
}

// FUNCTION: LEMBALL 0x004453a0
unsigned int CPVGWnd::GetStyle()
{
	return WINDOW_STYLE_DIRECT_SCROLL;
}

// FUNCTION: LEMBALL 0x00465aa0
void CPVGWnd::Clear(int p_colour)
{
	if (g_dwWindowQuitRequested != 0 || m_lifecycleRefs != 1 || m_gdi == NULL) {
		return;
	}
	if (p_colour == WINDOW_CLEAR_DEFAULT_COLOUR) {
		p_colour = 0;
	}

	m_gdi->m_renderTarget->GetCurrDB();
	CVSSize size(m_gdi->m_renderTarget->m_windowRect.m_width, m_gdi->m_renderTarget->m_windowRect.m_height);
	CVSRect clearRect(0, 0, &size);
	CDrawingMark* drawingMark = new CDrawingMark;

	m_gdi->m_renderTarget->GetChangeList()->Reset();
	drawingMark->Draw(m_gdi);

	m_clearRectangle[0].m_bounds = clearRect;
	m_clearRectangle[0].m_colour = p_colour;
	m_clearRectangle[0].Draw(m_gdi);

	if (m_gdi->m_renderTarget->HasBackBuff()) {
		m_backBufferCopy[0].m_x = 0;
		m_backBufferCopy[0].m_y = 0;
		m_backBufferCopy[0].m_destination = clearRect;
		m_backBufferCopy[0].Draw(m_gdi);
	}

	if (m_gdi->m_renderTarget->HasZBuff()) {
		m_zBufferClear[0].m_bounds = clearRect;
		m_zBufferClear[0].m_depth = 0;
		m_zBufferClear[0].Draw(m_gdi);
	}

	m_gdi->Render();
	m_gdi->m_primitiveCount = 0;
	Flush();
	delete drawingMark;
}

// FUNCTION: LEMBALL 0x00465c70
void CPVGWnd::SetInnerWindow(const CVSRect& p_rect)
{
	CPVWnd::SetInnerWindow(p_rect);
}

// FUNCTION: LEMBALL 0x00465c80
void CPVGWnd::_OnZoom(int p_oldZoom)
{
	CWnd::_OnZoom(p_oldZoom);
	if (m_gdi != NULL && m_gdi->m_renderTarget != NULL && m_parent == NULL) {
		m_gdi->m_renderTarget->m_zoom = m_zoom;
	}
}
