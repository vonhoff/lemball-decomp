#include "Main2DDisplay.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameMain.h"
#include "../../Control/Level/LevelLoader.h"
#include "../../Frontend/Base/BaseFrontendDrawer.h"
#include "../../Frontend/Drawers/IntroAnimDrawer.h"
#include "../../Frontend/Drawers/MainOptions1Drawer.h"
#include "../../Frontend/Drawers/MainOptions2Drawer.h"
#include "../../Frontend/Drawers/NetworkOptionsDrawer.h"
#include "../../Frontend/Drawers/PasswordDrawer.h"
#include "../../Frontend/Drawers/PreviewDrawer.h"
#include "../../Frontend/Drawers/SuccFailDrawer.h"
#include "../../Frontend/Resources/CdLoadAnim.h"
#include "../../Frontend/Support/AboutDialog.h"
#include "../../Frontend/Support/TargetAboutScreen.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/ChangeList.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResBase.h"
#include "../../Visos/Resources/ResPalette.h"
#include "../../Visos/Resources/ResZrle.h"
#include "../../Visos/Target/TargetGraphicsDriver.h"
#include "../../Visos/Target/TargetGraphicsSystemState.h"
#include "../../Visos/Target/TargetPlatformServices.h"
#include "C2D.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma intrinsic(strcpy, strcat)

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);

extern MenuList* g_apMainDisplayMenus[4];

// 68K 0x10b001a0 __ct__14CMain2DDisplayFP5CGame
// FUNCTION: LEMBALL 0x00431590
Main2DDisplay::Main2DDisplay(Game* p_arg0)
{
	m_lowHeight = 0;
	m_lowWidth = 0;
	m_highHeight = 0;
	m_pauseWindow = 0;
	m_highWidth = 0;
	m_drawPrimitive = 0;
	m_frameCount = 0;
	m_game = p_arg0;
	m_activeProcess = 0;
	m_windowReady = 1;
	m_activePalette = 0;
	m_drawer = 0;
	m_drawerClosing = 1;
	m_gdiFlags = 0x258;
	m_currentFlow = 3;
	m_background = ResZrle::Load(RES_CURSORS_PAW_CURSOR);
	m_primaryPalette = ResPalette::Load(RES_GAME_GAMEPALETTE);
	m_secondaryPalette = ResPalette::Load(RES_GAME_TITLEPALETTE);
	CursorChangeType(2, 0);
	g_pMasterInputQueue->Attach(this != 0 ? static_cast<BaseQueueHandler*>(this) : 0, -0x19);
	m_lowWidth = 0x140;
	m_lowHeight = 0xf0;
	m_highWidth = 0x280;
	m_highHeight = 0x1e0;
	m_resolutionMode = g_nCompactPrimaryContextLayout;
}

// 68K 0x10b002fc __dt__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x004316c0
Main2DDisplay::~Main2DDisplay()
{
	ResBase* resource;

	resource = (ResBase*) m_secondaryPalette;
	resource->UnLoad();
	resource = (ResBase*) m_primaryPalette;
	resource->UnLoad();
	resource = (ResBase*) m_background;
	resource->UnLoad();
	g_pMasterInputQueue->Detach(this != 0 ? static_cast<BaseQueueHandler*>(this) : 0, -0x19);
}

// 68K 0x10b003f8 GetStyle__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431730
unsigned int Main2DDisplay::GetStyle()
{
	unsigned int style = 0x80001801;
	if (g_nCompactPrimaryContextLayout != 0 || (GetSystemMetrics(0x3d) > 0x280 && GetSystemMetrics(0x3e) > 0x1f4)) {
		style = 0x80001b83;
	}
	if (g_nEditLevelMode != 0) {
		style |= 0x404;
	}
	return style;
}

// FUNCTION: LEMBALL 0x00431780
void Main2DDisplay::Dummy3c()
{
	SetZoom(1);
	AttachPalette(RES_GAME_GAMEPALETTE);
	m_gdi->m_renderTarget->EnableBackBuff(1);
	m_drawer = 0;
}

// 68K 0x10b00568 OnDestroy__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x004317c0
void Main2DDisplay::OnDestroy()
{
	CursorChangeType(0, 0);
	if (m_drawer != 0) {
		m_drawer->ShutDown();
		m_drawer->DestroyDrawer();
		delete m_drawer;
		m_drawer = 0;
	}
}

// 68K 0x10b0061e OnPaint__14CMain2DDisplayFRC7CVSRect
// FUNCTION: LEMBALL 0x00431810
void Main2DDisplay::OnPaint(const VsRect& p_rect)
{
	if (m_gdi != 0) {
		if (IsWindowValid() != 0) {
			if (m_drawPrimitive != 0) {
				((CdLoadAnimDraw*) m_drawPrimitive)->Draw();
			}
			if (m_drawer != 0) {
				((BaseFrontendDrawer*) m_drawer)->Draw(p_rect);
			}
			if (m_drawer != 0) {
				m_frameCount = m_frameCount + 1;
				((BaseFrontendDrawer*) m_drawer)->ResetPrimitives();
			}
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
// 68K 0x10b006fe OnZoom__14CMain2DDisplayFi
// FUNCTION: LEMBALL 0x00431880
void Main2DDisplay::OnZoom(int p_zoom)
{
	if (m_drawer != 0) {
		m_drawer->OnZoom(m_rect);
	}
}

// 68K 0x10b00746 OnMove__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x004318a0
void Main2DDisplay::OnMove()
{
	if (m_drawer != 0) {
		m_drawer->OnMove(m_rect);
	}
}

// 68K 0x10b00798 IsWindowValid__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x004318c0
bool Main2DDisplay::IsWindowValid()
{
	if (!GetSizeStatus()) {
		return 0;
	}
	short height = m_rect.m_height;
	short width = m_rect.m_width;
	if (m_lowWidth == width && m_lowHeight == height) {
		return 1;
	}
	if (m_highWidth == width && m_highHeight == height) {
		return 1;
	}
	return 0;
}

// 68K 0x10b0085c RefreshView__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431910
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
// FUNCTION: LEMBALL 0x00431940
void Main2DDisplay::Process()
{
	if (m_drawer != 0) {
		m_drawer->Process();
	}
}

// 68K 0x10b00922 KillDrawer__14CMain2DDisplayF14eFlowProcesses
// FUNCTION: LEMBALL 0x00431950
void Main2DDisplay::KillDrawer(eFlowProcesses p_flow)
{
	m_drawerClosing = 1;
	if (m_drawer != 0) {
		((Drawer*) m_drawer)->DestroyDrawer();
		if (m_drawer != 0) {
			delete (Drawer*) m_drawer;
		}
		m_drawer = 0;
	}
}

// 68K 0x10b009a0 StatusUpdate__14CMain2DDisplayF14eFlowProcesses
// FUNCTION: LEMBALL 0x00431990
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
	changeList = m_gdi->m_renderTarget->GetChangeList();
	changeList->Reset();
	changeList->SetDrawMark();

	m_currentFlow = p_flow;
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
	case 3:
		storage = operator new(0x3ac);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) MainOptions2Drawer(this, m_gdi, localRect);
		break;
	case 4:
		storage = operator new(0x488);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) PreviewDrawer(this, m_gdi, localRect);
		break;
	case 5:
	case 0x13: {
		Ai* ai;

		if (m_game->m_process == 0) {
			m_activeProcess = 0;
		}
		else {
			m_activeProcess = (Ai*) m_game->m_process;
		}
		if (m_game->m_process == 0) {
			ai = 0;
		}
		else {
			ai = (Ai*) m_game->m_process;
		}
		m_activePalette = ai->m_map;
		storage = operator new(0x2428);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) C2D(this, (Ai*) m_activeProcess, m_gdi, (Map*) m_activePalette, localRect);
		break;
	}
	case 10:
		storage = operator new(0x9c);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) TargetAboutScreen(this, m_gdi, localRect);
		break;
	case 0xc:
		storage = operator new(0x438);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) NetworkOptionsDrawer(this, m_gdi, localRect);
		break;
	case 0xe:
		storage = operator new(0x60c);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) SuccFailDrawer(this, m_gdi, localRect, 1);
		break;
	case 0xf:
		storage = operator new(0x60c);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) SuccFailDrawer(this, m_gdi, localRect, 0);
		break;
	case 0x10:
		storage = operator new(0x494);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) PasswordDrawer(this, m_gdi, localRect);
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
	}

	if (m_drawer != 0) {
		m_drawer->Initialise();
		m_drawer->ResetPrimitives();
	}
}

// 68K 0x10b00cf2 ToggleResolution__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431c90
void Main2DDisplay::ToggleResolution()
{
	if (m_drawer != 0) {
		((Drawer*) m_drawer)->DestroyDrawer();
	}
	if (g_pTargetGraphicsSystem->m_driverMode != 1) {
		if (g_pTargetGraphicsSystem->m_driverMode != 3) {
			return;
		}
		g_pTargetGraphicsSystem->SelectDriver(1);
		return;
	}
	g_pTargetGraphicsSystem->SelectDriver(3);
}

// 68K 0x10b00d5e ProcessMsg__14CMain2DDisplayFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00431cd0
int Main2DDisplay::ProcessMsg(Message* p_message)
{
	int helpOk;
	char* cdDir;
	char helpPath[256];

	if (p_message->type == 4) {
		if (p_message->code != 0x25) {
			return 0;
		}
		ToggleResolution();
		return 1;
	}
	if (p_message->type != 0xf) {
		return 0;
	}
	switch (p_message->code) {
	case 1:
		m_pauseWindow = (void*) 1;
		return 0;
	case 2:
		helpPath[0] = 0;
		cdDir = g_pTargetPlatformServices->GetCDDir(g_szLemballHelpFile);
		if (cdDir != 0) {
			strcpy(helpPath, cdDir);
			strcat(helpPath, g_szLemballHelpFile);
			helpOk = WinHelpA((HWND) m_nativeWindow, helpPath, 0x101, (unsigned long) g_szHelpContentsKey);
			if (helpOk != 0) {
				return 0;
			}
		}
		*g_pErrorOutput << g_szCouldntHelpYa;
		return 0;
	case 3:
		helpPath[0] = 0;
		cdDir = g_pTargetPlatformServices->GetCDDir(g_szLemballHelpFile);
		if (cdDir != 0) {
			strcpy(helpPath, cdDir);
			strcat(helpPath, g_szLemballHelpFile);
			helpOk = WinHelpA((HWND) m_nativeWindow, helpPath, 0x105, (unsigned long) "");
			if (helpOk != 0) {
				return 0;
			}
		}
		*g_pErrorOutput << g_szCouldntHelpYa;
		return 0;
	case 4:
		DialogBoxParamA(g_pApplicationInstance, g_szAboutBox, (HWND) m_nativeWindow, (DLGPROC) AboutDialogProc, 0);
		return 0;
	case 5:
		ToggleResolution();
		return 0;
	case 6:
		helpOk = WinHelpA((HWND) m_nativeWindow, 0, 4, 0);
		if (helpOk != 0) {
			return 0;
		}
		*g_pErrorOutput << g_szCouldntHelpYa;
		return 0;
	default:
		return 1;
	}
}

// 68K 0x10b00e32 GetMenu__14CMain2DDisplayFRiPPP11tagMenuList
// FUNCTION: LEMBALL 0x00431eb0
bool Main2DDisplay::GetMenu(int& p_count, MenuList*** p_menu)
{
	p_count = 0x73;
	*p_menu = g_apMainDisplayMenus;
	return 1;
}

// 68K 0x10b00e78 OnDriverChange__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431ed0
void Main2DDisplay::OnDriverChange()
{
	if (m_drawer != 0) {
		m_drawer->OnDriverChange();
	}
}

// 68K 0x10b00ec0 QuitYet__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431ee0
int Main2DDisplay::QuitYet()
{
	if (m_pauseWindow != 0) {
		return 2;
	}
	if (m_drawer != 0) {
		return m_drawer->QuitYet();
	}
	return 0;
}

// 68K 0x10b00f80 GetReturnState__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x00431f10
int Main2DDisplay::GetReturnState()
{
	if (m_pauseWindow == 0 && m_drawer != 0) {
		return m_drawer->GetReturnState();
	}
	return 0;
}

// 68K 0x10b00fe2 GetUseRect__14CMain2DDisplayFii
// FUNCTION: LEMBALL 0x00431f30
VsRect Main2DDisplay::GetUseRect(int p_x, int p_y)
{
	VsRect result;
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
	result.m_width = width;
	result.m_height = height;
	result.m_x = x;
	result.m_y = y;
	return result;
}

// 68K 0x1011c78c OnRestore__14CMain2DDisplayFv
// FUNCTION: LEMBALL 0x004322d0
void Main2DDisplay::OnRestore()
{
	OnDriverChange();
}

// GLOBAL: LEMBALL 0x0049e728
char g_szMenuFile[8] = "&File";

// GLOBAL: LEMBALL 0x0049e730
char g_szMenuExit[8] = "E&xit";

// GLOBAL: LEMBALL 0x0049e738
char g_szMenuOptions[12] = "&Options";

// GLOBAL: LEMBALL 0x0049e744
char g_szMenuFullScreen[20] = "Full Screen      F4";

// GLOBAL: LEMBALL 0x0049e758
char g_szMenuHelp[8] = "&Help";

// GLOBAL: LEMBALL 0x0049e760
char g_szMenuContents[12] = "&Contents";

// GLOBAL: LEMBALL 0x0049e76c
char g_szMenuSearchTopic[16] = "&Search Topic";

// GLOBAL: LEMBALL 0x0049e77c
char g_szMenuHelpOnHelp[16] = "H&elp On Help";

// GLOBAL: LEMBALL 0x0049e78c
char g_szMenuAbout[12] = "&About...";

// GLOBAL: LEMBALL 0x0049e5f8
MenuList g_aFileMenuItems[3] = {
	{g_szMenuFile, 0, 0, 1, 0, 0},
	{g_szMenuExit, 40001, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e640
MenuList g_aOptionsMenuItems[3] = {
	{g_szMenuOptions, 0, 0, 1, 0, 0},
	{g_szMenuFullScreen, 40012, 5, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e688
MenuList g_aHelpMenuItems[6] = {
	{g_szMenuHelp, 0, 0, 1, 0, 0},
	{g_szMenuContents, 40003, 2, 1, 0, 0},
	{g_szMenuSearchTopic, 40016, 3, 1, 0, 0},
	{g_szMenuHelpOnHelp, 40013, 6, 1, 0, 0},
	{g_szMenuAbout, 40011, 4, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e718
MenuList* g_apMainDisplayMenus[4] = {
	g_aFileMenuItems,
	g_aOptionsMenuItems,
	g_aHelpMenuItems,
	0,
};
