#include "PvGWnd.h"

#include "Bitmap.h"
#include "CopyToBackBuff.h"
#include "Line.h"

#include <new.h>

// 68K 0x1010b56c OnVisibilityChange__7CPVGWndFv
// STUB: LEMBALL 0x00432530
void PvGWnd::OnVisibilityChange()
{
}

// 68K 0x1010b5e0 SetDontUpdateRect__7CPVGWndFRC7CVSRect
// STUB: LEMBALL 0x00432560
void PvGWnd::SetDontUpdateRect(const VsRect& p_rect)
{
}

// FUNCTION: LEMBALL 0x0043a510
void PvGWnd::Dummy3c()
{
}

// FUNCTION: LEMBALL 0x0043a520
void PvGWnd::OnDestroy()
{
}

// FUNCTION: LEMBALL 0x0043a530
void PvGWnd::BaseOnSize()
{
}

// 68K 0x1010b5ba GetStyle__7CPVGWndFv
// FUNCTION: LEMBALL 0x004453a0
unsigned int PvGWnd::GetStyle()
{
	return 2147483648;
}

// STUB: LEMBALL 0x00463b50
PvGWnd::PvGWnd()
{
	new (m_renderState0) Line();
	new (m_renderState1) Bitmap();
	new (m_renderState2) CopyToBackBuff();
	m_refreshHeight = 0;
	m_refreshWidth = 0;
	m_gdi = 0;
	m_refreshY = 0;
	m_gdiFlags = 0;
	m_refreshX = 0;
	m_paletteResourceId = 0;
}

// 68K 0x10216626 Clear__7CPVGWndFi
// STUB: LEMBALL 0x00465aa0
void PvGWnd::Clear(int p_color)
{
}

// 68K 0x102168f0 SetInnerWindow__7CPVGWndFRC7CVSRect
// STUB: LEMBALL 0x00465c70
void PvGWnd::SetInnerWindow(const VsRect& p_rect)
{
}

// 68K 0x1021692e _OnZoom__7CPVGWndFi
// STUB: LEMBALL 0x00465c80
void PvGWnd::BaseOnZoom(int p_oldZoom)
{
}
