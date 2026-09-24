#include "CMainOptions1Drawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../Frontend/Controls/CGunController.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/Manifest.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CBitmapRes.h"

class CGWnd;

extern "C" unsigned long __stdcall timeGetTime(void);

// GLOBAL: LEMBALL 0x0049f490
int g_anMainOptions1ButtonLayout[12] = {32, 116, 480, 116, 48, 232, 464, 232, 80, 348, 416, 348};

// GLOBAL: LEMBALL 0x0049f4c0
int g_anMainOptions1CompactButtonLayout[12] = {16, 58, 240, 58, 24, 116, 232, 116, 40, 174, 208, 174};

// GLOBAL: LEMBALL 0x0049f4f4
unsigned long g_dwMainOptions1AnimIds[12] = {RES_NEWFRONT_ICONS_HIRES_ONE_PLAYER,
											 RES_NEWFRONT_ICONS_HIRES_TWO_PLAYER,
											 RES_NEWFRONT_ICONS_HIRES_OPTIONS,
											 RES_NEWFRONT_ICONS_HIRES_FLOPPY_DISK,
											 RES_NEWFRONT_ICONS_HIRES_PASSWORD,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_FUN,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_TRICKY,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_TAXING,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_MAYHEM,
											 RES_NEWFRONT_ICONS_HIRES_OKAY,
											 RES_NEWFRONT_ICONS_HIRES_FULLSCREEN,
											 RES_NEWFRONT_ICONS_LORES_ONE_PLAYER};

// GLOBAL: LEMBALL 0x0049f520
unsigned long g_dwMainOptions1CompactAnimIds[12] = {RES_NEWFRONT_ICONS_LORES_ONE_PLAYER,
													RES_NEWFRONT_ICONS_LORES_TWO_PLAYER,
													RES_NEWFRONT_ICONS_LORES_OPTIONS,
													RES_NEWFRONT_ICONS_LORES_PASSWORD,
													RES_NEWFRONT_ICONS_LORES_FLOPPY_DISK,
													0,
													RES_NEWFRONT_ICONS_LORES_SKILL_FUN,
													RES_NEWFRONT_ICONS_LORES_SKILL_TRICKY,
													RES_NEWFRONT_ICONS_LORES_SKILL_TAXING,
													RES_NEWFRONT_ICONS_LORES_SKILL_MAYHEM,
													RES_NEWFRONT_ICONS_LORES_OKAY,
													RES_NEWFRONT_ICONS_LORES_FULLSCREEN};

// FUNCTION: LEMBALL 0x00448200
CMainOptions1Drawer::CMainOptions1Drawer(CMain2DDisplay* p_arg0, CGDI* p_arg1, const CVsRect& p_arg2)
	: CBaseFrontendDrawer(p_arg0, p_arg1, p_arg2, FLOW_MAIN_OPTIONS_1, 0, 0, 0, 0, 0)
{
	m_idleDeadline = timeGetTime() + 20000;
	m_toggleResolutionButton = 0;
	m_navigationButton = 0;
	m_auxButtonState1 = 0;
	m_auxButtonState0 = 0;
	m_previousModeButton = 0;
	m_nextModeButton = 0;
	switch (g_pGameStatus->m_skill) {
	case 1:
		m_selectedDisplayMode = 1;
		break;
	case 2:
		m_selectedDisplayMode = 2;
		break;
	case 3:
		m_selectedDisplayMode = 3;
		break;
	case 0:
		m_selectedDisplayMode = 0;
		break;
	case 4:
		m_selectedDisplayMode = 0;
		break;
	}
	g_pGameStatus->m_lastLevels[4] = 0;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	Setup();
}

// FUNCTION: LEMBALL 0x00448300
void CMainOptions1Drawer::Load()
{
	unsigned long* previousModeAnim;
	unsigned long* playAnim;
	unsigned long* nextModeAnim;
	unsigned long* toggleAnim;
	unsigned long* modeAnim;
	unsigned long* quitAnim;
	int i;
	tagPRIMS* primitiveBundle;

	if (m_mode != 0) {
		m_buttonLayout = g_anMainOptions1CompactButtonLayout;
		previousModeAnim = &g_dwMainOptions1CompactAnimIds[0];
		nextModeAnim = &g_dwMainOptions1CompactAnimIds[1];
		toggleAnim = &g_dwMainOptions1CompactAnimIds[11];
		playAnim = &g_dwMainOptions1CompactAnimIds[2];
		modeAnim = &g_dwMainOptions1CompactAnimIds[6];
		quitAnim = &g_dwMainOptions1CompactAnimIds[3];
	}
	else {
		m_buttonLayout = g_anMainOptions1ButtonLayout;
		previousModeAnim = &g_dwMainOptions1AnimIds[0];
		nextModeAnim = &g_dwMainOptions1AnimIds[1];
		toggleAnim = &g_dwMainOptions1AnimIds[10];
		playAnim = &g_dwMainOptions1AnimIds[2];
		modeAnim = &g_dwMainOptions1AnimIds[5];
		quitAnim = &g_dwMainOptions1AnimIds[4];
	}
	primitiveBundle = m_primitiveBundle;
	i = 1;
	do {
		CResBITMAP* bitmap = m_backgroundBitmap;
		primitiveBundle->m_primitive.m_x = (short) (((int) m_display->m_rect.m_width - (int) bitmap->m_x) / 2);
		primitiveBundle->m_primitive.m_y = 0;
		primitiveBundle->m_primitive.m_resource = bitmap;
		primitiveBundle->m_primitive.m_flags = 0x800;
		primitiveBundle->m_primitive.m_remap = 0;
		primitiveBundle++;
	} while (--i != 0);
	m_gunController = new CGunController((CGWnd*) m_display, m_gdi, 6, m_mode);
	m_gunController->AddButton(m_buttonLayout[0],
							   m_buttonLayout[1],
							   previousModeAnim,
							   1,
							   0,
							   0,
							   0,
							   &m_previousModeButton,
							   0xacef00a6);
	m_gunController
		->AddButton(m_buttonLayout[2], m_buttonLayout[3], playAnim, 1, 0, 0, 0, &m_navigationButton, 0xacef0001);
	m_gunController
		->AddButton(m_buttonLayout[4], m_buttonLayout[5], nextModeAnim, 1, 0, 0, 0, &m_nextModeButton, 0xacef00a7);
	m_gunController->AddButton(m_buttonLayout[8],
							   m_buttonLayout[9],
							   toggleAnim,
							   1,
							   0,
							   0,
							   0,
							   &m_toggleResolutionButton,
							   0xacef00a5);
	m_gunController
		->AddButton(m_buttonLayout[6], m_buttonLayout[7], modeAnim, 0, 0, 3, 0, &m_selectedDisplayMode, 0xffffffff);
	m_gunController
		->AddButton(m_buttonLayout[10], m_buttonLayout[11], quitAnim, 1, 0, 0, 0, &m_navigationButton, 0xacef00a4);
	m_gunController->SetGun(0);
	m_gunController->SetSpriteWindow();
}

// FUNCTION: LEMBALL 0x00448540
void CMainOptions1Drawer::UnLoad()
{
	if (m_gunController != 0) {
		delete m_gunController;
	}
}

// FUNCTION: LEMBALL 0x00448560
CMainOptions1Drawer::~CMainOptions1Drawer()
{
	CGameStatus* status;

	switch (m_selectedDisplayMode) {
	case 0:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[0];
		status->m_skill = 0;
		break;
	case 1:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[1];
		status->m_skill = 1;
		break;
	case 2:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[2];
		status->m_skill = 2;
		break;
	case 3:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[3];
		status->m_skill = 3;
		break;
	}
	g_nDisplayMode = m_selectedDisplayMode;
	if (m_loaded != 0) {
		UnLoad();
	}
}

// FUNCTION: LEMBALL 0x00448610
void CMainOptions1Drawer::DrawBackGround()
{
}

// FUNCTION: LEMBALL 0x00448620
bool CMainOptions1Drawer::ProcessMessages(Message* p_message)
{
	int type;
	int mode;
	CGameStatus* status;

	type = p_message->type;
	switch (type) {
	case 3:
	case 4:
		m_idleDeadline = timeGetTime() + 20000;
		break;
	case 0xc:
		m_idleDeadline = timeGetTime() + 20000;
		switch (p_message->code) {
		case 0xacef0001:
			m_returnState = 3;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case 0xacef00a4:
			m_returnState = 0x10;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case 0xacef00a5:
			m_display->ToggleResolution();
			return true;
		case 0xacef00a6:
		case 0xacef00a7: {
			mode = m_selectedDisplayMode;
			status = g_pGameStatus;
			status->m_level = status->m_lastLevels[mode];
			status->m_skill = mode;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			if (p_message->code == 0xacef00a6) {
				m_returnState = 4;
				return true;
			}
			m_returnState = 0xc;
			return true;
		}
		}
		break;
	default:
		m_processedCount = m_processedCount + 1;
		return false;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00448800
void CMainOptions1Drawer::Processing()
{
	unsigned long now;

	if (g_nTestAllLevels != 0) {
		CGameStatus* status = g_pGameStatus;
		status->m_level = status->m_lastLevels[0];
		status->m_skill = 0;
		m_quitYet = 1;
		m_returnState = 4;
	}
	now = timeGetTime();
	if (m_display->IsWindowValid() == 0 || m_display->IsFocusWindow() == 0) {
		m_idleDeadline = timeGetTime() + 20000;
		return;
	}
	if (now <= m_idleDeadline) {
		return;
	}
	m_quitYet = 1;
	if (g_nFrontendAutoFlowToggle == 0 && g_nAnimationsDisabled == 0) {
		m_returnState = 1;
		g_nFrontendAutoFlowToggle = 1;
		return;
	}
	m_returnState = 0x13;
	g_nFrontendAutoFlowToggle = 0;
}
