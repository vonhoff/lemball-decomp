#include "Wnd.h"

// 68K 0x1010480a IsFocusWindow__4CWndFv
// STUB: LEMBALL 0x004324d0
bool Wnd::IsFocusWindow()
{
	return 0;
}

// 68K 0x101048b4 OnMinimise__4CWndFv
// STUB: LEMBALL 0x004324f0
void Wnd::OnMinimise()
{
}

// 68K 0x101048d2 OnMaximise__4CWndFv
// STUB: LEMBALL 0x00432500
void Wnd::OnMaximise()
{
}

// 68K 0x1010489a OnMove__4CWndFv
// STUB: LEMBALL 0x0043a4d0 FOLDED
void Wnd::OnMove()
{
}

// 68K 0x101048f0 OnRestore__4CWndFv
// STUB: LEMBALL 0x0043a4e0
void Wnd::OnRestore()
{
}

// 68K 0x1010490e OnZoom__4CWndFi
// STUB: LEMBALL 0x0043a4f0
void Wnd::OnZoom(int p_oldZoom)
{
}

// 68K 0x10104928 OnZoomBox__4CWndFUc
// STUB: LEMBALL 0x0043a500
void Wnd::OnZoomBox(unsigned char p_zoomBox)
{
}

// 68K 0x10104846 OnCreate__4CWndFv
// STUB: LEMBALL 0x004644f0 FOLDED
void Wnd::OnCreate()
{
}

// 68K 0x10104862 OnDestroy__4CWndFv
// STUB: LEMBALL 0x00464500 FOLDED
void Wnd::OnDestroy()
{
}

// 68K 0x10104880 OnSize__4CWndFv
// STUB: LEMBALL 0x00464510 FOLDED
void Wnd::OnSize()
{
}

// 68K 0x10110340 ProcessMessage__4CWndFR11EventRecord
// STUB: LEMBALL 0x00464520
int Wnd::ProcessMessage(EventRecord& p_event)
{
	// STRING: LEMBALL 0x004a1fa0 "fQuit\n"
	// STRING: LEMBALL 0x004a1f94 "Quitting\n"
	return 0;
}

// 68K 0x101112b8 MoveAbsolute__4CWndFRC8CVSPoint
// STUB: LEMBALL 0x00464f10
void Wnd::MoveAbsolute(const VsPoint& p_point)
{
}

// 68K 0x10111384 Move__4CWndFRC8CVSPoint
// STUB: LEMBALL 0x00464fa0
void Wnd::Move(const VsPoint& p_point)
{
}

// 68K 0x1011023e ProcessMouseMoves__4CWndFv
// STUB: LEMBALL 0x00465050
void Wnd::ProcessMouseMoves()
{
}

// 68K 0x10110d2c __ct__4CWndFv
// STUB: LEMBALL 0x004651d0
Wnd::Wnd()
{
}

// 68K 0x10110e74 Create__4CWndFRC7CVSRectP6CPVWndPc
// STUB: LEMBALL 0x00465200
void Wnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title)
{
	// STRING: LEMBALL 0x004a1f7c "VS_Base_Window_Class"
	// STRING: LEMBALL 0x004a1fd0 "Unable to create window"
}

// 68K 0x1011126e __dt__4CWndFv
// STUB: LEMBALL 0x00465570
Wnd::~Wnd()
{
}

// 68K 0x10111462 Destroy__4CWndFv
// STUB: LEMBALL 0x004655a0
void Wnd::Destroy()
{
}

// 68K 0x10104946 Refresh__4CWndFP7CVSRect
// STUB: LEMBALL 0x004655f0
void Wnd::Refresh(VsRect* p_rect)
{
}

// 68K 0x10110854 ProcessOtherMessages__4CWndFR11EventRecords
// STUB: LEMBALL 0x00465640
int Wnd::ProcessOtherMessages(EventRecords& p_event)
{
	return 0;
}

// 68K 0x10111552 ReSetMenu__4CWndFv
// STUB: LEMBALL 0x00465660
void Wnd::ReSetMenu()
{
}

// 68K 0x10111570 SetMenu__4CWndFRiPP11tagMenuList
// STUB: LEMBALL 0x004656f0
void Wnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
{
}

// 68K 0x1011162c SelectMenu__4CWndFl
// STUB: LEMBALL 0x00465750
int Wnd::SelectMenu(long p_selection)
{
	return 0;
}

// 68K 0x1011177e _SetRect__4CWndFRC7CVSRect
// STUB: LEMBALL 0x00465820
void Wnd::SetRect(const VsRect& p_rect)
{
}

// 68K 0x10111a0a _SetRelTL__4CWndFRC8CVSPoint
// STUB: LEMBALL 0x00465a00
void Wnd::SetRelTl(const VsPoint& p_point)
{
}

// 68K 0x10110cb8 GetStyle__4CWndFv
// STUB: LEMBALL 0x00465a90
unsigned int Wnd::GetStyle()
{
	return 0;
}
