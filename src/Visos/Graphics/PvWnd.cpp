#include "PvWnd.h"

// 68K 0x101049c0 GetSizeStatus__6CPVWndFv
// STUB: LEMBALL 0x004323b0
unsigned int PvWnd::GetSizeStatus()
{
	return 0;
}

// 68K 0x101049ec SetSizeStatus__6CPVWndF13wndSizeStatus
// STUB: LEMBALL 0x004323c0
void PvWnd::SetSizeStatus(unsigned int p_status)
{
}

// 68K 0x10104a28 AddChild__6CPVWndFP6CPVWnd
// STUB: LEMBALL 0x004323d0
void PvWnd::AddChild(class PvWnd* p_child)
{
}

// 68K 0x10104aaa RemoveChild__6CPVWndFP6CPVWnd
// STUB: LEMBALL 0x00432430
void PvWnd::RemoveChild(class PvWnd* p_child)
{
}

// 68K 0x1010496a GetMenu__6CPVWndFRiPPP11tagMenuList
// STUB: LEMBALL 0x0043a4c0
bool PvWnd::GetMenu(int& p_menuResourceId, MenuList*** p_menuLists)
{
	return 0;
}

// 68K 0x10104ba0 OnVisibilityChange__6CPVWndFv
// STUB: LEMBALL 0x00465a70
void PvWnd::OnVisibilityChange()
{
}

// 68K 0x10104bc8 SetDontUpdateRect__6CPVWndFRC7CVSRect
// STUB: LEMBALL 0x00465a80
void PvWnd::SetDontUpdateRect(const VsRect& p_rect)
{
}

// 68K 0x10216992 __ct__6CPVWndFv
// STUB: LEMBALL 0x00465cc0
PvWnd::PvWnd()
{
}

// 68K 0x10216a50 __dt__6CPVWndFv
// STUB: LEMBALL 0x00465d50
PvWnd::~PvWnd()
{
}

// 68K 0x10216aee SetInnerWindow__6CPVWndFRC7CVSRect
// STUB: LEMBALL 0x00465db0
void PvWnd::SetInnerWindow(const VsRect& p_rect)
{
}

// 68K 0x10216b5e SetRect__6CPVWndFRC7CVSRect
// STUB: LEMBALL 0x00465df0
void PvWnd::SetRect(const VsRect& p_rect)
{
}

// 68K 0x10216b98 SetRectInnerZoom__6CPVWndFRC7CVSRectRC7CVSRecti
// STUB: LEMBALL 0x00465e00
void PvWnd::SetRectInnerZoom(const VsRect& p_rect, const VsRect& p_innerRect, int p_zoom)
{
}

// 68K 0x10216c50 InitHotAreaList__6CPVWndFv
// STUB: LEMBALL 0x00465e60
unsigned int PvWnd::InitHotAreaList()
{
	return 0;
}

// 68K 0x10216d90 _OnCreate__6CPVWndFv
// STUB: LEMBALL 0x00465f80
void PvWnd::OnCreate()
{
}

// 68K 0x10216e16 _OnDestroy__6CPVWndFv
// STUB: LEMBALL 0x00465fe0
void PvWnd::BaseOnDestroy()
{
}

// 68K 0x10216edc _OnSize__6CPVWndFv
// STUB: LEMBALL 0x00466060
void PvWnd::OnSize()
{
}

// 68K 0x10217024 _OnMove__6CPVWndFv
// STUB: LEMBALL 0x00466160
void PvWnd::OnMove()
{
}

// 68K 0x1021716c _OnMove__6CPVWndF8CVSPoint
// STUB: LEMBALL 0x00466260
void PvWnd::BaseOnMove()
{
}

// 68K 0x102171cc _OnZoom__6CPVWndFi
// STUB: LEMBALL 0x00466280
void PvWnd::OnZoom(int p_oldZoom)
{
}

// 68K 0x10217228 SetZoom__6CPVWndFi
// STUB: LEMBALL 0x004662b0
void PvWnd::SetZoom(int p_zoom)
{
}

// 68K 0x10117ea6 ReSetMenu__6CPVWndFv
// STUB: LEMBALL 0x004662e0
void PvWnd::ReSetMenu()
{
}

// 68K 0x10117ec6 SetMenu__6CPVWndFRiPP11tagMenuList
// STUB: LEMBALL 0x004662f0
void PvWnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
{
}

// 68K 0x10117ef4 _SetRelTL__6CPVWndFRC8CVSPoint
// STUB: LEMBALL 0x00466330
void PvWnd::SetRelTl(const VsPoint& p_point)
{
}

// 68K 0x1010499c OnDriverChange__6CPVWndFv
// STUB: LEMBALL 0x00466340
void PvWnd::OnDriverChange()
{
}

// 68K 0x10117f1e IsFocusWindow__6CPVWndFv
// STUB: LEMBALL 0x00466350
bool PvWnd::IsFocusWindow()
{
	return 0;
}

// 68K 0x10104bfa Resize__6CPVWndF7CVSSize
// STUB: LEMBALL 0x00466360
void PvWnd::Resize(VsSize p_size)
{
}

void PvWnd::Dummy04() {}
void PvWnd::Dummy38() {}
void PvWnd::Dummy3c() {}
void PvWnd::OnDestroy() {}
void PvWnd::BaseOnSize() {}
void PvWnd::Dummy48() {}
void PvWnd::Dummy4c() {}
void PvWnd::Dummy50() {}
void PvWnd::Dummy54() {}
void PvWnd::Dummy58() {}
void PvWnd::Dummy64() {}
void PvWnd::Dummy70() {}
void PvWnd::Destroy() {}
