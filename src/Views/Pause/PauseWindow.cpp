#include "PauseWindow.h"

// 68K 0x10b0e048 Initialise__12CPauseWindowFv
// STUB: LEMBALL 0x00443af0
void PauseWindow::Initialise()
{
}

// 68K 0x10b0e1da Load__12CPauseWindowFv
// STUB: LEMBALL 0x00443c70
void PauseWindow::Load()
{
}

// 68K 0x10b0e2a0 UnLoad__12CPauseWindowFv
// STUB: LEMBALL 0x00443d40
void PauseWindow::UnLoad()
{
}

// 68K 0x10b0e2fc Restart__12CPauseWindowFv
// STUB: LEMBALL 0x00443d80
void PauseWindow::Restart()
{
}

// 68K 0x10b0e390 CreateTheWindow__12CPauseWindowFRC7CVSRect
// STUB: LEMBALL 0x00443db0
void PauseWindow::CreateTheWindow(const VsRect& p_rect)
{
	// STRING: LEMBALL 0x0049f108 "Pause mode"
}

// 68K 0x10b0e6ce CalculateWindow__12CPauseWindowFv
// STUB: LEMBALL 0x00444050
VsRect PauseWindow::CalculateWindow()
{
	return *(VsRect*) 0;
}

// 68K 0x10b0eeba __ct__12CPauseWindowFP19CReceiveWindowStateP7CPVGWnd20ePauseWindowMessages
// STUB: LEMBALL 0x00444680
PauseWindow::PauseWindow(ReceiveWindowState* p_arg0, PvGWnd* p_arg1, ePauseWindowMessages p_arg2)
{
}

// 68K 0x10b0f02a __dt__12CPauseWindowFv
// STUB: LEMBALL 0x00444790
PauseWindow::~PauseWindow()
{
}

// 68K 0x10b0f1aa RegisterRemaps__12CPauseWindowFv
// STUB: LEMBALL 0x004448c0
void PauseWindow::RegisterRemaps()
{
}

// 68K 0x10b0f222 UnRegisterRemaps__12CPauseWindowFv
// STUB: LEMBALL 0x00444900
void PauseWindow::UnRegisterRemaps()
{
}

// 68K 0x10b0f282 Remap__12CPauseWindowFi
// STUB: LEMBALL 0x00444930
BaseRemap* PauseWindow::Remap(int p_item)
{
	return 0;
}

// 68K 0x10b0f2d8 OnPaint__12CPauseWindowFRC7CVSRect
// STUB: LEMBALL 0x00444980
void PauseWindow::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x10b0f404 OnInside__12CPauseWindowFRC8CVSPoint
// STUB: LEMBALL 0x00444a90
void PauseWindow::OnInside(const VsPoint& p_point)
{
}

// 68K 0x10b0f506 OnButtonDown__12CPauseWindowFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00444b20
unsigned int PauseWindow::OnButtonDown(const VsPoint& p_point, unsigned int p_flags)
{
	return 0;
}

// 68K 0x10b0f61a OnButtonUp__12CPauseWindowFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00444bd0
void PauseWindow::OnButtonUp(const VsPoint& p_point, unsigned int p_flags)
{
}

// 68K 0x10b0f67c OnExternalButtonUp__12CPauseWindowFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00444bf0
void PauseWindow::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
{
}

// 68K 0x10b0f6e6 ProcessMsg__12CPauseWindowFP10tagMESSAGE
// STUB: LEMBALL 0x00444c10
int PauseWindow::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b0f8a0 FreeVram__12CPauseWindowFv
// STUB: LEMBALL 0x00444da0
void PauseWindow::FreeVram()
{
}

// 68K 0x10b0f8fc OnDriverChange__12CPauseWindowFv
// STUB: LEMBALL 0x00444dd0
void PauseWindow::OnDriverChange()
{
}
