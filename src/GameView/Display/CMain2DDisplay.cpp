#include "CMain2DDisplay.h"

#include "Platform/Windows/Windowing/MenuList.h"

#include "Gameplay/Simulation/CAI.h"
#include "Application/CGame.h"
#include "Application/GameMain.h"
#include "Level/CLevelLoader.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/Intro/CIntroAnimDrawer.h"
#include "Frontend/Options/CMainOptions1Drawer.h"
#include "Frontend/Options/CMainOptions2Drawer.h"
#include "Frontend/Network/CNetworkOptionsDrawer.h"
#include "Frontend/Password/CPasswordDrawer.h"
#include "Frontend/Preview/CPreviewDrawer.h"
#include "Frontend/Results/CSuccFailDrawer.h"
#include "Platform/Windows/Windowing/AboutDialog.h"
#include "Frontend/About/CAboutScreen.h"
#include "../../Platform/Windows/Entry.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Graphics/CChangeList.h"
#include "Engine/Streams/CVSOStream.h"
#include "Platform/Windows/Input/CCursor.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Platform/Windows/Graphics/CSurface.h"
#include "Engine/Resources/Types/CResBase.h"
#include "Engine/Resources/Types/CResPALETTE.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "../../Engine/Resources/Manifest.h"
#include "Platform/Windows/Graphics/CGraphicsDriver.h"
#include "Platform/Windows/Graphics/CGraphicsState.h"
#include "Platform/Windows/CPlatformServices.h"
#include "C2D.h"
#include "DisplayQuitState.h"

#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include "Application/FlowProcesses.h"
#include "GameView/Loading/LoadAnimCallbacks.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/Message.h"
#include "Engine/Input/CBaseCursor.h"
#include "Platform/Windows/Windowing/CDrawer.h"

#include <windows.h>

class CBaseQueueHandler;
class CMap;

#pragma intrinsic(strcpy, strcat)

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int p_index);
enum eDisplayStyleThreshold {
	DISPLAY_STYLE_WIDTH_THRESHOLD_PX = 640,
	DISPLAY_STYLE_HEIGHT_THRESHOLD_PX = 500
};

enum eMainDisplayWindowStyle {
	MAIN_DISPLAY_STYLE_COMPACT_LAYOUT = 0x80001801,
	MAIN_DISPLAY_STYLE_WIDE_LAYOUT = 0x80001b83,
	MAIN_DISPLAY_STYLE_EDIT_LEVEL_MODE = 0x404
};

enum eDisplayResolution {
	MAIN_DISPLAY_LOW_RESOLUTION_WIDTH_PX = 320,
	MAIN_DISPLAY_LOW_RESOLUTION_HEIGHT_PX = 240,
	MAIN_DISPLAY_HIGH_RESOLUTION_WIDTH_PX = 640,
	MAIN_DISPLAY_HIGH_RESOLUTION_HEIGHT_PX = 480
};

extern MenuList* g_apMainDisplayMenus[4];

enum eMainDisplayMenuAction {
	MAIN_MENU_EXIT = 1,
	MAIN_MENU_HELP_CONTENTS = 2,
	MAIN_MENU_HELP_SEARCH = 3,
	MAIN_MENU_ABOUT = 4,
	MAIN_MENU_TOGGLE_FULLSCREEN = 5,
	MAIN_MENU_HELP_ON_HELP = 6
};

// FUNCTION: LEMBALL 0x00431590
CMain2DDisplay::CMain2DDisplay(CGame* p_game)
{
	m_quitRequested = 0;
	m_loadingDraw = NULL;
	m_frameCount = 0;
	m_game = p_game;
	m_ai = NULL;
	m_windowReady = 1;
	m_map = NULL;
	m_drawer = NULL;
	m_drawerClosing = 1;
	m_gdiFlags = 0x258;
	m_currentFlow = FLOW_MAIN_OPTIONS_2;
	m_cursorResource = CResZRLE::Load(RES_CURSORS_PAW_CURSOR);
	m_gamePalette = CResPALETTE::Load(RES_GAME_GAMEPALETTE);
	m_titlePalette = CResPALETTE::Load(RES_GAME_TITLEPALETTE);
	CursorChangeType(CURSOR_DISPLAY_PAW, 0);
	g_pMasterInputQueue->Attach(static_cast<CBaseQueueHandler*>(this), MASTER_INPUT_QUEUE_PRIORITY);
	m_lowResolutionSize.m_width = MAIN_DISPLAY_LOW_RESOLUTION_WIDTH_PX;
	m_lowResolutionSize.m_height = MAIN_DISPLAY_LOW_RESOLUTION_HEIGHT_PX;
	m_highResolutionSize.m_width = MAIN_DISPLAY_HIGH_RESOLUTION_WIDTH_PX;
	m_highResolutionSize.m_height = MAIN_DISPLAY_HIGH_RESOLUTION_HEIGHT_PX;
	m_resolutionMode = g_nCompactPrimaryContextLayout;
}

// FUNCTION: LEMBALL 0x004316c0
CMain2DDisplay::~CMain2DDisplay()
{
	CResBase* resource;

	resource = m_titlePalette;
	resource->UnLoad();
	resource = m_gamePalette;
	resource->UnLoad();
	resource = (CResBase*) m_cursorResource;
	resource->UnLoad();
	g_pMasterInputQueue->Detach(static_cast<CBaseQueueHandler*>(this), MASTER_INPUT_QUEUE_PRIORITY);
}

// FUNCTION: LEMBALL 0x00431730
unsigned int CMain2DDisplay::GetStyle()
{
	unsigned int style = MAIN_DISPLAY_STYLE_COMPACT_LAYOUT;
	if (g_nCompactPrimaryContextLayout != 0 || (GetSystemMetrics(SM_CXMAXIMIZED) > DISPLAY_STYLE_WIDTH_THRESHOLD_PX &&
												GetSystemMetrics(SM_CYMAXIMIZED) > DISPLAY_STYLE_HEIGHT_THRESHOLD_PX)) {
		style = MAIN_DISPLAY_STYLE_WIDE_LAYOUT;
	}
	if (g_nEditLevelMode != 0) {
		style |= MAIN_DISPLAY_STYLE_EDIT_LEVEL_MODE;
	}
	return style;
}

// FUNCTION: LEMBALL 0x00431780
void CMain2DDisplay::OnCreate()
{
	SetZoom(1);
	AttachPalette(RES_GAME_GAMEPALETTE);
	m_gdi->m_renderTarget->EnableBackBuff(1);
	m_drawer = NULL;
}

// FUNCTION: LEMBALL 0x004317c0
void CMain2DDisplay::OnDestroy()
{
	CursorChangeType(CURSOR_DISPLAY_NONE, 0);
	if (m_drawer != NULL) {
		m_drawer->ShutDown();
		m_drawer->DestroyDrawer();
		delete m_drawer;
		m_drawer = NULL;
	}
}

// FUNCTION: LEMBALL 0x00431810
void CMain2DDisplay::OnPaint(const CVSRect& p_rect)
{
	if (m_gdi != NULL) {
		if (IsWindowValid() != 0) {
			if (m_loadingDraw != NULL) {
				m_loadingDraw->Draw();
			}
			if (m_drawer != NULL) {
				m_drawer->Draw(p_rect);
			}
			if (m_drawer != NULL) {
				m_frameCount = m_frameCount + 1;
				m_drawer->ResetPrimitives();
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00431860
void CMain2DDisplay::OnSize()
{
	if (m_drawer != NULL) {
		((CDrawer*) m_drawer)->OnSize(m_rect);
	}
}

// FUNCTION: LEMBALL 0x00431880
void CMain2DDisplay::OnZoom(int p_zoom)
{
	if (m_drawer != NULL) {
		m_drawer->OnZoom(m_rect);
	}
}

// FUNCTION: LEMBALL 0x004318a0
void CMain2DDisplay::OnMove()
{
	if (m_drawer != NULL) {
		m_drawer->OnMove(m_rect);
	}
}

// FUNCTION: LEMBALL 0x004318c0
bool CMain2DDisplay::IsWindowValid()
{
	if (!GetSizeStatus()) {
		return false;
	}
	short width;
	short height = m_rect.m_height;
	width = m_rect.m_width;
	if (m_lowResolutionSize.m_width == width && m_lowResolutionSize.m_height == height) {
		return true;
	}
	if (m_highResolutionSize.m_width == width && m_highResolutionSize.m_height == height) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00431910
void CMain2DDisplay::RefreshView()
{
	if (IsWindowValid() != 0) {
		if (m_lifecycleRefs == 1) {
			Refresh(NULL);
		}
		if (m_drawer != NULL) {
			((CDrawer*) m_drawer)->RefreshView();
		}
	}
}

// FUNCTION: LEMBALL 0x00431940
void CMain2DDisplay::Process()
{
	if (m_drawer != NULL) {
		m_drawer->Process();
	}
}

// FUNCTION: LEMBALL 0x00431950
void CMain2DDisplay::KillDrawer(eFlowProcesses p_flow)
{
	m_drawerClosing = 1;
	if (m_drawer != NULL) {
		((CDrawer*) m_drawer)->DestroyDrawer();
		if (m_drawer != NULL) {
			delete (CDrawer*) m_drawer;
		}
		m_drawer = NULL;
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
	CVSRect localRect;
	localRect.m_width = m_rect.m_width;
	localRect.m_height = m_rect.m_height;
	localRect.m_y = 0;
	localRect.m_x = 0;
	changeList = m_gdi->m_renderTarget->GetChangeList();
	changeList->Reset();
	changeList->SetDrawMark();

	m_currentFlow = p_flow;
	switch (p_flow) {
	case FLOW_INTRO_ANIM:
		storage = operator new(sizeof(CIntroAnimDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		variant = 0;
		m_drawer = new (storage) CIntroAnimDrawer(this, m_gdi, localRect, variant);
		break;
	case FLOW_MAIN_OPTIONS_1:
		storage = operator new(sizeof(CMainOptions1Drawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CMainOptions1Drawer(this, m_gdi, localRect);
		break;
	case FLOW_MAIN_OPTIONS_2:
		storage = operator new(sizeof(CMainOptions2Drawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CMainOptions2Drawer(this, m_gdi, localRect);
		break;
	case FLOW_PREVIEW:
		storage = operator new(sizeof(CPreviewDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CPreviewDrawer(this, m_gdi, localRect);
		break;
	case FLOW_GAMEPLAY:
	case FLOW_DEMO: {
		CAI* ai;

		m_ai = (CAI*) m_game->m_process;
		ai = (CAI*) m_game->m_process;
		m_map = ai->m_map;
		storage = operator new(sizeof(C2D));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) C2D(this, (CAI*) m_ai, m_gdi, (CMap*) m_map, localRect);
		break;
	}
	case FLOW_ABOUT:
		storage = operator new(sizeof(CAboutScreen));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CAboutScreen(this, m_gdi, localRect);
		break;
	case FLOW_NETWORK_OPTIONS:
		storage = operator new(sizeof(CNetworkOptionsDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CNetworkOptionsDrawer(this, m_gdi, localRect);
		break;
	case FLOW_SUCCESS:
		storage = operator new(sizeof(CSuccFailDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CSuccFailDrawer(this, m_gdi, localRect, 1);
		break;
	case FLOW_FAILURE:
		storage = operator new(sizeof(CSuccFailDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CSuccFailDrawer(this, m_gdi, localRect, 0);
		break;
	case FLOW_PASSWORD:
		storage = operator new(sizeof(CPasswordDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		m_drawer = new (storage) CPasswordDrawer(this, m_gdi, localRect);
		break;
	case FLOW_LEVEL_INTRO:
		storage = operator new(sizeof(CIntroAnimDrawer));
		if (storage == NULL) {
			m_drawer = NULL;
			break;
		}
		variant = 1;
		m_drawer = new (storage) CIntroAnimDrawer(this, m_gdi, localRect, variant);
		break;
	}

	if (m_drawer != NULL) {
		m_drawer->Initialise();
		m_drawer->ResetPrimitives();
	}
}

// FUNCTION: LEMBALL 0x00431c90
void CMain2DDisplay::ToggleResolution()
{
	if (m_drawer != NULL) {
		((CDrawer*) m_drawer)->DestroyDrawer();
	}
	switch (g_pTargetGraphicsSystem->m_driverMode) {
	case GFX_MODE_GDI:
		g_pTargetGraphicsSystem->ChangeDriver(GFX_MODE_VGA_320X240);
		break;
	case GFX_MODE_VGA_320X240:
		g_pTargetGraphicsSystem->ChangeDriver(GFX_MODE_GDI);
		break;
	}
}

// FUNCTION: LEMBALL 0x00431cd0
int CMain2DDisplay::ProcessMsg(Message* p_message)
{
	int helpOk;
	char* cdDir;
	char helpPath[256];
	// GLOBAL: LEMBALL 0x0049e818
	static const char searchHelpError[] = "Couldn't help ya!\n";
	// GLOBAL: LEMBALL 0x0049e82c
	static const char quitHelpError[] = "Couldn't help ya!\n";

	switch ((int) p_message->m_type) {
	case MESSAGE_KEY_DOWN:
		if (p_message->m_code != INPUT_KEY_F4) {
			break;
		}
		ToggleResolution();
		return 1;
	case MESSAGE_WINDOW_COMMAND:
		switch (p_message->m_code) {
		default:
			return 1;
		case MAIN_MENU_EXIT:
			m_quitRequested = 1;
			break;
		case MAIN_MENU_HELP_CONTENTS:
			helpPath[0] = 0;
			cdDir = g_pTargetPlatformServices->GetCDDir(g_szLemballHelpFile);
			strcpy(helpPath, cdDir);
			memcpy(helpPath + strlen(helpPath), "lemball\\lemball.hlp", sizeof("lemball\\lemball.hlp"));
			helpOk = WinHelpA((HWND) m_nativeWindow, helpPath, HELP_KEY, (unsigned long) g_szHelpContentsKey);
			if (helpOk == 0) {
				*g_pErrorOutput << g_szCouldntHelpYa;
			}
			break;
		case MAIN_MENU_HELP_SEARCH:
			helpPath[0] = 0;
			cdDir = g_pTargetPlatformServices->GetCDDir("lemball\\lemball.hlp");
			strcpy(helpPath, cdDir);
			memcpy(helpPath + strlen(helpPath), "lemball\\lemball.hlp", sizeof("lemball\\lemball.hlp"));
			helpOk = WinHelpA((HWND) m_nativeWindow, helpPath, HELP_PARTIALKEY, (unsigned long) "");
			if (helpOk == 0) {
				*g_pErrorOutput << searchHelpError;
			}
			break;
		case MAIN_MENU_ABOUT:
			DialogBoxParamA(g_pApplicationInstance, g_szAboutBox, (HWND) m_nativeWindow, (DLGPROC) AboutDialogProc, 0);
			break;
		case MAIN_MENU_TOGGLE_FULLSCREEN:
			ToggleResolution();
			break;
		case MAIN_MENU_HELP_ON_HELP:
			helpOk = WinHelpA((HWND) m_nativeWindow, NULL, HELP_HELPONHELP, 0);
			if (helpOk == 0) {
				*g_pErrorOutput << quitHelpError;
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
	return true;
}

// FUNCTION: LEMBALL 0x00431ed0
void CMain2DDisplay::OnDriverChange()
{
	if (m_drawer != NULL) {
		m_drawer->OnDriverChange();
	}
}

// FUNCTION: LEMBALL 0x00431ee0
int CMain2DDisplay::QuitYet()
{
	if (m_quitRequested != 0) {
		return DISPLAY_QUIT_APPLICATION;
	}
	if (m_drawer != NULL) {
		return m_drawer->QuitYet();
	}
	return DISPLAY_QUIT_NONE;
}

// FUNCTION: LEMBALL 0x00431f10
int CMain2DDisplay::GetReturnState()
{
	if (m_quitRequested == 0 && m_drawer != NULL) {
		return m_drawer->GetReturnState();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00431f30
CVSRect CMain2DDisplay::GetUseRect(int p_x, int p_y)
{
	CVSRect result;
	short& width = result.m_width;
	short& height = result.m_height;
	short& x = result.m_x;
	short& y = result.m_y;
	int compact;
	const CVSSize& screenSize = g_pTargetGraphicsDriver->m_screenSize;

	compact = g_pTargetGraphicsSystem->m_driverMode == GFX_MODE_VGA_320X240;
	g_nCompactPrimaryContextLayout = compact;
	if (compact != 0) {
		width = m_lowResolutionSize.m_width;
		height = m_lowResolutionSize.m_height;
	}
	else {
		width = m_highResolutionSize.m_width;
		height = m_highResolutionSize.m_height;
	}
	short centeredY = (short) (screenSize.m_height - height) / 2;
	x = (short) (screenSize.m_width - width) / 2;
	y = centeredY;
	if (p_x != DISPLAY_COORDINATE_AUTO_CENTER) {
		x = (short) p_x;
	}
	if (p_y != DISPLAY_COORDINATE_AUTO_CENTER) {
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

// GLOBAL: LEMBALL 0x0049e5f8
MenuList g_aFileMenuItems[3] = {
	{g_szMenuFile, 0, 0, 1, 0, 0},
	{g_szMenuExit, 40001, MAIN_MENU_EXIT, 1, 0, 0},
	{NULL, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e640
MenuList g_aOptionsMenuItems[3] = {
	{g_szMenuOptions, 0, 0, 1, 0, 0},
	{g_szMenuFullScreen, 40012, MAIN_MENU_TOGGLE_FULLSCREEN, 1, 0, 0},
	{NULL, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e688
MenuList g_aHelpMenuItems[6] = {
	{g_szMenuHelp, 0, 0, 1, 0, 0},
	{g_szMenuContents, 40003, MAIN_MENU_HELP_CONTENTS, 1, 0, 0},
	{g_szMenuSearchTopic, 40016, MAIN_MENU_HELP_SEARCH, 1, 0, 0},
	{g_szMenuHelpOnHelp, 40013, MAIN_MENU_HELP_ON_HELP, 1, 0, 0},
	{g_szMenuAbout, 40011, MAIN_MENU_ABOUT, 1, 0, 0},
	{NULL, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e718
MenuList* g_apMainDisplayMenus[4] = {
	g_aFileMenuItems,
	g_aOptionsMenuItems,
	g_aHelpMenuItems,
	NULL,
};
