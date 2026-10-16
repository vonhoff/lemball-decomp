#include "CMainOptions1Drawer.h"

#include <stddef.h>

#include "Application/CGameStatus.h"
#include "Application/FlowProcesses.h"
#include "Application/GameMain.h"
#include "Engine/Graphics/Primitives/CBigBitmap.h"
#include "Engine/Graphics/Primitives/CBitmap.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Resources/ResourceCatalog.h"
#include "Engine/Resources/Types/CResBITMAP.h"
#include "Platform/Windows/VsTime.h"
#include "Frontend/CBaseFrontendProcess.h"
#include "Frontend/Controls/CGunController.h"
#include "Frontend/Controls/ControlMessages.h"
#include "Frontend/FrontendLayoutMode.h"
#include "Frontend/tagPRIMS.h"
#include "GameView/Display/CMain2DDisplay.h"
#include "Level/CLevelLoader.h"
#include "Engine/Queues/tagMESSAGE.h"

class CGWnd;

// GLOBAL: LEMBALL 0x0049f490
int g_anMainOptions1ButtonLayout[12] = {32, 116, 480, 116, 48, 232, 464, 232, 80, 348, 416, 348};

// GLOBAL: LEMBALL 0x0049f4c0
int g_anMainOptions1CompactButtonLayout[12] = {16, 58, 240, 58, 24, 116, 232, 116, 40, 174, 208, 174};

// GLOBAL: LEMBALL 0x0049f4f4
unsigned long g_dwMainOptions1AnimIds[11] = {RES_NEWFRONT_ICONS_HIRES_ONE_PLAYER,
											 RES_NEWFRONT_ICONS_HIRES_TWO_PLAYER,
											 RES_NEWFRONT_ICONS_HIRES_OPTIONS,
											 RES_NEWFRONT_ICONS_HIRES_FLOPPY_DISK,
											 RES_NEWFRONT_ICONS_HIRES_PASSWORD,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_FUN,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_TRICKY,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_TAXING,
											 RES_NEWFRONT_ICONS_HIRES_SKILL_MAYHEM,
											 RES_NEWFRONT_ICONS_HIRES_OKAY,
											 RES_NEWFRONT_ICONS_HIRES_FULLSCREEN};

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
CMainOptions1Drawer::CMainOptions1Drawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_MAIN_OPTIONS_1, 0, 0, 0, 0, 0)
{
	m_idleDeadline = CurrentMilliTimer() + MAIN_OPTIONS1_IDLE_TIMEOUT_MS;
	m_toggleResolutionButton = 0;
	m_navigationButton = 0;
	m_auxButtonState1 = 0;
	m_auxButtonState0 = 0;
	m_singlePlayerButtonState = 0;
	m_multiplayerButtonState = 0;
	switch (g_pGameStatus->m_skill) {
	case SKILL_TRICKY:
		m_selectedSkill = SKILL_TRICKY;
		break;
	case SKILL_TAXING:
		m_selectedSkill = SKILL_TAXING;
		break;
	case SKILL_MAYHEM:
		m_selectedSkill = SKILL_MAYHEM;
		break;
	case SKILL_FUN:
		m_selectedSkill = SKILL_FUN;
		break;
	case SKILL_NETWORK:
		m_selectedSkill = SKILL_FUN;
		break;
	}
	g_pGameStatus->m_lastLevels[4] = 0;
	m_drawBackground = true;
	m_drawFrame = true;
	m_drawSolid = true;
	Setup();
}

// FUNCTION: LEMBALL 0x00448300
void CMainOptions1Drawer::Load()
{
	unsigned long* singlePlayerAnim;
	unsigned long* optionsAnim;
	unsigned long* multiplayerAnim;
	unsigned long* resolutionAnim;
	unsigned long* skillAnim;
	unsigned long* passwordAnim;
	int i;

	if (m_mode != FRONTEND_LAYOUT_STANDARD) {
		m_buttonLayout = g_anMainOptions1CompactButtonLayout;
		singlePlayerAnim = &g_dwMainOptions1CompactAnimIds[0];
		multiplayerAnim = &g_dwMainOptions1CompactAnimIds[1];
		resolutionAnim = &g_dwMainOptions1CompactAnimIds[11];
		optionsAnim = &g_dwMainOptions1CompactAnimIds[2];
		skillAnim = &g_dwMainOptions1CompactAnimIds[6];
		passwordAnim = &g_dwMainOptions1CompactAnimIds[3];
	}
	else {
		m_buttonLayout = g_anMainOptions1ButtonLayout;
		singlePlayerAnim = &g_dwMainOptions1AnimIds[0];
		multiplayerAnim = &g_dwMainOptions1AnimIds[1];
		resolutionAnim = &g_dwMainOptions1AnimIds[10];
		optionsAnim = &g_dwMainOptions1AnimIds[2];
		skillAnim = &g_dwMainOptions1AnimIds[5];
		passwordAnim = &g_dwMainOptions1AnimIds[4];
	}
	for (i = 0; i < 1; i++) {
		CResBITMAP* bitmap = m_backgroundBitmap;
		m_primitiveBundle[i].m_primitive.m_x = (short) ((m_display->m_rect.m_width - bitmap->m_rasterPoint.m_x) / 2);
		m_primitiveBundle[i].m_primitive.m_y = 0;
		m_primitiveBundle[i].m_primitive.m_resource = bitmap;
		m_primitiveBundle[i].m_primitive.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		m_primitiveBundle[i].m_primitive.m_remap = NULL;
	}
	m_gunController = new CGunController((CGWnd*) m_display, m_gdi, 6, m_mode);
	m_gunController->AddButton(m_buttonLayout[0],
							   m_buttonLayout[1],
							   singlePlayerAnim,
							   GUN_BUTTON_POST_ACTION_MESSAGE,
							   0,
							   0,
							   0,
							   &m_singlePlayerButtonState,
							   MAIN_OPTIONS1_BUTTON_MESSAGE_PREVIEW);
	m_gunController->AddButton(m_buttonLayout[2],
							   m_buttonLayout[3],
							   optionsAnim,
							   GUN_BUTTON_POST_ACTION_MESSAGE,
							   0,
							   0,
							   0,
							   &m_navigationButton,
							   MAIN_OPTIONS1_BUTTON_MESSAGE_OPTIONS);
	m_gunController->AddButton(m_buttonLayout[4],
							   m_buttonLayout[5],
							   multiplayerAnim,
							   GUN_BUTTON_POST_ACTION_MESSAGE,
							   0,
							   0,
							   0,
							   &m_multiplayerButtonState,
							   MAIN_OPTIONS1_BUTTON_MESSAGE_NETWORK);
	m_gunController->AddButton(m_buttonLayout[8],
							   m_buttonLayout[9],
							   resolutionAnim,
							   GUN_BUTTON_POST_ACTION_MESSAGE,
							   0,
							   0,
							   0,
							   &m_toggleResolutionButton,
							   MAIN_OPTIONS1_BUTTON_MESSAGE_RESOLUTION);
	m_gunController->AddButton(m_buttonLayout[6],
							   m_buttonLayout[7],
							   skillAnim,
							   GUN_BUTTON_CYCLE_VALUE,
							   SKILL_FUN,
							   SKILL_MAYHEM,
							   SKILL_FUN,
							   &m_selectedSkill,
							   BUTTON_ACTION_MESSAGE_UNASSIGNED);
	m_gunController->AddButton(m_buttonLayout[10],
							   m_buttonLayout[11],
							   passwordAnim,
							   GUN_BUTTON_POST_ACTION_MESSAGE,
							   0,
							   0,
							   0,
							   &m_navigationButton,
							   MAIN_OPTIONS1_BUTTON_MESSAGE_PASSWORD);
	m_gunController->SetGun(0);
	m_gunController->SetSpriteWindow();
}

// FUNCTION: LEMBALL 0x00448540
void CMainOptions1Drawer::UnLoad()
{
	if (m_gunController != NULL) {
		delete m_gunController;
	}
}

// FUNCTION: LEMBALL 0x00448560
CMainOptions1Drawer::~CMainOptions1Drawer()
{
	CGameStatus* status;

	switch (m_selectedSkill) {
	case SKILL_FUN:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[SKILL_FUN];
		status->m_skill = SKILL_FUN;
		break;
	case SKILL_TRICKY:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[SKILL_TRICKY];
		status->m_skill = SKILL_TRICKY;
		break;
	case SKILL_TAXING:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[SKILL_TAXING];
		status->m_skill = SKILL_TAXING;
		break;
	case SKILL_MAYHEM:
		status = g_pGameStatus;
		status->m_level = status->m_lastLevels[SKILL_MAYHEM];
		status->m_skill = SKILL_MAYHEM;
		break;
	}
	g_nSelectedSkill = m_selectedSkill;
	if (m_loaded != 0) {
		UnLoad();
	}
}

// FUNCTION: LEMBALL 0x00448610
void CMainOptions1Drawer::DrawBackGround()
{
}

// FUNCTION: LEMBALL 0x00448620
bool CMainOptions1Drawer::ProcessMessages(tagMESSAGE* p_message)
{
	int type;
	int skill;
	CGameStatus* status;

	type = p_message->m_type;
	switch (type) {
	case MESSAGE_KEY_UP:
	case MESSAGE_KEY_DOWN:
		m_idleDeadline = CurrentMilliTimer() + MAIN_OPTIONS1_IDLE_TIMEOUT_MS;
		break;
	case MESSAGE_BUTTON_RELEASED:
		m_idleDeadline = CurrentMilliTimer() + MAIN_OPTIONS1_IDLE_TIMEOUT_MS;
		switch ((unsigned int) p_message->m_code) {
		case MAIN_OPTIONS1_BUTTON_MESSAGE_OPTIONS:
			m_returnState = FLOW_MAIN_OPTIONS_2;
			m_quitYet = true;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case MAIN_OPTIONS1_BUTTON_MESSAGE_PASSWORD:
			m_returnState = FLOW_PASSWORD;
			m_quitYet = true;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case MAIN_OPTIONS1_BUTTON_MESSAGE_RESOLUTION:
			m_display->ToggleResolution();
			return true;
		case MAIN_OPTIONS1_BUTTON_MESSAGE_PREVIEW:
		case MAIN_OPTIONS1_BUTTON_MESSAGE_NETWORK: {
			skill = m_selectedSkill;
			status = g_pGameStatus;
			status->m_level = status->m_lastLevels[skill];
			status->m_skill = skill;
			m_quitYet = true;
			g_nFrontendAutoFlowToggle = 1;
			if (p_message->m_code == MAIN_OPTIONS1_BUTTON_MESSAGE_PREVIEW) {
				m_returnState = FLOW_PREVIEW;
			}
			else {
				m_returnState = FLOW_NETWORK_OPTIONS;
			}
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
		status->m_skill = SKILL_FUN;
		m_quitYet = true;
		m_returnState = FLOW_PREVIEW;
	}
	now = CurrentMilliTimer();
	if (m_display->IsWindowValid() == 0 || m_display->IsFocusWindow() == 0) {
		m_idleDeadline = CurrentMilliTimer() + MAIN_OPTIONS1_IDLE_TIMEOUT_MS;
		return;
	}
	if (now <= m_idleDeadline) {
		return;
	}
	m_quitYet = true;
	if (g_nFrontendAutoFlowToggle == 0 && g_nAnimationsDisabled == 0) {
		m_returnState = FLOW_INTRO_ANIM;
		g_nFrontendAutoFlowToggle = 1;
		return;
	}
	m_returnState = FLOW_DEMO;
	g_nFrontendAutoFlowToggle = 0;
}
