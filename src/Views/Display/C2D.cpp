#include "C2D.h"

#include "Gameplay/Simulation/CAI.h"
#include "Game/CDemo.h"
#include "Game/CGame.h"
#include "Game/GameMain.h"
#include "Game/GameTime.h"
#include "Level/CLevelLoader.h"
#include "Map/CMap.h"
#include "Network/CNetworkManager.h"
#include "Visos/Queues/CBaseQueue.h"
#include "CObjSq.h"
#include "Visos/Text/CTextManager.h"
#include "Visos/Graphics/Palettes/CBasePalManager.h"
#include "Platform/Windows/Graphics/CCursor.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Controls/CHotAreaList.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Network/CBaseNetwork.h"
#include "Visos/Network/NetworkConstants.h"
#include "Visos/Network/NetworkMode.h"
#include "Visos/Resources/Types/CResPALETTE.h"
#include "Visos/Resources/ResourceLimits.h"
#include "Views/Animation/CLemmingAnimsManager.h"
#include "Views/Input/CPadToButton.h"
#include "Views/Panel/CPanel.h"
#include "Views/Pause/CPauseWindow.h"
#include "ObjectClipGrid.h"
#include "SpriteGroundLookup.h"
#include "CMain2DDisplay.h"
#include "CPBButton.h"
#include "Frontend/FlowProcesses.h"
#include "Visos/Math/FixedPoint.h"

#include <new.h>
#include <string.h>

#include "Game/CGameStatus.h"

#include "Visos/Graphics/Palettes/CBaseRemap.h"

class CBaseQueueHandler;
class CRemap;

#include "Visos/Streams/CVSOStream.h"
#include "Visos/Network/CConnect.h"

#include <stddef.h>

#include "Visos/Resources/Manifest.h"

enum eLemmingPaletteIndex {
	LEMMING_RED_SHADOW = 0x37,
	LEMMING_BLUE_SHADOW = 0x52,
	LEMMING_RED_RAMP_DARK = 0x5c,
	LEMMING_RED_RAMP_LIGHT = 0x71,
	LEMMING_RED_RAMP_HIGHLIGHT = 0x75,
	LEMMING_BLUE_RAMP = 0x74,
	LEMMING_RED_HIGHLIGHT = 0x80,
	LEMMING_BLUE_HIGHLIGHT = 0x8c
};

enum {
	C2D_VIEW_DATA_CAPACITY = 200
};

enum eLevelTestFrame {
	LEVEL_TEST_FRAME_RESOLUTION_TOGGLE_FIRST = 50,
	LEVEL_TEST_FRAME_RESOLUTION_TOGGLE_SECOND = 51,
	LEVEL_TEST_FRAME_RESOLUTION_TOGGLE_THIRD = 52,
	LEVEL_TEST_FRAME_COMPLETE = 53
};

extern int g_anC2DRemapSourceIndices[17];

extern int g_anC2DRemapTargetIndices[4][17];

extern unsigned char g_abC2DType2Remap[5];

// GLOBAL: LEMBALL 0x0049e8b8
int g_anC2DRemapSourceIndices[17] = {250, 204, 205, 206, 118, 107, 101, 95, 85, 75, 69, 59, 49, 46, 44, 37, 48};

// GLOBAL: LEMBALL 0x0049e8fc
int g_anC2DRemapTargetIndices[4][17] = {
	{224, 225, 226, 227, 228, 229, 230, 231, 232, 232, 233, 234, 234, 234, 234, 235, 235},
	{192, 193, 194, 195, 196, 197, 198, 199, 200, 200, 201, 202, 202, 202, 202, 203, 203},
	{208, 209, 210, 211, 212, 213, 214, 215, 216, 216, 217, 218, 218, 218, 218, 219, 219},
	{179, 180, 181, 182, 183, 184, 185, 186, 187, 187, 188, 189, 189, 189, 189, 190, 191}};

// GLOBAL: LEMBALL 0x0049ea28
unsigned char g_abC2DType2Remap[5] = {2, 241, 81, 168, 108};

// FUNCTION: LEMBALL 0x004358d0
C2D::C2D(CMain2DDisplay* p_display, CAI* p_ai, CGDI* p_gdi, CMap* p_map, const CVSRect& p_rect)
	: CHotAreaHandler(p_rect)
{
	void* storage;
	CBaseQueueHandler* queueHandler;
	ObjectClipGrid* objectClipGrid;
	unsigned int cellCount;
	int groundWidth;
	int groundHeight;

	m_frameCount = 0;
	m_frameTime = 0;
	m_paused = 0;
	m_pauser = 0;
	m_connectionTimeoutActive = 0;
	m_pad0x920 = 0;
	m_mouseButtonDown = 0;
	m_cursorBlinkPhase = 0;
	m_zBufferEnabled = 1;
	InitSpriteGroundLU();
	m_groundHitMode = 0;
	m_pauseWindow = NULL;
	m_optionSelection = 0;
	m_cursorState = C2D_CURSOR_STATE_GROUND;
	m_cursorTimestamp = g_dwSimulationTimestamp;
	m_returnState = FLOW_MAIN_OPTIONS_1;
	if (g_nTestAllLevels != 0) {
		m_levelTestFrame = 0;
	}
	m_ai = p_ai;
	m_gdi = p_gdi;
	m_display = p_display;
	m_map = p_map;
	m_viewOriginX = 0;
	m_viewOriginY = 0;
	m_viewOrientation = 0;
	p_map->m_orientation = MAP_ORIENTATION_ROTATION_0_DEGREES;
	m_groupCount = 0;
	m_quitRequested = 0;
	m_groupSelectionCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	m_externalEnabled = 1;
	m_lemmingManager = m_ai->m_aiQueue;

	m_display->m_hotAreaList->AddToList(this);

	storage = operator new(sizeof(CLemmingAnimsManager));
	if (storage != NULL) {
		m_lemmingAnims = new (storage) CLemmingAnimsManager(m_gdi, m_display, m_ai);
	}
	else {
		m_lemmingAnims = NULL;
	}

	storage = operator new(sizeof(CTextManager));
	if (storage != NULL) {
		m_textManager = new (storage) CTextManager(RESOURCE_ID_COUNT, 2, 2, 10);
	}
	else {
		m_textManager = NULL;
	}

	ResetPrimitives();
	RegisterRemaps();
	m_cursorState = C2D_CURSOR_STATE_GROUND;
	m_clipConfigured = 1;
	m_viewDataCount = 0;

	m_viewData = new CViewData[C2D_VIEW_DATA_CAPACITY];

	m_zBuffer = (unsigned char*) operator new(0x800);
	queueHandler = this;
	g_pMasterInputQueue->Attach(queueHandler, 0);
	ClockEditMode(0);

	storage = operator new(sizeof(CPadToButton));
	if (storage != NULL) {
		m_padToButton = new (storage) CPadToButton(3);
	}
	else {
		m_padToButton = NULL;
	}

	m_unk0xc90 = 0;
	m_primitiveCount = 0;
	SetUpRemapPalettes();
	m_textManager->LoadFont(RES_NEWFRONT_FONTS_GAME_SCORETIME);
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		m_textManager->LoadFont(0xf8);
	}

	m_spriteGroundLookup = NULL;
	objectClipGrid = (ObjectClipGrid*) operator new(sizeof(ObjectClipGrid));
	if (objectClipGrid != NULL) {
		groundHeight = m_map->m_ground.m_height << GROUND_BLOCK_PIXEL_SHIFT;
		groundWidth = m_map->m_ground.m_width << GROUND_BLOCK_PIXEL_SHIFT;
		objectClipGrid->m_touchedCount = 0;
		objectClipGrid->m_cells = NULL;
		objectClipGrid->m_cellWidth = 0x10;
		objectClipGrid->m_cellHeight = 0x10;
		if (objectClipGrid->m_cells != NULL) {
			operator delete(objectClipGrid->m_cells);
			objectClipGrid->m_cells = NULL;
		}
		objectClipGrid->m_width =
			(short) ((groundWidth + objectClipGrid->m_cellWidth - 1) / objectClipGrid->m_cellWidth);
		objectClipGrid->m_height =
			(short) ((groundHeight + objectClipGrid->m_cellHeight - 1) / objectClipGrid->m_cellHeight);
		cellCount = (unsigned int) (int) objectClipGrid->m_width * (unsigned int) (int) objectClipGrid->m_height;
		objectClipGrid->m_cellCount = cellCount;
		objectClipGrid->m_cells = new CObjSq[cellCount];
		m_objectClipGrid = objectClipGrid;
	}
	else {
		m_objectClipGrid = NULL;
	}

	m_panel = NULL;
	m_pad0x8c8 = 0;
	m_pad0x8cc = 0;
	m_groundWidth = (unsigned short) m_map->m_ground.m_width;
	m_groundHeight = (unsigned short) m_map->m_ground.m_height;
	m_viewSize.m_x = p_rect.m_width;
	m_viewSize.m_y = p_rect.m_height;
	m_zoom = (unsigned short) m_display->m_zoom;
	m_viewSize.m_x = (short) ((int) m_viewSize.m_x / (int) (unsigned int) m_zoom);
	m_viewSize.m_y = (short) ((int) m_viewSize.m_y / (int) (unsigned int) m_zoom);
	m_clipSize.m_x = m_viewSize.m_x;
	m_clipSize.m_y = m_viewSize.m_y;
	m_scoreTimestamp = g_dwGameTick;
	{
		int score = m_ai->m_score;
		m_score = score;
		m_levelScore = score;
	}
}

// FUNCTION: LEMBALL 0x00436050
C2D::~C2D()
{
	if (g_nTestAllLevels != 0) {
		g_nZoomEnabled = g_nZoomEnabled == 0;
	}
}

// FUNCTION: LEMBALL 0x00436190
void C2D::ShutDown()
{
	ObjectClipGrid* objectClipGrid;
	SpriteGroundLookup* spriteGroundLookup;
	CLemmingAnimsManager* lemmingAnims;
	unsigned long started;
	unsigned long now;

	m_gdi->m_renderTarget->EnableZBuff(0);
	m_textManager->UnLoadFont(RES_NEWFRONT_FONTS_GAME_SCORETIME);
	delete m_textManager;
	if (m_panel != NULL) {
		delete m_panel;
		m_panel = NULL;
	}
	if (m_pauseWindow != NULL) {
		CPauseWindow& pauseWindow = *m_pauseWindow;
		delete &pauseWindow;
		m_pauseWindow = NULL;
	}
	KillRemapPalettes();
	objectClipGrid = m_objectClipGrid;
	if (objectClipGrid != NULL) {
		operator delete(objectClipGrid->m_cells);
		operator delete(objectClipGrid);
	}
	spriteGroundLookup = m_spriteGroundLookup;
	if (spriteGroundLookup != NULL) {
		operator delete(spriteGroundLookup->m_maskA);
		operator delete(spriteGroundLookup->m_maskB);
		operator delete(spriteGroundLookup);
	}
	if (m_padToButton != NULL) {
		delete m_padToButton;
	}
	g_pMasterInputQueue->Detach(this, 0);
	CMain2DDisplay& display = *m_display;
	if (display.m_lifecycleRefs == 1) {
		display.m_hotAreaList->RemoveFromList(this);
	}
	operator delete(m_zBuffer);
	operator delete(m_viewData);
	CursorChangeType(0, 0);
	lemmingAnims = m_lemmingAnims;
	if (lemmingAnims != NULL) {
		lemmingAnims->~CLemmingAnimsManager();
		operator delete(lemmingAnims);
	}
	UnRegisterRemaps();
	CPBButton::DumpStrs();
	if (m_ai->m_networkMode != NETWORK_MODE_SINGLE_PLAYER && m_returnState == FLOW_MAIN_OPTIONS_1) {
		if (g_pNetworkManager != NULL) {
			g_pNetworkManager->Stop();
		}
		if (g_pBaseNetwork != NULL) {
			started = CurrentMilliTimer();
			do {
				now = CurrentMilliTimer();
				if (now - started >= NETWORK_QUEUE_TRANSITION_TIMEOUT_MS) {
					break;
				}
			} while (g_pBaseNetwork->m_queueTransitionPending != 0);
		}
		if (g_pNetworkManager != NULL) {
			delete g_pNetworkManager;
			g_pNetworkManager = NULL;
		}
	}
	m_display->m_gdi->m_renderTarget->SetWorldWidth(0);
	CVSRect rect;
	m_display->SetInnerWindow(rect);
}

// FUNCTION: LEMBALL 0x004363c0
void C2D::RegisterRemaps()
{
	CResPALETTE* palette;
	int paletteSize;
	int* targets;
	int remapIndex;

	targets = g_anC2DRemapTargetIndices[0];
	palette = CResPALETTE::Load(RES_GAME_GAMEPALETTE);
	paletteSize = (int) palette->m_entryCount;
	remapIndex = 0;
	do {
		m_remapTables[remapIndex] = (unsigned char*) operator new(paletteSize);
		int i = 0;
		if (paletteSize > 0) {
			do {
				m_remapTables[remapIndex][i] = (unsigned char) i;
				i = i + 1;
			} while (i < paletteSize);
		}

		int* sources = g_anC2DRemapSourceIndices;
		do {
			int target = *targets;
			int source = *sources;
			if (target != 0) {
				m_remapTables[remapIndex][source] = (unsigned char) target;
			}
			sources = sources + 1;
			targets = targets + 1;
		} while (sources < g_anC2DRemapSourceIndices + 17);

		CBaseRemap* remap =
			g_pBasePalManager->RegisterRemap(RES_GAME_GAMEPALETTE, m_remapTables[remapIndex], PALETTE_DEFAULT);
		m_remaps[remapIndex] = remap;
		remapIndex = remapIndex + 1;
	} while (remapIndex < 4);

	m_remaps[4] = g_pBasePalManager->RegisterRemap(RES_GAME_GAMEPALETTE, g_abC2DType2Remap, PALETTE_MAPPED);
	palette->UnLoad();
}

// FUNCTION: LEMBALL 0x00436480
void C2D::UnRegisterRemaps()
{
	int i;
	for (i = 0; i < 5; i++) {
		g_pBasePalManager->UnRegisterRemap(m_remaps[i]);
	}
}

// FUNCTION: LEMBALL 0x004364d0
void C2D::OnLoaded()
{
	int zoom;
	unsigned int zoomDivisor;
	unsigned int oldZoom;
	CPanel* panel;

	CursorChangeType(CURSOR_DISPLAY_HAND, CURSOR_HAND_FRAME_DEFAULT);
	if (g_nCompactPrimaryContextLayout != 0 || g_nEditLevelMode != 0 || g_nZoomEnabled != 0) {
		zoom = 1;
	}
	else {
		zoom = 2;
	}

	oldZoom = m_zoom;
	m_zoom = (unsigned short) zoom;
	zoomDivisor = (unsigned short) zoom;
	CVSRect* displayRect = &m_display->m_rect;
	m_viewSize.m_x = displayRect->m_width;
	m_viewSize.m_y = displayRect->m_height;
	m_viewSize.m_x = (short) ((int) m_viewSize.m_x / (int) zoomDivisor);
	m_viewSize.m_y = (short) ((int) m_viewSize.m_y / (int) zoomDivisor);
	SetClipSize();

	if (m_viewSize.m_x != m_clipSize.m_x || m_viewSize.m_y != m_clipSize.m_y) {
		CVSRect innerRect((short) m_clipOffsetX, (short) m_clipOffsetY, m_clipSize.m_x, m_clipSize.m_y);
		m_display->SetInnerWindow(innerRect);
	}

	panel = (CPanel*) operator new(0x58);
	if (panel != NULL) {
		m_panel = new (panel) CPanel(this);
	}
	else {
		m_panel = NULL;
	}

	if (oldZoom != (unsigned int) zoom) {
		m_display->SetZoom(m_zoom);
	}
	else {
		OnSize(m_display->m_rect);
	}

	m_display->m_gdi->m_renderTarget->SetWorldWidth(3000);
	m_gdi->m_renderTarget->EnableZBuff(1);
	m_redrawPending = 1;
	m_scrollPending = 0;
	if (g_pDemo != NULL) {
		g_pDemo->m_window = m_display;
	}
	m_display->Clear(0);
	m_ai->Start();
}

// FUNCTION: LEMBALL 0x004366a0
void C2D::OnZoom(const CVSRect& p_rect)
{
}

// FUNCTION: LEMBALL 0x004366b0
void C2D::OnSize(const CVSRect& p_rect)
{
	unsigned int zoomDivisor;

	if (m_display->GetSizeStatus() == 0) {
		if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
			g_pDemo->GameIsOver();
			return;
		}
	}
	else if (m_display->IsWindowValid()) {
		m_viewSize.m_x = p_rect.m_width;
		m_viewSize.m_y = p_rect.m_height;
		zoomDivisor = m_zoom;
		m_viewSize.m_x = (short) ((int) m_viewSize.m_x / (int) zoomDivisor);
		m_viewSize.m_y = (short) ((int) m_viewSize.m_y / (int) zoomDivisor);
		DoButtons();
		m_redrawPending = 1;
		if (m_panel != NULL) {
			m_panel->OnSize();
		}
	}
}

// FUNCTION: LEMBALL 0x00436760
void C2D::SetUpRemapPalettes()
{
	enum {
		C2D_PALETTE_REMAP_ENTRY_COUNT = 256
	};
	unsigned char* mapping = (unsigned char*) operator new(C2D_PALETTE_REMAP_ENTRY_COUNT);
	int value;
	int i = 0;
	do {
		switch (i) {
		case LEMMING_RED_SHADOW:
			value = LEMMING_BLUE_SHADOW;
			break;
		case LEMMING_RED_RAMP_DARK:
		case LEMMING_RED_RAMP_LIGHT:
		case LEMMING_RED_RAMP_HIGHLIGHT:
			value = LEMMING_BLUE_RAMP;
			break;
		case LEMMING_RED_HIGHLIGHT:
			value = LEMMING_BLUE_HIGHLIGHT;
			break;
		default:
			value = i;
			break;
		}
		mapping[i] = value;
		i++;
	} while (i < C2D_PALETTE_REMAP_ENTRY_COUNT);
	m_paletteRemap = g_pBasePalManager->RegisterRemap(RES_GAME_GAMEPALETTE, mapping, PALETTE_DEFAULT);
}

// FUNCTION: LEMBALL 0x00436830
void C2D::KillRemapPalettes()
{
	g_pBasePalManager->UnRegisterRemap(m_paletteRemap);
}

// FUNCTION: LEMBALL 0x00436850
void C2D::Restart()
{
	m_viewOriginX = 0;
	m_viewOriginY = 0;
	m_redrawPending = 1;
	m_panel->SetPause(0);
	m_ai->Restart();
	m_display->Clear(0);
	m_scoreTimestamp = g_dwGameTick;
	g_pGameStatus->m_levelState = (unsigned int) m_levelScore;
	m_ai->m_score = m_levelScore;
	m_ai->Start();
	m_score = m_ai->m_score;
}

// FUNCTION: LEMBALL 0x00436a10
void C2D::Process()
{
	CheckValidFormGroup();
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0 && g_pDemo->m_gameOver != 0) {
		m_ai->GameState(GAME_STATUS_SUCCESS);
	}
	if (m_connectionTimeoutActive != 0) {
		if (CurrentMilliTimer() - m_connectionTimeoutStart >= CONNECTION_LOST_RETURN_DELAY_MS) {
			m_quitRequested = 1;
			m_returnState = FLOW_MAIN_OPTIONS_1;
		}
		return;
	}
	if (m_lemmingAnims->m_loaded == 0) {
		m_lemmingAnims->Load(m_ai->m_mapType);
		OnLoaded();
	}
	SetMouseShape();
	m_panel->Process();
	if (m_ai->m_started == 0 && m_ai->m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		if (m_pauseMessage != PAUSE_MSG_PLEASE_WAIT) {
			NewPauseWindow(PAUSE_MSG_PLEASE_WAIT);
		}
	}
	else if (m_pauseMessage == PAUSE_MSG_PLEASE_WAIT) {
		NewPauseWindow(PAUSE_MSG_NONE);
	}

	if (m_optionSelection != 0) {
		switch (m_pauseMessage) {
		case PAUSE_MSG_PAUSED:
			switch (m_optionSelection) {
			case PAUSE_OPTION_RESUME:
				if (m_pauser != 0) {
					m_panel->SetPause(0);
				}
				break;
			case PAUSE_OPTION_RESTART:
				NewPauseWindow(PAUSE_MSG_ARE_YOU_SURE);
				break;
			case PAUSE_OPTION_QUIT:
				NewPauseWindow(PAUSE_MSG_ARE_YOU_SURE);
				break;
			default:
				*g_pErrorOutput << "Unknown Pause window option selected\n";
				break;
			}
			break;
		case PAUSE_MSG_LOADING:
			m_optionSelection = 0;
			goto optionHandled;
		case PAUSE_MSG_ARE_YOU_SURE:
			*g_pErrorOutput << "Confirmed\n";
			if (m_optionSelection == PAUSE_CONFIRM_YES) {
				*g_pErrorOutput << "Confirmed Yes\n";
				if (m_previousPauseMessage == PAUSE_MSG_PAUSED) {
					*g_pErrorOutput << "Confirmed Yes Pause\n";
					switch (m_pauseSelection) {
					case PAUSE_OPTION_RESTART:
						*g_pErrorOutput << "Confirmed Yes Pause Restart\n";
						m_ai->GameState(GAME_STATUS_RESTART);
						break;
					case PAUSE_OPTION_QUIT:
						*g_pErrorOutput << "Confirmed Yes Pause Quit\n";
						m_ai->QuitGame();
						break;
					default:
						*g_pErrorOutput << "Unknown Confirmation option selected\n";
						break;
					}
				}
			}
			else {
				NewPauseWindow(PAUSE_MSG_PAUSED);
			}
			break;
		}
		m_optionSelection = 0;
	}

optionHandled:
	switch (m_ai->m_gameStatus) {
	case GAME_STATUS_PAUSED:
		if (m_paused == 0 && m_ai->m_gameStatePending == 0) {
			m_panel->SetPause(1);
		}
		break;
	case GAME_STATUS_RUNNING:
		if (m_paused != 0 && m_ai->m_gameStatePending == 0) {
			m_panel->SetPause(0);
		}
		break;
	case GAME_STATUS_SUCCESS:
		if (g_nDemoMode != 0) {
			m_returnState = FLOW_MAIN_OPTIONS_1;
		}
		else {
			if (m_score != m_ai->m_score) {
				m_score = m_ai->m_score;
			}
			m_returnState = FLOW_SUCCESS;
			if (m_ai->m_networkMode == NETWORK_MODE_SINGLE_PLAYER) {
				int levelCount = g_pGameStatus->NoOfLevelsInSkill(g_pGameStatus->m_skill);
				if (levelCount == g_pGameStatus->Level()) {
					m_returnState = FLOW_LEVEL_INTRO;
					g_pGameStatus->IncSkill(1);
				}
				else {
					g_pGameStatus->IncLevel();
				}
			}
		}
		m_quitRequested = 1;
		NewPauseWindow(PAUSE_MSG_LOADING);
		m_display->RefreshView();
		break;
	case GAME_STATUS_FAILURE:
		m_quitRequested = 1;
		if (g_nDemoMode != 0) {
			m_returnState = FLOW_MAIN_OPTIONS_1;
		}
		else {
			m_returnState = FLOW_FAILURE;
		}
		m_ai->m_paused = 0;
		NewPauseWindow(PAUSE_MSG_LOADING);
		m_display->RefreshView();
		g_pGameStatus->m_levelState = m_levelScore;
		m_ai->m_score = m_levelScore;
		m_score = m_levelScore;
		break;
	case GAME_STATUS_RESTART:
		Restart();
		break;
	}

	if (m_ai->m_networkMode != NETWORK_MODE_SINGLE_PLAYER && g_pActiveConnection == NULL) {
		NewPauseWindow(PAUSE_MSG_CONNECTION_LOST);
		m_connectionTimeoutActive = 1;
		m_connectionTimeoutStart = CurrentMilliTimer();
	}
	if (g_nTestAllLevels != 0) {
		switch ((int) m_levelTestFrame) {
		case LEVEL_TEST_FRAME_RESOLUTION_TOGGLE_FIRST:
		case LEVEL_TEST_FRAME_RESOLUTION_TOGGLE_SECOND:
		case LEVEL_TEST_FRAME_RESOLUTION_TOGGLE_THIRD:
			m_display->ToggleResolution();
			break;
		case LEVEL_TEST_FRAME_COMPLETE:
			m_ai->GameState(GAME_STATUS_SUCCESS);
			break;
		}
		m_levelTestFrame++;
	}
}

// FUNCTION: LEMBALL 0x00437970
bool C2D::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
{
	CMap* initialMap;
	int maxGameX;
	int maxGameY;
	int searchY;
	int searchMinX;
	int searchMaxX;
	int searchX;
	int gameX;
	int gameY;
	int groundScreenX;
	int groundScreenY;
	int left;
	int top;
	int right;
	int bottom;
	CGround* ground;
	int hitX;
	int hitY;
	unsigned int includeSpecial;

	initialMap = m_map;
	maxGameX = initialMap->m_ground.m_width * 0x10 - 1;
	maxGameY = initialMap->m_ground.m_height * 0x10 - 1;
	searchY = p_screenY + 0x50;
	if (searchY >= p_screenY) {
		searchMinX = p_screenX - 0x20;
		searchMaxX = p_screenX + 0x20;
		do {
			searchX = searchMinX;
			if (searchMaxX >= searchX) {
				do {
					m_map->ScreenToGame(searchX, searchY, gameX, gameY);
					if (gameX >= 0 && maxGameX >= gameX && gameY >= 0 && maxGameY >= gameY) {
						gameX /= GROUND_BLOCK_PIXEL_SIZE;
						gameY /= GROUND_BLOCK_PIXEL_SIZE;

						m_map->GameToScreen(gameX << GROUND_BLOCK_PIXEL_SHIFT,
											gameY << GROUND_BLOCK_PIXEL_SHIFT,
											groundScreenX,
											groundScreenY);
						groundScreenY -= m_map->m_ground.m_ground[m_map->m_ground.m_width * gameY + gameX].m_height;

						left = groundScreenX - GROUND_BLOCK_PIXEL_SIZE;
						right = groundScreenX + GROUND_BLOCK_PIXEL_MASK;
						top = groundScreenY - GROUND_BLOCK_PIXEL_SIZE;
						bottom = groundScreenY + GROUND_BLOCK_PIXEL_MASK;
						if (left <= p_screenX && right >= p_screenX && top <= p_screenY && bottom >= p_screenY) {
							ground = m_map->m_ground.m_ground + m_map->m_ground.m_width * gameY + gameX;
							hitX = p_screenX - left;
							hitY = p_screenY - top;
							if (hitX >= 0 && hitY >= 0 && hitX <= GROUND_HIT_MASK_LAST_INDEX &&
								hitY <= GROUND_HIT_MASK_LAST_INDEX) {
								includeSpecial = m_groundHitMode >= 1;
								if (ground->IsHit(hitX, hitY, includeSpecial)) {
									p_gameX = gameX * 0x10 + 8;
									p_gameY = gameY * 0x10 + 8;
									return true;
								}
							}
						}
					}
					searchX += 0x10;
				} while (searchMaxX >= searchX);
			}
			searchY -= 8;
		} while (searchY >= p_screenY);
	}
	return false;
}

// FUNCTION: LEMBALL 0x00437d00
void C2D::NewPauseWindow(ePauseWindowMessages p_message)
{
	m_previousPauseMessage = m_pauseMessage;
	m_pauseMessage = p_message;
	if (m_pauseWindow != NULL) {
		delete m_pauseWindow;
		m_pauseWindow = NULL;
	}
	if (m_pauseMessage != PAUSE_MSG_NONE) {
		m_pauseWindow = new CPauseWindow(this, m_display, m_pauseMessage);
	}
	if (m_pauseMessage == PAUSE_MSG_ARE_YOU_SURE) {
		m_pauseSelection = m_optionSelection;
	}
}

// FUNCTION: LEMBALL 0x00437da0
void C2D::TriggerPause(unsigned int p_paused)
{
	unsigned int paused = p_paused;
	if (paused != 0) {
		int gameStatus = m_ai->m_gameStatus;
		if (gameStatus >= GAME_STATUS_PAUSED && gameStatus <= GAME_STATUS_RUNNING) {
			m_ai->GameState(GAME_STATUS_PAUSED);
		}
	}
	else {
		SetPause(paused);
	}
}

// FUNCTION: LEMBALL 0x00437de0
void C2D::SetPause(unsigned int p_paused)
{
	m_pauser = m_ai->m_isSinglePlayer == 0;
	if (p_paused != 0) {
		if (m_ai->m_gameStatus < GAME_STATUS_PAUSED || m_ai->m_gameStatus > GAME_STATUS_RUNNING) {
			return;
		}
	}
	m_paused = p_paused;
	ClockEditMode(p_paused);
	m_ai->m_paused = m_paused;
	if (p_paused != 0) {
		m_ai->GameState(GAME_STATUS_PAUSED);
	}
	else {
		m_ai->GameState(GAME_STATUS_RUNNING);
		m_pauser = 0;
	}
	if (m_paused != 0) {
		NewPauseWindow(PAUSE_MSG_PAUSED);
	}
	else {
		NewPauseWindow(PAUSE_MSG_NONE);
	}
}

// FUNCTION: LEMBALL 0x004383c0
void C2D::OnDriverChange()
{
	if (m_display->GetSizeStatus() != 0) {
		CVSRect useRect = m_display->GetUseRect(DISPLAY_COORDINATE_AUTO_CENTER, DISPLAY_COORDINATE_AUTO_CENTER);
		int zoom;
		if (g_nCompactPrimaryContextLayout != 0 || g_nEditLevelMode != 0 || g_nZoomEnabled != 0) {
			zoom = 1;
		}
		else {
			zoom = 2;
		}
		m_zoom = (unsigned short) zoom;
		unsigned int zoomDivisor = m_zoom;
		m_viewSize.m_x = (short) ((int) useRect.m_width / (int) zoomDivisor);
		m_viewSize.m_y = (short) ((int) useRect.m_height / (int) zoomDivisor);
		SetClipSize();

		CVSRect innerRect;
		short clipSizeX = m_clipSize.m_x;
		if (m_viewSize.m_x != clipSizeX || m_clipSize.m_y != m_viewSize.m_y) {
			CVSRect clipRect((short) m_clipOffsetX, (short) m_clipOffsetY, clipSizeX, m_clipSize.m_y);
			const CVSRect& source = clipRect;
			memcpy(&innerRect.m_width, &clipRect.m_width, sizeof(short));
			memcpy(&innerRect.m_height, &clipRect.m_height, sizeof(short));
			innerRect.m_x = source.m_x;
			innerRect.m_y = source.m_y;
		}
		m_display->SetRectInnerZoom(useRect, innerRect, m_zoom);
		if (m_pauseWindow != NULL) {
			m_pauseWindow->OnDriverChange();
		}
	}
}

// FUNCTION: LEMBALL 0x00439450
bool C2D::QuitYet()
{
	return m_quitRequested;
}

// FUNCTION: LEMBALL 0x00439460
int C2D::GetReturnState()
{
	return m_returnState;
}

// FUNCTION: LEMBALL 0x00439470
bool C2D::GetPauser()
{
	return m_pauser;
}

// FUNCTION: LEMBALL 0x0043ed20
void C2D::SetOrigin()
{
	int projectedX;
	int projectedY;
	AICOORD origin;
	unsigned int player;
	int changed = 0;

	if (m_groupingActive == GROUPING_SELECTING && m_groupSelectionCount != 0) {
		m_ai->GetPlayerPos(m_groupObjectIds[m_groupSelectionCount - 1], origin);
	}
	else if (m_ai->GetOrigin(origin, player) == 0) {
		return;
	}

	int gameX = origin.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int gameY = origin.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	unsigned short z;
	if (gameX >= 0 && gameY >= 0 && gameX >> GROUND_BLOCK_PIXEL_SHIFT < m_map->m_ground.m_width &&
		gameY >> GROUND_BLOCK_PIXEL_SHIFT < m_map->m_ground.m_height) {
		z = m_map->m_ground
				.m_ground[(gameY >> GROUND_BLOCK_PIXEL_SHIFT) * m_map->m_ground.m_width +
						  (gameX >> GROUND_BLOCK_PIXEL_SHIFT)]
				.GetZ(gameX & GROUND_BLOCK_PIXEL_MASK, gameY & GROUND_BLOCK_PIXEL_MASK);
	}
	else {
		z = 0;
	}
	origin.m_zFixed = (int) z << FIXED_POINT_FRACTION_BITS;

	if (m_ai->m_gameStatus == GAME_STATUS_NOT_STARTED || m_ai->m_gameStatus == GAME_STATUS_RUNNING) {
		int marginX = m_clipSize.m_x * 2 / 5;
		int marginY = m_clipSize.m_y * 2 / 5;
		m_originPosition = origin;

		{
			int screenX = origin.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			int screenY = origin.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			int screenZ = origin.m_zFixed >> FIXED_POINT_FRACTION_BITS;
			m_map->GameToScreen(screenX, screenY);
			origin.m_xFixed = screenX << FIXED_POINT_FRACTION_BITS;
			origin.m_yFixed = (screenY - screenZ) * FIXED_POINT_ONE;
		}
		int oldViewOriginX = m_viewOriginX;
		projectedX = origin.m_xFixed >> FIXED_POINT_FRACTION_BITS;
		projectedY = origin.m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int oldViewOriginY = m_viewOriginY;
		int differenceX = projectedX - m_viewOriginX;
		int differenceY = projectedY - m_viewOriginY;
		if (differenceX < marginX) {
			changed = 1;
			m_viewOriginX = projectedX - marginX;
		}
		if (differenceY < marginY) {
			changed = 1;
			m_viewOriginY = projectedY - marginY;
		}
		if (m_clipSize.m_x - marginX < differenceX) {
			changed = 1;
			m_viewOriginX = projectedX - m_clipSize.m_x + marginX;
		}
		if (m_clipSize.m_y - marginY < differenceY) {
			changed = 1;
			m_viewOriginY = projectedY - m_clipSize.m_y + marginY;
		}
		if (changed != 0) {
			SendCursorMsg();
			m_scrollPending = 1;
			m_scrollDeltaX = (short) oldViewOriginX - (short) m_viewOriginX;
			m_scrollDeltaY = (short) oldViewOriginY - (short) m_viewOriginY;
		}
	}
}
