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
void CPVGWnd::SetDontUpdateRect(const CVSRect& p_rect)
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

#include "CDrawingMark.h"
#include "Visos/Foundation/CChangeList.h"
#include "Visos/Target/System/CPlatformServices.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00465aa0
void CPVGWnd::Clear(int p_colour)
{
	if (g_dwWindowQuitRequested != 0 || m_lifecycleRefs != 1 || m_gdi == 0) {
		return;
	}
	if (p_colour == -1) {
		p_colour = 0;
	}

	m_gdi->m_renderTarget->GetCurrDB();
	CVSRect clearRect(m_gdi->m_renderTarget->CPVSurface::m_windowRect);
	clearRect.m_x = clearRect.m_y = 0;
	CDrawingMark* drawingMark = new CDrawingMark;

	m_gdi->m_renderTarget->GetChangeList()->Reset();
	drawingMark->Draw(m_gdi);

	memcpy(&m_clearRectangle[0].m_bounds.m_width, &clearRect.m_width, sizeof(clearRect.m_width));
	memcpy(&m_clearRectangle[0].m_bounds.m_height, &clearRect.m_height, sizeof(clearRect.m_height));
	memcpy(&m_clearRectangle[0].m_bounds.m_x, &clearRect.m_x, sizeof(clearRect.m_x));
	memcpy(&m_clearRectangle[0].m_bounds.m_y, &clearRect.m_y, sizeof(clearRect.m_y));
	m_clearRectangle[0].m_colour = p_colour;
	m_clearRectangle[0].Draw(m_gdi);

	if (m_gdi->m_renderTarget->HasBackBuff()) {
		m_backBufferCopy[0].m_x = 0;
		m_backBufferCopy[0].m_y = 0;
		memcpy(&m_backBufferCopy[0].m_destination.m_width, &clearRect.m_width, sizeof(clearRect.m_width));
		memcpy(&m_backBufferCopy[0].m_destination.m_height, &clearRect.m_height, sizeof(clearRect.m_height));
		memcpy(&m_backBufferCopy[0].m_destination.m_x, &clearRect.m_x, sizeof(clearRect.m_x));
		memcpy(&m_backBufferCopy[0].m_destination.m_y, &clearRect.m_y, sizeof(clearRect.m_y));
		m_backBufferCopy[0].Draw(m_gdi);
	}

	if (m_gdi->m_renderTarget->HasZBuff()) {
		memcpy(&m_zBufferClear[0].m_bounds.m_width, &clearRect.m_width, sizeof(clearRect.m_width));
		memcpy(&m_zBufferClear[0].m_bounds.m_height, &clearRect.m_height, sizeof(clearRect.m_height));
		memcpy(&m_zBufferClear[0].m_bounds.m_x, &clearRect.m_x, sizeof(clearRect.m_x));
		memcpy(&m_zBufferClear[0].m_bounds.m_y, &clearRect.m_y, sizeof(clearRect.m_y));
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
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0 && m_parent == 0) {
		m_gdi->m_renderTarget->CPVSurface::m_zoom = m_zoom;
	}
}
