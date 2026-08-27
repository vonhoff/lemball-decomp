#include "GWnd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../Resources/ResPalette.h"
#include "../Target/TargetGraphicsDriver.h"
#include "../Target/TargetGraphicsSystemState.h"
#include "Cursor.h"
#include "Gdi.h"
#include "GdiDevice.h"
#include "VsGdi.h"

#include <new.h>

// 68K 0x1010b3ca OnMove__5CGWndFv
// FUNCTION: LEMBALL 0x0043a4d0 FOLDED
void GWnd::OnMove()
{
}

// 68K 0x1010a70a __dt__5CGWndFv
// STUB: LEMBALL 0x00463bd0
GWnd::~GWnd()
{
}

// 68K 0x1010b3e6 Move__5CGWndFRC8CVSPoint
// FUNCTION: LEMBALL 0x00463c20
void GWnd::Move(const VsPoint& p_point)
{
	Wnd::Move(p_point);
}

// 68K 0x1010a808 _OnCreate__5CGWndFv
// STUB: LEMBALL 0x00463c30
void GWnd::OnCreate()
{
	VsRect localRect;
	Surface* parentSurface;
	Surface* target;
	void* storage;
	short originX;
	short originY;
	unsigned int style;

	PvWnd::OnCreate();
	parentSurface = (Surface*) g_pGdiHelperTarget;
	if (m_parent != 0) {
		parentSurface = ((GWnd*) m_parent)->m_gdi->m_renderTarget;
	}

	localRect.m_x = m_relativeTopLeft.m_x;
	localRect.m_y = m_relativeTopLeft.m_y;
	localRect.m_width = m_rect.m_width;
	localRect.m_height = m_rect.m_height;
	if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
		localRect.m_x = (short) (m_innerRect.m_x + m_relativeTopLeft.m_x);
		localRect.m_y = (short) (m_innerRect.m_y + m_relativeTopLeft.m_y);
		localRect.m_width = m_innerRect.m_width;
		localRect.m_height = m_innerRect.m_height;
	}
	if (m_parent == 0 && m_zoom != 0) {
		localRect.m_width = (short) ((int) localRect.m_width / (int) m_zoom);
		localRect.m_height = (short) ((int) localRect.m_height / (int) m_zoom);
	}

	storage = operator new(0x10);
	if (storage == 0) {
		m_gdi = 0;
	}
	else {
		m_gdi = new (storage) Gdi(localRect, m_gdiFlags, parentSurface);
	}

	target = 0;
	if (m_gdi != 0) {
		target = m_gdi->m_renderTarget;
	}
	if (target != 0 && m_parent == 0) {
		target->m_zoom = (short) m_zoom;
	}
	originX = m_innerRect.m_x;
	originY = m_innerRect.m_y;
	if (m_parent != 0) {
		originX = (short) (originX - m_parent->m_relativeTopLeft.m_x);
		originY = (short) (originY - m_parent->m_relativeTopLeft.m_y);
	}
	if (target != 0) {
		target->m_relOriginX = originX;
		target->m_relOriginY = originY;
		style = GetStyle();
		if ((style & 0x40000000) != 0 && m_nativeWindow != 0) {
			style = GetWindowLongA((HWND) m_nativeWindow, -16);
			if ((style & 0x40000000) != 0 && m_createRect != 0) {
				localRect.m_x = (short) (localRect.m_x - m_createRect->m_relativeTopLeft.m_x);
				localRect.m_y = (short) (localRect.m_y - m_createRect->m_relativeTopLeft.m_y);
				target->m_presentX = localRect.m_x;
				target->m_presentY = localRect.m_y;
				m_createRect->SetDontUpdateRect(localRect);
			}
		}
		target->m_directScroll = (unsigned int) (g_pTargetGraphicsSystem->m_driverMode != 3);
	}
}

// 68K 0x1010aa18 _OnDestroy__5CGWndFv
// STUB: LEMBALL 0x00463df0
void GWnd::OnDestroy()
{
}

// 68K 0x1010aa64 _OnSize__5CGWndFv
// STUB: LEMBALL 0x00463e70
void GWnd::OnSize()
{
}

// 68K 0x1010ab98 OnPaint__5CGWndFRC7CVSRect
// FUNCTION: LEMBALL 0x00463f60
void GWnd::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x1010abbe ProcessOtherMessages__5CGWndFR11EventRecords
// STUB: LEMBALL 0x00463f70
int GWnd::ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	PAINTSTRUCT paint;
	short paintX;
	short paintY;
	short paintWidth;
	short paintHeight;

	if (p_message == WM_PAINT) {
		if (g_pTargetGraphicsSystem->m_driverMode > 3 && g_pTargetGraphicsSystem->m_driverMode < 6) {
			return DefWindowProcA((HWND) g_pTargetGraphicsDriver->m_window, WM_PAINT, p_wParam, p_lParam);
		}
		BeginPaint((HWND) m_nativeWindow, &paint);
		paintX = (short) paint.rcPaint.left;
		paintY = (short) paint.rcPaint.top;
		paintWidth = (short) ((short) paint.rcPaint.right - (unsigned short) paint.rcPaint.left);
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
	if (p_message == WM_ACTIVATEAPP) {
		if (p_wParam != 0 && m_gdi != 0) {
			m_gdi->m_renderTarget->SetDefaultCtable();
		}
	}
	else if (p_message == 0x311) {
		if ((int) m_rect.m_height * (int) m_rect.m_width != 0) {
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
	}
	if (m_nativeWindow == 0) {
		return 0;
	}
	return DefWindowProcA((HWND) m_nativeWindow, p_message, p_wParam, p_lParam);
}

// 68K 0x1010ac9c Render__5CGWndFv
// STUB: LEMBALL 0x00464190
void GWnd::Render()
{
	VsRect paintRect;
	void** childNode;

	paintRect.m_width = 0;
	paintRect.m_height = 0;
	paintRect.m_x = 0;
	paintRect.m_y = 0;
	if (m_lifecycleRefs != 0 && m_active != 0) {
		OnPaint(paintRect);
		if (m_gdi != 0) {
			m_gdi->Render();
			m_gdi->m_primitiveCount = 0;
		}
		childNode = (void**) m_childList;
		while (childNode != 0) {
			((GWnd*) childNode[0])->Render();
			childNode = (void**) childNode[1];
		}
		if (m_parent == 0 && g_pCursor != 0) {
			g_pCursor->Draw(this);
			if (m_gdi != 0) {
				m_gdi->Render();
				m_gdi->m_primitiveCount = 0;
			}
		}
	}
}

// 68K 0x1010ad7a Flush__5CGWndFv
// STUB: LEMBALL 0x00464220
void GWnd::Flush()
{
	HDC dc;
	void** childNode;
	Surface* helper;

	if (m_lifecycleRefs == 0) {
		return;
	}
	dc = 0;
	helper = (Surface*) g_pGdiHelperTarget;
	if (m_nativeWindow != 0) {
		dc = GetDC((HWND) m_nativeWindow);
		if (helper != 0) {
			EnterCriticalSection((CRITICAL_SECTION*) helper->m_lock);
			helper->SetWindowPtr(dc);
		}
	}
	childNode = (void**) m_childList;
	while (childNode != 0) {
		((GWnd*) childNode[0])->Flush();
		childNode = (void**) childNode[1];
	}
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		m_gdi->m_renderTarget->Flush();
	}
	if (m_nativeWindow != 0) {
		ReleaseDC((HWND) m_nativeWindow, dc);
		if (helper != 0) {
			LeaveCriticalSection((CRITICAL_SECTION*) helper->m_lock);
		}
	}
}

// 68K 0x1010adb8 Refresh__5CGWndFP7CVSRect
// STUB: LEMBALL 0x004642c0
void GWnd::Refresh(VsRect* p_rect)
{
	Render();
	Flush();
	m_refreshWidth = 0;
	m_refreshHeight = 0;
	m_refreshX = 0;
	m_refreshY = 0;
}

// 68K 0x1010a7ac Create__5CGWndFRC7CVSRectP6CPVWndPcUl
// STUB: LEMBALL 0x00464440
void GWnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title, unsigned long p_paletteId)
{
	Wnd::Create(p_rect, p_parent, p_title);
	AttachPalette(p_paletteId);
}

// 68K 0x1010b4c6 Create__5CGWndFRC7CVSRectP6CPVWndPc
// STUB: LEMBALL 0x00464470
void GWnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title)
{
	Create(p_rect, p_parent, p_title, 0);
}

// 68K 0x1010b30c AttachPalette__5CGWndFUl
// STUB: LEMBALL 0x00464490
void GWnd::AttachPalette(unsigned long p_paletteId)
{
	ResPalette* palette;

	if (p_paletteId == 0) {
		return;
	}
	palette = ResPalette::Load(p_paletteId);
	if (palette == 0) {
		return;
	}
	if (palette->m_loaded == 0) {
		palette->LoadData();
	}
	else {
		palette->m_age = 0;
	}
	palette->m_directUseCount = palette->m_directUseCount + 1;
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		m_gdi->m_renderTarget->AttachPalette(palette);
	}
	palette->m_directUseCount = palette->m_directUseCount - 1;
	palette->UnLoad();
	m_paletteResourceId = p_paletteId;
}
