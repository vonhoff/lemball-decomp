#include "GWnd.h"

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
	return 0;
}

// 68K 0x1010ac9c Render__5CGWndFv
// STUB: LEMBALL 0x00464190
void GWnd::Render()
{
}

// 68K 0x1010ad7a Flush__5CGWndFv
// STUB: LEMBALL 0x00464220
void GWnd::Flush()
{
}

// 68K 0x1010adb8 Refresh__5CGWndFP7CVSRect
// STUB: LEMBALL 0x004642c0
void GWnd::Refresh(VsRect* p_rect)
{
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
}
