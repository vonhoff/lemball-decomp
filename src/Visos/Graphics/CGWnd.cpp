#include "CGWnd.h"

#define WIN32_LEAN_AND_MEAN
#include "../Resources/CResPALETTE.h"
#include "../Target/Graphics/CGraphicsDriver.h"
#include "../Target/Graphics/CGraphicsState.h"
#include "CCursor.h"
#include "CGDI.h"
#include "CGDIDevice.h"
#include "CSurface.h"

#include <new.h>
#include <windows.h>

// FUNCTION: LEMBALL 0x0043a510
void CGWnd::OnCreate()
{
}

// FUNCTION: LEMBALL 0x0043a520
void CGWnd::OnDestroy()
{
}

// FUNCTION: LEMBALL 0x0043a530
void CGWnd::OnSize()
{
}

#include "CPVGWnd.inl"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Graphics/CPVSurface.h"
#include "Visos/Graphics/CPVWnd.h"
#include "Visos/Graphics/CWnd.h"

// FUNCTION: LEMBALL 0x00463b50
CGWnd::CGWnd()
{
	m_refreshWidth = 0;
	m_gdi = 0;
	m_refreshY = 0;
	m_gdiFlags = 0;
	m_refreshX = 0;
	m_paletteResourceId = 0;
}

// FUNCTION: LEMBALL 0x00463bd0
CGWnd::~CGWnd()
{
}

// FUNCTION: LEMBALL 0x00463c20
void CGWnd::Move(const CVsPoint& p_point)
{
	CWnd::Move(p_point);
}

// FUNCTION: LEMBALL 0x00463c30
void CGWnd::_OnCreate()
{
	CSurface* parentSurface;
	CSurface* target;
	short originX;
	short originY;
	unsigned int style;

	CPVWnd::_OnCreate();
	if (m_parent == 0) {
		parentSurface = (CSurface*) g_pGdiHelperTarget;
	}
	else {
		parentSurface = ((CGWnd*) m_parent)->m_gdi->m_renderTarget;
	}

	CVsRect localRect(m_rect);
	localRect.m_x = m_relativeTopLeft.m_x;
	localRect.m_y = m_relativeTopLeft.m_y;
	if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
		localRect.m_width = m_innerRect.m_width;
		localRect.m_height = m_innerRect.m_height;
		CVsPoint* innerOrigin = &m_innerRect;
		localRect.m_x = innerOrigin->m_x;
		localRect.m_y = innerOrigin->m_y;
		localRect.m_x += m_relativeTopLeft.m_x;
		localRect.m_y += m_relativeTopLeft.m_y;
	}
	if (m_parent == 0) {
		localRect.m_width = (short) ((int) localRect.m_width / (int) m_zoom);
		localRect.m_height = (short) ((int) localRect.m_height / (int) m_zoom);
	}

	CGDI** gdi = &m_gdi;
	*gdi = new CGDI(localRect, m_gdiFlags, parentSurface);

	target = (*gdi)->m_renderTarget;
	if (m_parent == 0) {
		target->m_zoom = (short) m_zoom;
	}
	CVsPoint* innerOrigin = &m_innerRect;
	originX = innerOrigin->m_x;
	originY = innerOrigin->m_y;
	if (m_parent != 0) {
		CVsPoint* parentOrigin = &m_parent->m_relativeTopLeft;
		originX = (short) (originX - parentOrigin->m_x);
		originY = (short) (originY - parentOrigin->m_y);
	}
	CPVSurface* surface = (CPVSurface*) ((char*) &target->m_relOriginX - FIELD_OFFSET(CPVSurface, m_relOriginX));
	surface->m_relOriginX = originX;
	surface->m_relOriginY = originY;
	style = GetStyle();
	if ((style & 0x40000000) != 0 && m_nativeWindow != 0) {
		style = GetWindowLongA((HWND) m_nativeWindow, -16);
		if ((style & 0x40000000) != 0) {
			CVsPoint* createOrigin = &m_createRect->m_relativeTopLeft;
			localRect.m_x = (short) (localRect.m_x - createOrigin->m_x);
			localRect.m_y = (short) (localRect.m_y - createOrigin->m_y);
			CSurface* presentTarget = (*gdi)->m_renderTarget;
			presentTarget->m_presentX = localRect.m_x;
			presentTarget->m_presentY = localRect.m_y;
			m_createRect->SetDontUpdateRect(localRect);
		}
	}
	unsigned int directScroll = 1;
	if (g_pTargetGraphicsSystem->m_driverMode == 3) {
		directScroll = 0;
	}
	target->m_directScroll = directScroll;
}

// FUNCTION: LEMBALL 0x00463df0
void CGWnd::_OnDestroy()
{
	CGDI* gdi;
	unsigned int style;

	gdi = m_gdi;
	if (gdi != 0) {
		gdi->~CGDI();
		operator delete(gdi);
		m_gdi = 0;
	}
	style = GetStyle();
	if ((style & 0x40000000) != 0 && m_nativeWindow != 0) {
		if ((GetWindowLongA((HWND) m_nativeWindow, GWL_STYLE) & 0x40000000) != 0) {
			CVsRect emptyRect(0, 0, 0, 0);
			m_createRect->SetDontUpdateRect(emptyRect);
		}
	}
	CPVWnd::_OnDestroy();
}

// FUNCTION: LEMBALL 0x00463e70
void CGWnd::_OnSize()
{
	CSurface* target;
	CVsPoint* innerOrigin;
	CVsPoint* parentOrigin;
	short relX;
	short relY;

	CPVWnd::_OnSize();
	if (m_gdi == 0) {
		return;
	}
	CVsSize size(m_rect);
	if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
		size.m_width = m_innerRect.m_width;
		size.m_height = m_innerRect.m_height;
	}
	target = m_gdi->m_renderTarget;
	if (m_parent == 0) {
		size.m_width = (short) ((int) size.m_width / (int) m_zoom);
		size.m_height = (short) ((int) size.m_height / (int) m_zoom);
	}
	target->Resize(size);
	if (this != (CGWnd*) -16) {
		innerOrigin = (CVsPoint*) &m_innerRect.m_x;
	}
	else {
		innerOrigin = 0;
	}
	relX = innerOrigin->m_x;
	relY = innerOrigin->m_y;
	if (m_parent != 0) {
		parentOrigin = &m_parent->m_relativeTopLeft;
		relX = (short) (relX - parentOrigin->m_x);
		relY = (short) (relY - parentOrigin->m_y);
	}
	CPVSurface* surface = (CPVSurface*) ((char*) &target->m_relOriginX - FIELD_OFFSET(CPVSurface, m_relOriginX));
	surface->m_relOriginX = relX;
	surface->m_relOriginY = relY;
}

// FUNCTION: LEMBALL 0x00463f30
void CGWnd::_OnMove()
{
	CPVWnd::_OnMove();
	m_gdi->m_renderTarget->Move(m_relativeTopLeft);
}

// FUNCTION: LEMBALL 0x00463f60
void CGWnd::OnPaint(const CVsRect& p_rect)
{
}

// FUNCTION: LEMBALL 0x00463f70
int CGWnd::ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	PAINTSTRUCT paint;
	short paintWidth;
	short paintX;
	short paintY;
	short paintHeight;

	switch (p_message) {
	case WM_PAINT:
		switch (g_pTargetGraphicsSystem->m_driverMode) {
		case 4:
		case 5:
			return DefWindowProcA((HWND) g_pTargetGraphicsDriver->m_window, p_message, p_wParam, p_lParam);
		default: {
			HDC(WINAPI * beginPaint)(HWND, LPPAINTSTRUCT) = BeginPaint;
			beginPaint((HWND) m_nativeWindow, &paint);
			paintX = (short) paint.rcPaint.left;
			paintWidth = (short) ((short) paint.rcPaint.right - (unsigned short) paint.rcPaint.left);
			paintY = (short) paint.rcPaint.top;
			paintHeight = (short) ((short) paint.rcPaint.bottom - (unsigned short) paint.rcPaint.top);
			if ((int) paintHeight * (int) paintWidth != 0) {
				if (paintX < (short) m_refreshX) {
					m_refreshWidth = (unsigned short) (m_refreshWidth + (m_refreshX - (unsigned short) paintX));
					m_refreshX = (unsigned short) paintX;
				}
				if ((short) (m_refreshWidth + m_refreshX) < (short) ((unsigned short) paintX + paintWidth)) {
					m_refreshWidth = (unsigned short) ((paintWidth - m_refreshX) + (unsigned short) paintX);
				}
				if (paintY < (short) m_refreshY) {
					m_refreshHeight = (unsigned short) (m_refreshHeight + (m_refreshY - (unsigned short) paintY));
					m_refreshY = (unsigned short) paintY;
				}
				if ((short) (m_refreshHeight + m_refreshY) < (short) ((unsigned short) paintY + paintHeight)) {
					m_refreshHeight = (unsigned short) ((paintHeight - m_refreshY) + (unsigned short) paintY);
				}
			}
			EndPaint((HWND) m_nativeWindow, &paint);
			return 0;
		}
		}
	case WM_ACTIVATEAPP:
		if (p_wParam != 0 && m_gdi != 0) {
			m_gdi->m_renderTarget->SetDefaultCtable();
		}
		break;
	case 0x311:
		paintWidth = m_rect.m_width;
		paintHeight = m_rect.m_height;
		if ((int) paintHeight * (int) paintWidth != 0) {
			if (0 < (short) m_refreshX) {
				m_refreshWidth = (unsigned short) (m_refreshWidth + m_refreshX);
				m_refreshX = 0;
			}
			if ((short) (m_refreshWidth + m_refreshX) < m_rect.m_width) {
				m_refreshWidth = (unsigned short) (m_rect.m_width - m_refreshX);
			}
			if (0 < (short) m_refreshY) {
				m_refreshHeight = (unsigned short) (m_refreshHeight + m_refreshY);
				m_refreshY = 0;
			}
			if ((short) (m_refreshHeight + m_refreshY) < m_rect.m_height) {
				m_refreshHeight = (unsigned short) (m_rect.m_height - m_refreshY);
			}
		}
		break;
	}
	if (m_nativeWindow != 0) {
		return DefWindowProcA((HWND) m_nativeWindow, p_message, p_wParam, p_lParam);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00464190
void CGWnd::Render()
{
	CVsRect paintRect;
	if (m_lifecycleRefs == 0 || m_active == 0) {
		return;
	}
	OnPaint(paintRect);
	m_gdi->Render();
	m_gdi->m_primitiveCount = 0;
	void** childNode = (void**) m_childList;
	for (;;) {
		if (childNode == 0) {
			break;
		}
		((CGWnd*) childNode[0])->Render();
		childNode = (void**) childNode[1];
	}
	if (m_parent == 0 && g_pCursor != 0) {
		g_pCursor->Draw(this);
		m_gdi->Render();
		m_gdi->m_primitiveCount = 0;
	}
}

// FUNCTION: LEMBALL 0x00464220
void CGWnd::Flush()
{
	HDC dc;
	void** childNode;

	if (m_lifecycleRefs == 0) {
		return;
	}
	if (m_nativeWindow != 0) {
		dc = GetDC((HWND) m_nativeWindow);
		EnterCriticalSection((CRITICAL_SECTION*) ((CSurface*) g_pGdiHelperTarget)->m_lock);
		((CSurface*) g_pGdiHelperTarget)->SetWindowPtr(dc);
	}
	childNode = (void**) m_childList;
	for (;;) {
		if (childNode == 0) {
			break;
		}
		((CGWnd*) childNode[0])->Flush();
		childNode = (void**) childNode[1];
	}
	m_gdi->m_renderTarget->Flush();
	if (m_nativeWindow != 0) {
		ReleaseDC((HWND) m_nativeWindow, dc);
		LeaveCriticalSection((CRITICAL_SECTION*) ((CSurface*) g_pGdiHelperTarget)->m_lock);
	}
}

// FUNCTION: LEMBALL 0x004642c0
void CGWnd::Refresh(CVsRect* p_rect)
{
	unsigned short height;
	unsigned short width;
	unsigned short innerX;
	short innerWidth;
	short innerHeight;
	CVsRect damage;

	Render();
	height = m_refreshHeight;
	width = m_refreshWidth;
	if ((int) (short) height * (int) (short) width > 0) {
		damage.m_x = (short) m_refreshX;
		innerWidth = m_innerRect.m_width;
		damage.m_y = (short) m_refreshY;
		innerHeight = m_innerRect.m_height;
		damage.m_width = (short) width;
		damage.m_height = (short) height;
		if ((int) innerWidth * (int) innerHeight != 0) {
			innerX = (unsigned short) m_innerRect.m_x;
			if ((short) damage.m_x < (short) innerX) {
				damage.m_width = (short) (width + (damage.m_x - innerX));
				damage.m_x = (short) innerX;
			}
			innerWidth = (short) (innerX + innerWidth);
			if (innerWidth < (short) (damage.m_x + damage.m_width)) {
				damage.m_width = (short) (innerWidth - damage.m_x);
			}
			width = (unsigned short) m_innerRect.m_y;
			if ((short) damage.m_y < (short) width) {
				damage.m_height = (short) (height + (damage.m_y - width));
				damage.m_y = (short) width;
			}
			innerHeight = (short) (width + innerHeight);
			if (innerHeight < (short) (damage.m_y + damage.m_height)) {
				damage.m_height = (short) (innerHeight - damage.m_y);
			}
			if ((short) damage.m_width < 1 || (short) damage.m_height < 1) {
				damage.m_height = 0;
				damage.m_width = 0;
				damage.m_y = 0;
				damage.m_x = 0;
			}
		}
		if ((int) (short) damage.m_height * (int) (short) damage.m_width > 0) {
			damage.m_x = (short) (damage.m_x - m_innerRect.m_x);
			damage.m_y = (short) (damage.m_y - m_innerRect.m_y);
			m_gdi->m_renderTarget->AddToChangeList(&damage);
		}
	}
	Flush();
	m_refreshWidth = 0;
	m_refreshHeight = 0;
	m_refreshX = 0;
	m_refreshY = 0;
}

// FUNCTION: LEMBALL 0x00464440
void CGWnd::Create(const CVsRect& p_rect, CPVWnd* p_parent, char* p_title, unsigned long p_paletteId)
{
	CWnd::Create(p_rect, p_parent, p_title);
	AttachPalette(p_paletteId);
}

// FUNCTION: LEMBALL 0x00464470
void CGWnd::Create(const CVsRect& p_rect, CPVWnd* p_parent, char* p_title)
{
	Create(p_rect, p_parent, p_title, 0);
}
