#include "Main2DDisplay.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameMain.h"
#include "../../Control/Level/LevelLoader.h"
#include "../../Frontend/Base/BaseFrontendDrawer.h"
#include "../../Frontend/Drawers/IntroAnimDrawer.h"
#include "../../Frontend/Drawers/MainOptions1Drawer.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/ChangeList.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/ResPalette.h"
#include "../../Visos/Resources/ResZrle.h"
#include "../../Visos/Target/TargetGraphicsDriver.h"
#include "../../Visos/Target/TargetGraphicsSystemState.h"

#include <new.h>

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);

// 68K 0x10b001a0 __ct__14CMain2DDisplayFP5CGame
// STUB: LEMBALL 0x00431590
Main2DDisplay::Main2DDisplay(Game* p_arg0)
{
	m_pauseWindow = 0;
	m_drawPrimitive = 0;
	m_frameCount = 0;
	m_game = p_arg0;
	m_activeProcess = 0;
	m_windowReady = 1;
	m_activePalette = 0;
	m_drawer = 0;
	m_drawerClosing = 1;
	m_currentFlow = 3;
	m_background = ResZrle::Load(0xfc);
	m_primaryPalette = ResPalette::Load(0x2e);
	m_secondaryPalette = ResPalette::Load(0x2f);
	CursorChangeType(2, 0);
	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->Attach(&m_queueHandler, -0x19);
	}
	m_lowWidth = 0x140;
	m_lowHeight = 0xf0;
	m_highWidth = 0x280;
	m_highHeight = 0x1e0;
	m_resolutionMode = (unsigned int) g_nCompactPrimaryContextLayout;
}

// 68K 0x10b003f8 GetStyle__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431730
unsigned int Main2DDisplay::GetStyle()
{
	unsigned int style;

	style = 0x80001801;
	if (g_nCompactPrimaryContextLayout == 0) {
		if (GetSystemMetrics(0x3d) < 0x281) {
			goto compactStyle;
		}
		if (GetSystemMetrics(0x3e) < 0x1f5) {
			goto compactStyle;
		}
	}
	style = 0x80001b83;
compactStyle:
	if (g_nEditLevelMode != 0) {
		style = style | 0x404;
	}
	return style;
}

// 68K 0x10b0049a OnCreate__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431780
void Main2DDisplay::OnCreate()
{
	GWnd::OnCreate();
	SetZoom(1);
	AttachPalette(0x2e);
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		m_gdi->m_renderTarget->EnableBackBuff(1);
	}
	m_drawer = 0;
}

// 68K 0x10b00568 OnDestroy__14CMain2DDisplayFv
// STUB: LEMBALL 0x004317c0
void Main2DDisplay::OnDestroy()
{
}

// 68K 0x10b0061e OnPaint__14CMain2DDisplayFRC7CVSRect
// STUB: LEMBALL 0x00431810
void Main2DDisplay::OnPaint(const VsRect& p_rect)
{
	if (m_gdi != 0 && IsWindowValid() != 0) {
		if (m_drawer != 0) {
			((BaseFrontendDrawer*) m_drawer)->Draw(p_rect);
			m_frameCount = m_frameCount + 1;
			((BaseFrontendDrawer*) m_drawer)->ResetPrimitives();
		}
	}
}

// 68K 0x10b006b6 OnSize__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431860
void Main2DDisplay::OnSize()
{
	if (m_drawer != 0) {
		((Drawer*) m_drawer)->OnSize(m_rect);
	}
}

// 68K 0x10b00746 OnMove__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431880
void Main2DDisplay::OnMove()
{
}

// 68K 0x10b006fe OnZoom__14CMain2DDisplayFi
// STUB: LEMBALL 0x004318a0
void Main2DDisplay::OnZoom(int p_zoom)
{
}

// 68K 0x10b00798 IsWindowValid__14CMain2DDisplayFv
// STUB: LEMBALL 0x004318c0
bool Main2DDisplay::IsWindowValid()
{
	if (GetSizeStatus() == 0) {
		return 0;
	}
	if (m_lowWidth == m_rect.m_width && m_lowHeight == m_rect.m_height) {
		return 1;
	}
	if (m_highWidth == m_rect.m_width && m_highHeight == m_rect.m_height) {
		return 1;
	}
	return 0;
}

// 68K 0x10b0085c RefreshView__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431910
void Main2DDisplay::RefreshView()
{
	if (IsWindowValid() != 0) {
		if (m_lifecycleRefs == 1) {
			Refresh(0);
		}
		if (m_drawer != 0) {
			((Drawer*) m_drawer)->RefreshView();
		}
	}
}

// 68K 0x10b008d0 Process__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431940
void Main2DDisplay::Process()
{
	if (m_drawer != 0) {
		((Drawer*) m_drawer)->Process();
	}
}

// 68K 0x10b00922 KillDrawer__14CMain2DDisplayF14eFlowProcesses
// STUB: LEMBALL 0x00431950
void Main2DDisplay::KillDrawer(eFlowProcesses p_flow)
{
	m_drawerClosing = 1;
	if (m_drawer != 0) {
		((Drawer*) m_drawer)->ShutDown();
		delete (Drawer*) m_drawer;
		m_drawer = 0;
	}
}

// 68K 0x10b009a0 StatusUpdate__14CMain2DDisplayF14eFlowProcesses
// STUB: LEMBALL 0x00431990
void Main2DDisplay::StatusUpdate(eFlowProcesses p_flow)
{
	VsRect localRect;
	void* storage;
	ChangeList* changeList;
	unsigned char variant;

	if (m_currentFlow == p_flow) {
		return;
	}

	SetZoom(1);
	localRect.m_width = m_rect.m_width;
	localRect.m_height = m_rect.m_height;
	localRect.m_y = 0;
	localRect.m_x = 0;
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		changeList = m_gdi->m_renderTarget->GetChangeList();
		if (changeList != 0) {
			changeList->Reset();
			changeList->SetDrawMark();
		}
	}

	m_currentFlow = p_flow;
	storage = 0;
	switch (p_flow) {
	case 1:
		storage = operator new(0x484);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		variant = 0;
		m_drawer = new (storage) IntroAnimDrawer(this, m_gdi, localRect, variant);
		break;
	case 2:
		storage = operator new(0x3bc);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) MainOptions1Drawer(this, m_gdi, localRect);
		break;
	case 0x12:
		storage = operator new(0x484);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		variant = 1;
		m_drawer = new (storage) IntroAnimDrawer(this, m_gdi, localRect, variant);
		break;
	default:
		m_drawer = 0;
		break;
	}

	if (m_drawer != 0) {
		((Drawer*) m_drawer)->Initialise();
		((BaseFrontendDrawer*) m_drawer)->ResetPrimitives();
	}
}

// 68K 0x10b00cf2 ToggleResolution__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431c90
void Main2DDisplay::ToggleResolution()
{
}

// 68K 0x10b00d5e ProcessMsg__14CMain2DDisplayFP10tagMESSAGE
// STUB: LEMBALL 0x00431cd0
bool Main2DDisplay::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b00e32 GetMenu__14CMain2DDisplayFRiPPP11tagMenuList
// STUB: LEMBALL 0x00431eb0
bool Main2DDisplay::GetMenu(int& p_count, MenuList*** p_menu)
{
	return 0;
}

// 68K 0x10b00e78 OnDriverChange__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431ed0
void Main2DDisplay::OnDriverChange()
{
}

// 68K 0x10b00ec0 QuitYet__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431ee0
unsigned char Main2DDisplay::QuitYet()
{
	if (m_pauseWindow != 0) {
		return 2;
	}
	if (m_drawer != 0) {
		return (unsigned char) ((BaseFrontendDrawer*) m_drawer)->QuitYet();
	}
	return 0;
}

// 68K 0x10b00f80 GetReturnState__14CMain2DDisplayFv
// STUB: LEMBALL 0x00431f10
unsigned char Main2DDisplay::GetReturnState()
{
	if (m_pauseWindow == 0 && m_drawer != 0) {
		return (unsigned char) ((BaseFrontendDrawer*) m_drawer)->GetReturnState();
	}
	return 0;
}

// 68K 0x10b00fe2 GetUseRect__14CMain2DDisplayFii
// STUB: LEMBALL 0x00431f30
VsRect Main2DDisplay::GetUseRect(int p_x, int p_y)
{
	short width;
	short height;
	short x;
	short y;
	int compact;

	compact = g_pTargetGraphicsSystem->m_driverMode == 3;
	g_nCompactPrimaryContextLayout = compact;
	if (compact != 0) {
		width = m_lowWidth;
		height = m_lowHeight;
	}
	else {
		width = m_highWidth;
		height = m_highHeight;
	}
	y = (short) ((g_pTargetGraphicsDriver->m_screenSize.m_height - height) / 2);
	x = (short) ((g_pTargetGraphicsDriver->m_screenSize.m_width - width) / 2);
	if (p_x != -1) {
		x = (short) p_x;
	}
	if (p_y != -1) {
		y = (short) p_y;
	}
	return VsRect(x, y, width, height);
}

// 68K 0x1011c78c OnRestore__14CMain2DDisplayFv
// STUB: LEMBALL 0x004322d0
void Main2DDisplay::OnRestore()
{
}

// 68K 0x10b002fc __dt__14CMain2DDisplayFv
Main2DDisplay::~Main2DDisplay()
{
}
