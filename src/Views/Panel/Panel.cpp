#include "Panel.h"

// 68K 0x10b0d942 GetPausePos__6CPanelFv
// STUB: LEMBALL 0x00442f00
VsPoint Panel::GetPausePos()
{
	return *(VsPoint*) 0;
}

// 68K 0x10b0da4a __ct__6CPanelFP3C2D
// STUB: LEMBALL 0x00442f80
Panel::Panel(C2D* p_arg0)
{
}

// 68K 0x10b0dc46 __dt__6CPanelFv
// STUB: LEMBALL 0x00443140
Panel::~Panel()
{
}

// 68K 0x10b0dd10 OnSize__6CPanelFv
// STUB: LEMBALL 0x004431f0
void Panel::OnSize()
{
}

// 68K 0x10b0dd98 Process__6CPanelFv
// STUB: LEMBALL 0x00443250
void Panel::Process()
{
}

// 68K 0x10b0ddd8 SetPause__6CPanelFUc
// STUB: LEMBALL 0x00443270
void Panel::SetPause(unsigned char p_paused)
{
}

// 68K 0x10b0de2c TranslateKey__6CPanelFUl
// STUB: LEMBALL 0x004432a0
unsigned long Panel::TranslateKey(unsigned long p_key)
{
	return 0;
}

// 68K 0x10b0de64 ProcessMsg__6CPanelFP10tagMESSAGE
// STUB: LEMBALL 0x004432c0
int Panel::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b0df46 MouseInPanel__6CPanelFRC8CVSPoint
// STUB: LEMBALL 0x00443360
bool Panel::MouseInPanel(const VsPoint& p_point)
{
	return 0;
}
