#include "CMain2DDisplay.h"

#include "../../AI/Navigation/CAi.h"
#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameMain.h"
#include "../../Control/Level/CLevelLoader.h"
#include "../../Frontend/Base/CBaseFrontendDrawer.h"
#include "../../Frontend/Drawers/CIntroAnimDrawer.h"
#include "../../Frontend/Drawers/CMainOptions1Drawer.h"
#include "../../Frontend/Drawers/CMainOptions2Drawer.h"
#include "../../Frontend/Drawers/CNetworkOptionsDrawer.h"
#include "../../Frontend/Drawers/CPasswordDrawer.h"
#include "../../Frontend/Drawers/CPreviewDrawer.h"
#include "../../Frontend/Drawers/CSuccFailDrawer.h"
#include "../../Frontend/Support/AboutDialog.h"
#include "../../Frontend/Support/CAboutScreen.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CChangeList.h"
#include "../../Visos/Foundation/CVsOStream.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/CResBase.h"
#include "../../Visos/Resources/CResPalette.h"
#include "../../Visos/Resources/CResZrle.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Target/CGraphicsDriver.h"
#include "../../Visos/Target/CGraphicsState.h"
#include "../../Visos/Target/CPlatformServices.h"
#include "C2D.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Resources/CCdLoadAnimDraw.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CBaseCursor.h"
#include "Visos/Graphics/CDrawer.h"

#include <windows.h>

class CBaseQueueHandler;
class CMap;

#pragma intrinsic(strcpy, strcat)

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);

extern MenuList* g_apMainDisplayMenus[4];

// FUNCTION: LEMBALL 0x00431590
CMain2DDisplay::CMain2DDisplay(CGame* p_arg0)
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
	m_currentFlow = FLOW_MAIN_OPTIONS_2;
	m_background = CResZrle::Load(RES_CURSORS_PAW_CURSOR);
	m_primaryPalette = CResPalette::Load(RES_GAME_GAMEPALETTE);
	m_secondaryPalette = CResPalette::Load(RES_GAME_TITLEPALETTE);
	CursorChangeType(CURSOR_DISPLAY_PAW, 0);
	g_pMasterInputQueue->Attach(static_cast<CBaseQueueHandler*>(this), -0x19);
	m_lowWidth = 0x140;
	m_lowHeight = 0xf0;
	m_highWidth = 0x280;
	m_highHeight = 0x1e0;
	m_resolutionMode = g_nCompactPrimaryContextLayout;
}

// FUNCTION: LEMBALL 0x004316c0
CMain2DDisplay::~CMain2DDisplay()
{
	CResBase* resource;

	resource = (CResBase*) m_secondaryPalette;
	resource->UnLoad();
	resource = (CResBase*) m_primaryPalette;
	resource->UnLoad();
	resource = (CResBase*) m_background;
	resource->UnLoad();
	g_pMasterInputQueue->Detach(static_cast<CBaseQueueHandler*>(this), -0x19);
}

// FUNCTION: LEMBALL 0x00431730
unsigned int CMain2DDisplay::GetStyle()
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
void CMain2DDisplay::OnCreate()
{
	SetZoom(1);
	AttachPalette(RES_GAME_GAMEPALETTE);
	m_gdi->m_renderTarget->EnableBackBuff(1);
	m_drawer = 0;
}

// FUNCTION: LEMBALL 0x004317c0
void CMain2DDisplay::OnDestroy()
{
	CursorChangeType(CURSOR_DISPLAY_NONE, 0);
	if (m_drawer != 0) {
		m_drawer->ShutDown();
		m_drawer->DestroyDrawer();
		delete m_drawer;
		m_drawer = 0;
	}
}

// FUNCTION: LEMBALL 0x00431810
void CMain2DDisplay::OnPaint(const CVsRect& p_rect)
{
	if (m_gdi != 0) {
		if (IsWindowValid() != 0) {
			if (m_drawPrimitive != 0) {
				((CCdLoadAnimDraw*) m_drawPrimitive)->Draw();
			}
			if (m_drawer != 0) {
				((CBaseFrontendDrawer*) m_drawer)->Draw(p_rect);
			}
			if (m_drawer != 0) {
				m_frameCount = m_frameCount + 1;
				((CBaseFrontendDrawer*) m_drawer)->ResetPrimitives();
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00431860
void CMain2DDisplay::OnSize()
{
	if (m_drawer != 0) {
		((CDrawer*) m_drawer)->OnSize(m_rect);
	}
}

// FUNCTION: LEMBALL 0x00431880
void CMain2DDisplay::OnZoom(int p_zoom)
{
	if (m_drawer != 0) {
		m_drawer->OnZoom(m_rect);
	}
}

// FUNCTION: LEMBALL 0x004318a0
void CMain2DDisplay::OnMove()
{
	if (m_drawer != 0) {
		m_drawer->OnMove(m_rect);
	}
}

// FUNCTION: LEMBALL 0x004318c0
bool CMain2DDisplay::IsWindowValid()
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

// FUNCTION: LEMBALL 0x00431910
void CMain2DDisplay::RefreshView()
{
	if (IsWindowValid() != 0) {
		if (m_lifecycleRefs == 1) {
			Refresh(0);
		}
		if (m_drawer != 0) {
			((CDrawer*) m_drawer)->RefreshView();
		}
	}
}

// FUNCTION: LEMBALL 0x00431940
void CMain2DDisplay::Process()
{
	if (m_drawer != 0) {
		m_drawer->Process();
	}
}

// FUNCTION: LEMBALL 0x00431950
void CMain2DDisplay::KillDrawer(eFlowProcesses p_flow)
{
	m_drawerClosing = 1;
	if (m_drawer != 0) {
		((CDrawer*) m_drawer)->DestroyDrawer();
		if (m_drawer != 0) {
			delete (CDrawer*) m_drawer;
		}
		m_drawer = 0;
	}
}

// FUNCTION: LEMBALL 0x00431990
void CMain2DDisplay::StatusUpdate(eFlowProcesses p_flow)
{
	void* storage;
	CChangeList* changeList;
	unsigned char variant;

	if (m_currentFlow == p_flow) {
		return;
	}

	SetZoom(1);
	CVsRect localRect;
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
		m_drawer = new (storage) CIntroAnimDrawer(this, m_gdi, localRect, variant);
		break;
	case 2:
		storage = operator new(0x3bc);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CMainOptions1Drawer(this, m_gdi, localRect);
		break;
	case 3:
		storage = operator new(0x3ac);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CMainOptions2Drawer(this, m_gdi, localRect);
		break;
	case 4:
		storage = operator new(0x488);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CPreviewDrawer(this, m_gdi, localRect);
		break;
	case 5:
	case 0x13: {
		CAi* ai;

		m_activeProcess = (CAi*) m_game->m_process;
		ai = (CAi*) m_game->m_process;
		m_activePalette = ai->m_map;
		storage = operator new(0x2428);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) C2D(this, (CAi*) m_activeProcess, m_gdi, (CMap*) m_activePalette, localRect);
		break;
	}
	case 10:
		storage = operator new(0x9c);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CAboutScreen(this, m_gdi, localRect);
		break;
	case 0xc:
		storage = operator new(0x438);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CNetworkOptionsDrawer(this, m_gdi, localRect);
		break;
	case 0xe:
		storage = operator new(0x60c);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CSuccFailDrawer(this, m_gdi, localRect, 1);
		break;
	case 0xf:
		storage = operator new(0x60c);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CSuccFailDrawer(this, m_gdi, localRect, 0);
		break;
	case 0x10:
		storage = operator new(0x494);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		m_drawer = new (storage) CPasswordDrawer(this, m_gdi, localRect);
		break;
	case 0x12:
		storage = operator new(0x484);
		if (storage == 0) {
			m_drawer = 0;
			break;
		}
		variant = 1;
		m_drawer = new (storage) CIntroAnimDrawer(this, m_gdi, localRect, variant);
		break;
	}

	if (m_drawer != 0) {
		m_drawer->Initialise();
		m_drawer->ResetPrimitives();
	}
}

// FUNCTION: LEMBALL 0x00431c90
void CMain2DDisplay::ToggleResolution()
{
	if (m_drawer != 0) {
		((CDrawer*) m_drawer)->DestroyDrawer();
	}
	switch (g_pTargetGraphicsSystem->m_driverMode) {
	case 1:
		g_pTargetGraphicsSystem->ChangeDriver(3);
		break;
	case 3:
		g_pTargetGraphicsSystem->ChangeDriver(1);
		break;
	}
}

// FUNCTION: LEMBALL 0x00431cd0
int CMain2DDisplay::ProcessMsg(Message* p_message)
{
	int helpOk;
	char* cdDir;
	char helpPath[256];

	switch ((int) p_message->type) {
	case 4:
		if (p_message->code != 0x25) {
			break;
		}
		ToggleResolution();
		return 1;
	case 0xf:
		switch (p_message->code) {
		default:
			return 1;
		case 1:
			m_pauseWindow = (void*) 1;
			break;
		case 2:
			helpPath[0] = 0;
			cdDir = g_pTargetPlatformServices->GetCdDir(g_szLemballHelpFile);
			strcpy(helpPath, cdDir);
			memcpy(helpPath + strlen(helpPath), "lemball\\lemball.hlp", sizeof("lemball\\lemball.hlp"));
			helpOk = WinHelpA((HWND) m_nativeWindow, helpPath, 0x101, (unsigned long) g_szHelpContentsKey);
			if (helpOk == 0) {
				*g_pErrorOutput << g_szCouldntHelpYa;
			}
			break;
		case 3:
			helpPath[0] = 0;
			cdDir = g_pTargetPlatformServices->GetCdDir("lemball\\lemball.hlp");
			strcpy(helpPath, cdDir);
			memcpy(helpPath + strlen(helpPath), "lemball\\lemball.hlp", sizeof("lemball\\lemball.hlp"));
			helpOk = WinHelpA((HWND) m_nativeWindow, helpPath, 0x105, (unsigned long) "");
			if (helpOk == 0) {
				*g_pErrorOutput << "Couldn't help ya!\n";
			}
			break;
		case 4:
			DialogBoxParamA(g_pApplicationInstance, g_szAboutBox, (HWND) m_nativeWindow, (DLGPROC) AboutDialogProc, 0);
			break;
		case 5:
			ToggleResolution();
			break;
		case 6:
			helpOk = WinHelpA((HWND) m_nativeWindow, 0, 4, 0);
			if (helpOk == 0) {
				*g_pErrorOutput << "Couldn't help ya!\n";
			}
			break;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00431eb0
bool CMain2DDisplay::GetMenu(int& p_count, MenuList*** p_menu)
{
	p_count = 0x73;
	*p_menu = g_apMainDisplayMenus;
	return 1;
}

// FUNCTION: LEMBALL 0x00431ed0
void CMain2DDisplay::OnDriverChange()
{
	if (m_drawer != 0) {
		m_drawer->OnDriverChange();
	}
}

// FUNCTION: LEMBALL 0x00431ee0
int CMain2DDisplay::QuitYet()
{
	if (m_pauseWindow != 0) {
		return 2;
	}
	if (m_drawer != 0) {
		return m_drawer->QuitYet();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00431f10
int CMain2DDisplay::GetReturnState()
{
	if (m_pauseWindow == 0 && m_drawer != 0) {
		return m_drawer->GetReturnState();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00431f30
CVsRect CMain2DDisplay::GetUseRect(int p_x, int p_y)
{
	CVsRect result;
	short& width = result.m_width;
	short& height = result.m_height;
	short& x = result.m_x;
	short& y = result.m_y;
	int compact;
	const CVsSize& screenSize = g_pTargetGraphicsDriver->m_screenSize;

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
	y = (short) (screenSize.m_height - height) / 2;
	x = (short) (screenSize.m_width - width) / 2;
	if (p_x != -1) {
		x = (short) p_x;
	}
	if (p_y != -1) {
		y = (short) p_y;
	}
	return result;
}

// FUNCTION: LEMBALL 0x004322d0
void CMain2DDisplay::OnRestore()
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

typedef void* MenuListStorage[6];

// GLOBAL: LEMBALL 0x0049e5f8
MenuListStorage g_aFileMenuItems[3] = {
	{g_szMenuFile, 0, 0, (void*) 1, 0, 0},
	{g_szMenuExit, (void*) 40001, (void*) 1, (void*) 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e640
MenuListStorage g_aOptionsMenuItems[3] = {
	{g_szMenuOptions, 0, 0, (void*) 1, 0, 0},
	{g_szMenuFullScreen, (void*) 40012, (void*) 5, (void*) 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e688
MenuListStorage g_aHelpMenuItems[6] = {
	{g_szMenuHelp, 0, 0, (void*) 1, 0, 0},
	{g_szMenuContents, (void*) 40003, (void*) 2, (void*) 1, 0, 0},
	{g_szMenuSearchTopic, (void*) 40016, (void*) 3, (void*) 1, 0, 0},
	{g_szMenuHelpOnHelp, (void*) 40013, (void*) 6, (void*) 1, 0, 0},
	{g_szMenuAbout, (void*) 40011, (void*) 4, (void*) 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e718
MenuList* g_apMainDisplayMenus[4] = {
	(MenuList*) g_aFileMenuItems,
	(MenuList*) g_aOptionsMenuItems,
	(MenuList*) g_aHelpMenuItems,
	0,
};
