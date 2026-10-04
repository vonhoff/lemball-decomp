#include "C2D.h"

#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Animation/AnimSpecialEntry.h"
#include "Gameplay/Animation/CAnimSpecial.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Hazards/CSlinky.h"
#include "Gameplay/Mechanisms/CSwitch.h"
#include "Gameplay/Objects/CViewData.h"
#include "Game/CDemo.h"
#include "Game/CGame.h"
#include "Game/GameMain.h"
#include "Game/GameTime.h"
#include "Level/CLevelLoader.h"
#include "../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../Frontend/Resources/CFrontendResourceLoader.h"
#include "Map/CMap.h"
#include "Network/CNetworkManager.h"
#include "Visos/Queues/CBaseQueue.h"
#include "CObjSq.h"
#include "Visos/Text/CTextManager.h"
#include "Gameplay/Geometry/CVSMath.h"
#include "Visos/Sorting/VsSort.h"
#include "Visos/Time/VsTime.h"
#include "Visos/Graphics/Palettes/CBasePalManager.h"
#include "Platform/Windows/Graphics/CCursor.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Controls/CHotAreaList.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Graphics/Primitives/CZRLE.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/NetworkConstants.h"
#include "../../Visos/Network/NetworkMode.h"
#include "Visos/Resources/Types/CResFONT.h"
#include "Visos/Resources/Types/CResPALETTE.h"
#include "../../Visos/Resources/ResourceLimits.h"
#include "../Animation/CLemmingAnimsManager.h"
#include "../Input/CPadToButton.h"
#include "../Panel/CPanel.h"
#include "../Pause/CPauseWindow.h"
#include "../Sound/CSoundView.h"
#include "ObjectClipGrid.h"
#include "SpriteGroundLookup.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "CMain2DDisplay.h"
#include "CPBButton.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Visos/Math/FixedPoint.h"

#include <new.h>
#include <string.h>

enum {
	VIEW_DATA_INDEX_NOT_FOUND = -1,
	C2D_BOTTOM_EDGE_OUTSIDE_VIEWPORT = -1,
	DOOR_LOCK_NO_PLAYER_REMAP = -1
};

enum eMoverVisualState {
	MOVER_VISUAL_GROUND = 0,
	MOVER_VISUAL_STAR = 1
};

enum eAnimationTiming {
	ANIMATION_FRAME_RATE_FPS = 15,
	PAINT_GUN_ANIMATION_FRAME_RATE_FPS = 8,
	LEMMING_CONVEYOR_ANIMATION_FRAME_COUNT = 8,
	SPECIAL_ANIMATION_FRAME_INTERVAL_MS = 70,
	SPECIAL_ANIMATION_FRAME_COUNT = 8,
	SPECIAL_ANIMATION_FRAME_MASK = SPECIAL_ANIMATION_FRAME_COUNT - 1,
	GROUND_ANIMATION_VARIANT_COUNT = 4,
	GROUND_ANIMATION_VARIANT_MASK = GROUND_ANIMATION_VARIANT_COUNT - 1,
	ANIMATION_PHASE_TIME_PERIOD_MS = 2048,
	ANIMATION_PHASE_TIME_MASK = ANIMATION_PHASE_TIME_PERIOD_MS - 1,
	ANIMATION_PHASE_TIME_SHIFT = 7,
	ROCKET_ANIMATION_ALTERNATE_FRAME_COUNT = 2,
	ROCKET_ANIMATION_ALTERNATE_FRAME_MASK = ROCKET_ANIMATION_ALTERNATE_FRAME_COUNT - 1,
	DEMO_TEXT_BLINK_INTERVAL_MS = 500
};

enum eLevelTimeDisplay {
	LEVEL_TIME_DISPLAY_LIMIT_SECONDS = 10 * 60,
	LEVEL_TIME_DISPLAY_MAX_SECONDS = LEVEL_TIME_DISPLAY_LIMIT_SECONDS - 1,
	SECONDS_PER_DISPLAY_MINUTE = 60,
	SECONDS_PER_DISPLAY_TEN = 10
};

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

enum eGroupingMode {
	GROUPING_INACTIVE = 0,
	GROUPING_SELECTING = 1
};

enum eC2DCursorState {
	C2D_CURSOR_STATE_GROUND = 0,
	C2D_CURSOR_STATE_NO_GROUND = 1,
	C2D_CURSOR_STATE_OBJECT = 2,
	C2D_CURSOR_STATE_PANEL = 3
};

enum eSpriteSortCode {
	SPRITE_SORT_CODE_SPECIAL_OBJECT = 0x7d00,
	SPRITE_SORT_CODE_TOPMOST = 0x7fff
};

extern int g_anC2DRemapSourceIndices[17];
extern int g_anC2DHitBounds[11][4];
extern int g_anC2DRemapTargetIndices[4][17];
extern unsigned char g_abC2DType2Remap[5];
extern "C" unsigned long __stdcall timeGetTime(void);
extern char* g_demoText;

enum {
	C2D_VIEW_DATA_CAPACITY = 200
};

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

#include "Platform/Windows/Graphics/CCursor.h"

// FUNCTION: LEMBALL 0x004364b0
void C2D::CursorChangeType(int p_cursorType, int p_value)
{
	::CursorChangeType((eCursorDisplayType) p_cursorType, p_value);
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

// FUNCTION: LEMBALL 0x00436690
void C2D::DoButtons()
{
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

#include "Game/CGameStatus.h"

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

// FUNCTION: LEMBALL 0x004369b0
void C2D::CheckValidFormGroup()
{
	int i;

	if (m_groupingActive == GROUPING_SELECTING) {
		for (i = 0; i < m_groupCount; i++) {
			if (!g_pObjects[m_groupObjectIds[i]]->IsSelectable()) {
				RemoveFromGroupByObjectNo(m_groupObjectIds[i]);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00436e40
bool C2D::FindGameObject(const CVSPoint& p_point, int& p_index, int p_preferLemming)
{
	int pointX = p_point.m_x - m_viewOriginX;
	int pointY = p_point.m_y - m_viewOriginY;
	int index;
	int selected = VIEW_DATA_INDEX_NOT_FOUND;
	int lemming = VIEW_DATA_INDEX_NOT_FOUND;
	index = 0;
	if (m_viewDataCount != 0) {
		do {
			int type = m_viewData[index].m_objectType;
			int x = m_viewData[index].m_positionX;
			int y = m_viewData[index].m_positionY;
			int left, top, right, bottom;
			switch (type) {
			case OBJECT_PLAYER_2:
				left = x + g_anC2DHitBounds[0][0];
				top = y + g_anC2DHitBounds[0][1];
				right = x + g_anC2DHitBounds[0][2];
				bottom = y + g_anC2DHitBounds[0][3];
				break;
			case OBJECT_CATAPULT:
				left = x + g_anC2DHitBounds[2][0];
				top = y + g_anC2DHitBounds[2][1];
				right = x + g_anC2DHitBounds[2][2];
				bottom = y + g_anC2DHitBounds[2][3];
				break;
			case OBJECT_AMMO:
				left = x + g_anC2DHitBounds[3][0];
				top = y + g_anC2DHitBounds[3][1];
				right = x + g_anC2DHitBounds[3][2];
				bottom = y + g_anC2DHitBounds[3][3];
				break;
			case OBJECT_FLAG_2:
				left = x + g_anC2DHitBounds[10][0];
				top = y + g_anC2DHitBounds[10][1];
				right = x + g_anC2DHitBounds[10][2];
				bottom = y + g_anC2DHitBounds[10][3];
				break;
			case OBJECT_CRATE:
				left = x + g_anC2DHitBounds[1][0];
				top = y + g_anC2DHitBounds[1][1];
				right = x + g_anC2DHitBounds[1][2];
				bottom = y + g_anC2DHitBounds[1][3];
				break;
			case OBJECT_SWITCH:
				left = x + g_anC2DHitBounds[4][0];
				top = y + g_anC2DHitBounds[4][1];
				right = x + g_anC2DHitBounds[4][2];
				bottom = y + g_anC2DHitBounds[4][3];
				break;
			case OBJECT_KEY_1:
			case OBJECT_KEY_2:
			case OBJECT_KEY_3:
				left = x + g_anC2DHitBounds[5][0];
				top = y + g_anC2DHitBounds[5][1];
				right = x + g_anC2DHitBounds[5][2];
				bottom = y + g_anC2DHitBounds[5][3];
				break;
			case OBJECT_DUPLICATOR:
				left = x + g_anC2DHitBounds[6][0];
				top = y + g_anC2DHitBounds[6][1];
				right = x + g_anC2DHitBounds[6][2];
				bottom = y + g_anC2DHitBounds[6][3];
				break;
			case OBJECT_TRAMPOLINE:
				left = x + g_anC2DHitBounds[9][0];
				top = y + g_anC2DHitBounds[9][1];
				right = x + g_anC2DHitBounds[9][2];
				bottom = y + g_anC2DHitBounds[9][3];
				break;
			case OBJECT_BALLOON_0:
			case OBJECT_BALLOON_2:
			case OBJECT_BALLOON_4:
			case OBJECT_BALLOON_6:
				left = x + g_anC2DHitBounds[7][0];
				top = y + g_anC2DHitBounds[7][1];
				right = x + g_anC2DHitBounds[7][2];
				bottom = y + g_anC2DHitBounds[7][3];
				break;
			case OBJECT_MOVER:
				left = x + g_anC2DHitBounds[8][0];
				top = y + g_anC2DHitBounds[8][1];
				right = x + g_anC2DHitBounds[8][2];
				bottom = y + g_anC2DHitBounds[8][3];
				break;
			default:
				goto next;
			}
			if (left <= pointX && pointX < right && top <= pointY && pointY < bottom) {
				if (p_preferLemming != 0) {
					if (type == OBJECT_PLAYER_2 && bottom > C2D_BOTTOM_EDGE_OUTSIDE_VIEWPORT) {
						selected = index;
					}
				}
				else if (type == OBJECT_PLAYER_2) {
					if (bottom > C2D_BOTTOM_EDGE_OUTSIDE_VIEWPORT) {
						lemming = index;
					}
				}
				else if (bottom > C2D_BOTTOM_EDGE_OUTSIDE_VIEWPORT) {
					selected = index;
				}
			}
		next:
			index++;
		} while (m_viewDataCount > index);
	}
	if (selected != VIEW_DATA_INDEX_NOT_FOUND) {
		p_index = selected;
		return true;
	}
	if (lemming != VIEW_DATA_INDEX_NOT_FOUND) {
		p_index = lemming;
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00437130
void C2D::AddObjectToGroup(int p_objectNo, int p_markSelection)
{
	m_groupObjectIds[m_groupCount] = (unsigned short) p_objectNo;
	m_groupCount++;
	if (p_markSelection != 0) {
		m_groupSelectionCount = m_groupCount;
	}
}

// FUNCTION: LEMBALL 0x00437170
void C2D::FormGroup()
{
	Message message;
	message.m_type = AI_MESSAGE_FORM_GROUP;
	message.m_code = m_groupCount;
	message.m_time = 0;
	message.m_payload = m_groupObjectIds;
	message.m_source = NULL;

	CheckValidFormGroup();
	if (m_groupCount > 0) {
		m_lemmingManager->Post(message);
		m_groupCount = 0;
		m_groupingActive = GROUPING_INACTIVE;
		g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
	}
}

// FUNCTION: LEMBALL 0x004371e0
void C2D::MoveGroup(const CVSPoint& p_point)
{
	Message msg;
	msg.m_type = AI_MESSAGE_MOVE_GROUP;
	memset(&msg.m_time, 0, sizeof(msg.m_time) + sizeof(msg.m_code) + sizeof(msg.m_payload) + sizeof(msg.m_source));
	msg.m_code = p_point.m_x;
	msg.m_payload = (void*) (int) p_point.m_y;
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	g_pSoundView->m_pendingEffect = SFX_DRUM1;
}

// FUNCTION: LEMBALL 0x00437250
void C2D::CancelMoves()
{
	Message msg;
	msg.m_type = AI_MESSAGE_CANCEL_MOVES;
	memset(&msg.m_time, 0, sizeof(msg.m_time) + sizeof(msg.m_code) + sizeof(msg.m_payload) + sizeof(msg.m_source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	g_pSoundView->m_pendingEffect = SFX_DRUM1;
}

// FUNCTION: LEMBALL 0x004372a0
void C2D::NextGroup()
{
	Message msg;
	msg.m_type = AI_MESSAGE_NEXT_GROUP;
	memset(&msg.m_time, 0, sizeof(msg.m_time) + sizeof(msg.m_code) + sizeof(msg.m_payload) + sizeof(msg.m_source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	g_pSoundView->m_pendingEffect = SFX_CHANGEOP;
}

// FUNCTION: LEMBALL 0x004372f0
void C2D::PrevGroup()
{
	Message msg;
	msg.m_type = AI_MESSAGE_PREVIOUS_GROUP;
	memset(&msg.m_time, 0, sizeof(msg.m_time) + sizeof(msg.m_code) + sizeof(msg.m_payload) + sizeof(msg.m_source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	g_pSoundView->m_pendingEffect = SFX_CHANGEOP;
}

// FUNCTION: LEMBALL 0x00437340
void C2D::SelectLemming(int p_playerIndex)
{
	Message msg;
	msg.m_type = AI_MESSAGE_USE_OBJECT;
	msg.m_time = 0;
	msg.m_code = m_ai->m_networkLemmings[p_playerIndex]->m_objectId;
	memset(&msg.m_payload, 0, sizeof(msg.m_payload) + sizeof(msg.m_source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
}

// FUNCTION: LEMBALL 0x004373b0
void C2D::SelectObject(int p_viewIndex)
{
	Message msg;
	msg.m_type = AI_MESSAGE_USE_OBJECT;
	msg.m_time = 0;
	msg.m_code = m_viewData[p_viewIndex].m_objectId;
	memset(&msg.m_payload, 0, sizeof(msg.m_payload) + sizeof(msg.m_source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = GROUPING_INACTIVE;
	g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
}

// FUNCTION: LEMBALL 0x00437420
bool C2D::InGroupByObjectNo(int p_objectNo)
{
	int i;
	unsigned short* ids = m_groupObjectIds;
	for (i = 0; i < m_groupCount; i++, ids++) {
		if (*ids == p_objectNo) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x00437460
void C2D::RemoveFromGroupByObjectNo(int p_objectNo)
{
	unsigned int i = 0;
	int objectNo = p_objectNo;
	unsigned short id;
	unsigned short* write;
	unsigned short* read;

	if (m_groupCount <= i) {
	}
	else {
		write = m_groupObjectIds;
		read = write;
		do {
			id = *read;
			if ((unsigned int) id != (unsigned int) objectNo) {
				*write = id;
				write = write + 1;
			}
			read = read + 1;
			i = i + 1;
		} while ((int) m_groupCount > (int) i);
	}
	m_groupCount = m_groupCount - 1;
	if (m_groupCount < m_groupSelectionCount) {
		m_groupSelectionCount = m_groupSelectionCount - 1;
	}
	if (m_groupCount == 0) {
		m_groupingActive = GROUPING_INACTIVE;
	}
}

// FUNCTION: LEMBALL 0x004374e0
bool C2D::IsInGrouping(CGameObject* p_object)
{
	int i;
	for (i = 0; i < m_groupCount; i++) {
		if (p_object->m_objectId == m_groupObjectIds[i]) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x004376b0
void C2D::GroupingLeftClick(const CVSPoint& p_screenPoint, const CVSPoint& p_gamePoint, unsigned int p_alternate)
{
	int index;
	if (FindGameObject(p_screenPoint, index, 0)) {
		switch (m_viewData[index].m_objectType) {
		case OBJECT_PLAYER_2:
			if (p_alternate == 0) {
				if (InGroupByObjectNo(m_viewData[index].m_objectId)) {
					RemoveFromGroupByObjectNo(m_viewData[index].m_objectId);
				}
				else {
					AddObjectToGroup(m_viewData[index].m_objectId, 0);
				}
				g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
				return;
			}
			break;
		case OBJECT_CATAPULT:
		case OBJECT_AMMO:
		case OBJECT_FLAG_2:
		case OBJECT_CRATE:
		case OBJECT_SWITCH:
		case OBJECT_KEY_1:
		case OBJECT_KEY_2:
		case OBJECT_KEY_3:
		case OBJECT_DUPLICATOR:
		case OBJECT_TRAMPOLINE:
		case OBJECT_BALLOON_0:
		case OBJECT_BALLOON_2:
		case OBJECT_BALLOON_4:
		case OBJECT_BALLOON_6:
			if (m_groupCount > 0) {
				FormGroup();
			}
			SelectObject(index);
			return;
		case OBJECT_MOVER:
			break;
		default:
			return;
		}
	}
	if (m_groupCount > 0) {
		FormGroup();
		MoveGroup(p_gamePoint);
		return;
	}
	MoveGroup(p_gamePoint);
}

// FUNCTION: LEMBALL 0x00437840
void C2D::LeftClick(const CVSPoint& p_screenPoint,
					const CVSPoint& p_gamePoint,
					unsigned int p_cancelMoves,
					unsigned int p_alternate)
{
	switch (m_groupingActive) {
	case GROUPING_INACTIVE:
		NoStateLeftClick(p_screenPoint, p_gamePoint, p_cancelMoves, p_alternate);
		break;
	case GROUPING_SELECTING:
		GroupingLeftClick(p_screenPoint, p_gamePoint, p_alternate);
		break;
	}
}

// FUNCTION: LEMBALL 0x00437890
void C2D::NoStateRightClick(const CVSPoint& p_screenPoint, const CVSPoint& p_gamePoint)
{
	CViewData* views;
	int index;
	Message message;
	message.m_type = AI_MESSAGE_REQUEST_FIRE;
	memset(&message.m_time,
		   0,
		   sizeof(message.m_time) + sizeof(message.m_code) + sizeof(message.m_payload) + sizeof(message.m_source));
	if (FindGameObject(p_screenPoint, index, 1)) {
		views = m_viewData;
		if ((index + views)->m_objectType == OBJECT_PLAYER_2) {
			SelectObject(index);
			return;
		}
	}
	message.m_code = p_gamePoint.m_x;
	message.m_payload = (void*) (int) p_gamePoint.m_y;
	m_lemmingManager->Post(message);
	m_groupingActive = GROUPING_INACTIVE;
}

// FUNCTION: LEMBALL 0x00437930
void C2D::RightClick(const CVSPoint& p_screenPoint, const CVSPoint& p_gamePoint)
{
	if (m_groupingActive != GROUPING_INACTIVE) {
		if (m_groupingActive != GROUPING_SELECTING) {
			return;
		}
		FormGroup();
		if (m_groupCount == 0) {
			m_groupingActive = GROUPING_INACTIVE;
		}
	}
	NoStateRightClick(p_screenPoint, p_gamePoint);
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

// GLOBAL: LEMBALL 0x00496ec8
int g_anC2DHitBounds[11][4] = {{-8, -16, 8, 8},
							   {-8, -12, 8, 4},
							   {-28, -48, 20, 8},
							   {-8, -16, 8, 2},
							   {-10, -17, 8, 2},
							   {-8, -32, 8, 32},
							   {-10, -37, 36, 6},
							   {-10, -48, 10, 0},
							   {-16, -16, 15, 8},
							   {-16, -16, 16, -8},
							   {-13, -27, 15, 2}};

// GLOBAL: LEMBALL 0x0049e8b8
int g_anC2DRemapSourceIndices[17] = {250, 204, 205, 206, 118, 107, 101, 95, 85, 75, 69, 59, 49, 46, 44, 37, 48};

// GLOBAL: LEMBALL 0x0049e8fc
int g_anC2DRemapTargetIndices[4][17] = {
	{224, 225, 226, 227, 228, 229, 230, 231, 232, 232, 233, 234, 234, 234, 234, 235, 235},
	{192, 193, 194, 195, 196, 197, 198, 199, 200, 200, 201, 202, 202, 202, 202, 203, 203},
	{208, 209, 210, 211, 212, 213, 214, 215, 216, 216, 217, 218, 218, 218, 218, 219, 219},
	{179, 180, 181, 182, 183, 184, 185, 186, 187, 187, 188, 189, 189, 189, 189, 190, 191}};

// GLOBAL: LEMBALL 0x0049ea14
int g_nMouseShapeGameX = 0;

// GLOBAL: LEMBALL 0x0049ea18
int g_nMouseShapeGameY = 0;

// GLOBAL: LEMBALL 0x0049ea1c
unsigned int g_nMouseShapeOnGround = 0;

// GLOBAL: LEMBALL 0x0049ea28
unsigned char g_abC2DType2Remap[5] = {2, 241, 81, 168, 108};

// GLOBAL: LEMBALL 0x0049efcc
int g_lastDrawnTime = 0;

// GLOBAL: LEMBALL 0x004a78bc
char g_timeText[5];

// FUNCTION: LEMBALL 0x00437e90
void C2D::SetMouseShape()
{
	int objectIndex;
	int zoom;

	if (m_paused != 0) {
		return;
	}
	const CVSPoint* origin = &m_display->m_rect;
	zoom = (int) m_display->m_zoom;
	short screenX = (short) ((int) (short) (g_pCursor->m_position.m_x - origin->m_x) / zoom);
	short screenY = (short) ((int) (short) (g_pCursor->m_position.m_y - origin->m_y) / zoom);
	if (m_panel->MouseInPanel(CVSPoint(screenX, screenY)) != 0) {
		m_cursorState = C2D_CURSOR_STATE_PANEL;
		return;
	}
	CVSPoint game((short) (m_viewOriginX + m_cursorGamePoint.m_x),
				  (short) (m_cursorGamePoint.m_y + (short) m_viewOriginY));
	if (screenX < m_bounds.m_x || (short) (m_bounds.m_width + m_bounds.m_x) <= screenX || screenY < m_bounds.m_y ||
		(short) (m_bounds.m_height + m_bounds.m_y) <= screenY) {
		CursorChangeType(CURSOR_DISPLAY_HAND, CURSOR_HAND_FRAME_DEFAULT);
	}
	else if (FindGameObject(game, objectIndex, 0) != 0) {
		if (m_cursorState != C2D_CURSOR_STATE_OBJECT) {
			m_cursorTimestamp = g_dwSimulationTimestamp;
			m_cursorState = C2D_CURSOR_STATE_OBJECT;
			CursorChangeType(CURSOR_DISPLAY_HAND, CURSOR_HAND_FRAME_HOVER);
			return;
		}
	}
	else {
		g_nMouseShapeOnGround = ScreenToGame((int) game.m_x, (int) game.m_y, g_nMouseShapeGameX, g_nMouseShapeGameY);
		if (g_nMouseShapeOnGround != 0) {
			m_cursorState = C2D_CURSOR_STATE_GROUND;
			m_cursorTimestamp = g_dwSimulationTimestamp;
			if (m_mouseButtonDown != 0) {
				CursorChangeType(CURSOR_DISPLAY_HAND, CURSOR_HAND_FRAME_PRESSED);
				return;
			}
			CursorChangeType(CURSOR_DISPLAY_HAND, CURSOR_HAND_FRAME_DEFAULT);
			return;
		}
		if (m_cursorState != C2D_CURSOR_STATE_NO_GROUND) {
			m_cursorTimestamp = g_dwSimulationTimestamp;
			m_cursorState = C2D_CURSOR_STATE_NO_GROUND;
			CursorChangeType(CURSOR_DISPLAY_HAND, CURSOR_HAND_FRAME_NO_GROUND);
			return;
		}
		if (m_cursorTimestamp + 100 < g_dwSimulationTimestamp) {
			m_cursorBlinkPhase = (unsigned short) (m_cursorBlinkPhase ^ 1);
			m_cursorTimestamp = g_dwSimulationTimestamp;
			CursorChangeType(CURSOR_DISPLAY_HAND, m_cursorBlinkPhase + CURSOR_HAND_FRAME_NO_GROUND);
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x004380c0
void C2D::SendCursorMsg()
{
	Message message;
	int screenX;
	int screenY;
	message.m_type = AI_MESSAGE_CURSOR_POSITION;
	memset(&message.m_time,
		   0,
		   sizeof(message.m_time) + sizeof(message.m_code) + sizeof(message.m_payload) + sizeof(message.m_source));
	{
		CVSPoint point =
			CVSPoint((short) m_viewOriginX + m_cursorGamePoint.m_x, m_cursorGamePoint.m_y + (short) m_viewOriginY);
		CVSPoint* screenPoint = &point;
		screenY = screenPoint->m_y;
		screenX = screenPoint->m_x;
	}
	{
		int gameX;
		int gameY;
		if (!ScreenToGame(screenX, screenY, gameX, gameY)) {
			m_map->ScreenToGame(screenX, screenY, gameX, gameY);
		}
		message.m_code = gameX;
		message.m_payload = (void*) gameY;
		m_lemmingManager->Post(message);
	}
}

// FUNCTION: LEMBALL 0x00438170
void C2D::OnInside(const CVSPoint& p_point)
{
	if ((g_pDemo == NULL || g_pDemo->m_demoMode == 0) && !m_display->IsFocusWindow()) {
		return;
	}
	m_cursorGamePoint.m_x = p_point.m_x;
	m_cursorGamePoint.m_y = p_point.m_y;
	SendCursorMsg();
}

// FUNCTION: LEMBALL 0x004381c0
void C2D::OnButtonUp(const CVSPoint& p_point, int p_flags)
{
	m_mouseButtonDown = 0;
	if (m_paused == 0) {
		if ((g_pDemo == NULL || g_pDemo->m_demoMode == 0) && !m_display->IsFocusWindow()) {
			return;
		}
		SendCursorMsg();
	}
}

// FUNCTION: LEMBALL 0x00438210
void C2D::OnButtonDown(const CVSPoint& p_point, int p_flags)
{
	m_mouseButtonDown = 1;
	if (m_paused == 0) {
		if ((g_pDemo == NULL || g_pDemo->m_demoMode == 0) && !m_display->IsFocusWindow()) {
			return;
		}
		CVSPoint screenPoint((short) m_viewOriginX + p_point.m_x, p_point.m_y + (short) m_viewOriginY);
		int screenX = screenPoint.m_x;
		int screenY = screenPoint.m_y;
		g_nMouseShapeOnGround = ScreenToGame(screenX, screenY, g_nMouseShapeGameX, g_nMouseShapeGameY);
		if (g_nMouseShapeOnGround == 0) {
			m_map->ScreenToGame(screenX, screenY, g_nMouseShapeGameX, g_nMouseShapeGameY);
		}
		CVSPoint gamePoint((short) g_nMouseShapeGameX, (short) g_nMouseShapeGameY);
		switch (p_flags) {
		case MOUSE_BUTTON_INDEX_LEFT:
		case MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK:
			LeftClick(screenPoint, gamePoint, 1, 0);
			break;
		case MOUSE_BUTTON_INDEX_RIGHT:
		case MOUSE_BUTTON_INDEX_RIGHT_DOUBLE_CLICK:
			RightClick(screenPoint, gamePoint);
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00438330
void C2D::UseBalloon(int p_playerIndex)
{
	CPlayerLemming** pLemming = &m_ai->m_networkLemmings[p_playerIndex];
	if ((*pLemming)->GetLastBalloon() != OBJECT_INVALID && (*pLemming)->m_action != ACTION_DEAD) {
		(*pLemming)->SetSndEffect(SFX_BALLOON);
		UseBalloon(*pLemming);
	}
}

// FUNCTION: LEMBALL 0x00438380
void C2D::UseBalloon(CPlayerLemming* p_lemming)
{
	if (p_lemming->m_action != ACTION_DEAD) {
		m_groupCount = 0;
		AddObjectToGroup(p_lemming->m_objectId, 0);
		FormGroup();
		p_lemming->RequestBalloon();
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

// FUNCTION: LEMBALL 0x00438500
void C2D::SetClipSize()
{
	int width;
	int height;
	int count;
	SpriteGroundLookup* lookup;
	CResFONT* font;
	short clipSizeX;
	short translatedX;

	if (g_nZoomEnabled != 0 && g_nCompactPrimaryContextLayout == 0) {
		m_clipSize.m_x = 0x140;
		m_clipSize.m_y = 0xf0;
		m_clipOffsetX = 0xa0;
		m_clipOffsetY = 0x78;
	}
	else {
		m_clipSize.m_x = m_viewSize.m_x;
		m_clipSize.m_y = m_viewSize.m_y;
		m_clipOffsetX = 0;
		m_clipOffsetY = 0;
	}
	if (g_pDemo != NULL) {
		short demoOffsetY = (short) m_clipOffsetY;
		CDemo* demo = g_pDemo;
		demo->m_offsetX = (short) m_clipOffsetX;
		demo->m_offsetY = demoOffsetY;
	}
	lookup = m_spriteGroundLookup;
	if (lookup != NULL) {
		width = (m_clipSize.m_x + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE;
		height = (m_clipSize.m_y + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE;
		if (lookup->m_width != width || lookup->m_height != height) {
			if (lookup->m_maskA != NULL) {
				operator delete(lookup->m_maskA);
				lookup->m_maskA = NULL;
			}
			if (lookup->m_maskB != NULL) {
				operator delete(lookup->m_maskB);
				lookup->m_maskB = NULL;
			}
			lookup->m_width = (short) width;
			lookup->m_height = (short) height;
			lookup->m_maskA =
				(unsigned char*) operator new((unsigned int) lookup->m_width*(unsigned int) lookup->m_height);
			lookup->m_maskB =
				(unsigned char*) operator new((unsigned int) lookup->m_width*(unsigned int) lookup->m_height);
		}
		count = (int) lookup->m_width * (int) lookup->m_height;
		memset(lookup->m_maskA, 1, count);
		count = (int) lookup->m_width * (int) lookup->m_height;
		memset(lookup->m_maskB, 1, count);
	}
	clipSizeX = m_clipSize.m_x;
	m_spriteGroundLookupRectA.m_width = 0x33;
	translatedX = clipSizeX - 0x43;
	m_clipConfigured = 1;
	m_spriteGroundLookupRectA.m_height = 0x20;
	m_spriteGroundLookupRectB.m_width = 0x60;
	m_spriteGroundLookupRectB.m_x = 0x10;
	m_spriteGroundLookupRectA.m_x = translatedX;
	m_spriteGroundLookupRectA.m_y = 8;
	m_spriteGroundLookupRectB.m_height = 0x20;
	m_spriteGroundLookupRectB.m_y = 8;
	g_nLevelViewportHorizontalRemainder = m_viewSize.m_x - clipSizeX;
	g_nLevelViewportVerticalRemainder = m_viewSize.m_y - m_clipSize.m_y;
	m_redrawPending = 1;
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		font = m_textManager->GetFont(0xf8);
		short remainingWidth = m_clipSize.m_x;
		remainingWidth -= font->GetSize(g_demoText, TEXT_ADVANCE_X_POSITIVE).m_width;
		m_demoTextRect.m_y = 0;
		m_demoTextRect.m_x = remainingWidth / 2;
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

// GLOBAL: LEMBALL 0x0049705c
static const short g_treeGroundOffset[] = {0x20, 0x30};
// GLOBAL: LEMBALL 0x00497060
static const short g_groundOffset[] = {0x10, 0x10};

// FUNCTION: LEMBALL 0x0043a880
void C2D::DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame)
{
	switch (p_groundType) {
	case TERRAIN_TREE:
		m_lemmingAnims->DrawAnim(p_x - g_treeGroundOffset[0],
								 p_y - g_treeGroundOffset[1],
								 g_anGroundStyleResourceIds[3],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_BLOX_1:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox1ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_2:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox2ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox3ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_4:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox4ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_5:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox5ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_6:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox6ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_7:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox7ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 g_anGroundStyleResourceIds[0],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 g_anGroundStyleResourceIds[4],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 g_anGroundStyleResourceIds[5],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_ANIM:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_ANIM, p_frame, 0, NULL);
		return;
	case TERRAIN_FLAME:
		m_lemmingAnims->DrawAnim(
			p_x - 0x10,
			p_y - 0x20,
			RES_GAME_FLAME,
			(((unsigned short) p_x >> GROUND_BLOCK_PIXEL_SHIFT) + (unsigned short) m_groundAnimationFrame) % 9,
			0,
			NULL);
		return;
	case TERRAIN_ELECTRIC:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 RES_GAME_ELECTRIC,
								 (unsigned short) m_groundAnimationFrame & GROUND_ANIMATION_VARIANT_MASK,
								 0,
								 NULL);
		return;
	case TERRAIN_EMBERS:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_EMBERS, p_frame, 0, NULL);
		return;
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_CONVEYOR, p_frame, 0, NULL);
		return;
	default:
		return;
	}
}

// FUNCTION: LEMBALL 0x0043ace0
void C2D::DrawCliff(int p_x, int p_y, int p_height, int p_count)
{
	int x = p_x - g_groundOffset[0];
	int y = p_y + (p_count * 0x10 - p_height);

	if (p_count > 0) {
		do {
			m_lemmingAnims->DrawAnim(x, y - g_groundOffset[1], g_groundBlox1ResourceId, 0, 0, NULL);
			y -= 0x10;
		} while (--p_count != 0);
	}
}

// FUNCTION: LEMBALL 0x0043ad40
void C2D::DoClipWidth(int p_mapX, int p_mapY, int p_count)
{
	int mapX = p_mapX;
	int screenY;
	int screenX;
	eObjectType defaultGroundType;
	int defaultGroundData;
	int processed;
	short baseZ;
	int delayed;
	int drawGround;
	CGround* ground;
	int groundStep;
	unsigned short groundData;
	short height;
	unsigned short cliff;
	eObjectType groundType;
	int heightValue;
	int groundY;
	int zOffset;

	screenY = m_clipScreenY;
	screenX = m_clipScreenX;
	defaultGroundType = m_map->m_defaultBlox;
	defaultGroundData = m_map->m_defaultBloxData;
	processed = 0;
	baseZ = ((short) p_mapY + (short) mapX) * 0x40;
	if (baseZ < 0) {
		baseZ = 0;
	}
	m_lemmingAnims->m_primitiveSequence = baseZ;

	if (p_count > 0) {
		do {
			if (mapX >= 0 && p_mapY >= 0 && mapX < m_groundWidth && p_mapY < m_groundHeight) {
				break;
			}
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
			mapX += m_clipMapStepX;
			processed++;
			p_mapY += m_clipMapStepY;
		} while (processed < p_count);
	}

	if (processed < p_count) {
		delayed = 0;
		drawGround = 1;
		ground = m_map->m_ground.m_ground + m_map->m_ground.m_width * p_mapY + mapX;
		groundStep = 1 - m_groundWidth;

		for (; p_count > processed && mapX >= 0 && p_mapY >= 0 && mapX < m_groundWidth && p_mapY < m_groundHeight;) {
			if ((ground->m_collision & GROUND_COLLISION_SPECIAL_RENDER) == 0) {
				groundData = ground->m_objectData;
				height = ground->m_height;
				cliff = ground->m_cliff;
				groundType = ground->m_objectType;

				if (height < 0) {
					groundType = defaultGroundType;
					groundData = defaultGroundData;
					height = 0;
				}

				heightValue = height;
				groundY = screenY - heightValue;
				switch (groundType) {
				case TERRAIN_TREE:
					zOffset = 0x20;
					break;
				case TERRAIN_BLOX_1:
					zOffset = 0x10;
					break;
				case TERRAIN_BLOX_2:
					zOffset = 8;
					break;
				case TERRAIN_BLOX_5:
					zOffset = 8;
					break;
				case TERRAIN_BLOX_6:
				case TERRAIN_BLOX_7:
					delayed = 1;
					zOffset = 0;
					break;
				case TERRAIN_ANIM:
				case TERRAIN_FLAME:
				case TERRAIN_ELECTRIC:
				case TERRAIN_CONVEYOR_VARIANT_A:
				case TERRAIN_CONVEYOR_VARIANT_B:
					drawGround = 0;
				case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_4:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
					zOffset = 0;
					break;
				case TERRAIN_CLIFF_ONLY_GROUND:
				case TERRAIN_EMBERS:
					zOffset = 0;
					break;
				}

				m_lemmingAnims->m_primitiveSequence = (unsigned short) (zOffset + baseZ + height);
				if (delayed == 0) {
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					if (drawGround != 0) {
						DrawGround(screenX, groundY, groundType, groundData);
					}
					drawGround = 1;
				}
				if (delayed != 0) {
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					if (drawGround != 0) {
						DrawGround(screenX, groundY, groundType, groundData);
					}
					delayed = 0;
					drawGround = 1;
				}
			}

			mapX += m_clipMapStepX;
			p_mapY += m_clipMapStepY;
			screenX += 0x20;
			processed++;
			ground += groundStep;
		}
	}

	m_lemmingAnims->m_primitiveSequence = baseZ;
	if (processed < p_count) {
		processed = p_count - processed;
		do {
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
		} while (--processed != 0);
	}
}

// FUNCTION: LEMBALL 0x0043b0e0
void C2D::DoClipWidthSearch(int p_mapX, int p_mapY, int p_count)
{
	int screenX;
	int screenY;
	eObjectType defaultGroundType;
	int defaultGroundData;
	short baseZ;
	int processed;
	int delayed;
	unsigned short groundWidth;
	CGround* ground;
	int groundStep;
	unsigned short groundData;
	short height;
	unsigned short cliff;
	eObjectType groundType;
	int zOffset;
	int heightValue;
	int groundY;
	int remaining;

	screenX = m_clipScreenX;
	screenY = m_clipScreenY;
	defaultGroundType = m_map->m_defaultBlox;
	defaultGroundData = m_map->m_defaultBloxData;
	baseZ = ((short) p_mapY + (short) p_mapX) * 0x40;
	if (baseZ < 0) {
		baseZ = 0;
	}
	processed = 0;
	m_lemmingAnims->m_primitiveSequence = baseZ;

	if (p_count > 0) {
		do {
			if (p_mapX >= 0 && p_mapY >= 0 && p_mapX < m_groundWidth && p_mapY < m_groundHeight) {
				break;
			}
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
			processed++;
			p_mapX += m_clipMapStepX;
			p_mapY += m_clipMapStepY;
		} while (processed < p_count);
	}

	if (processed < p_count) {
		delayed = 0;
		groundWidth = m_groundWidth;
		ground = m_map->m_ground.m_ground + m_map->m_ground.m_width * p_mapY + p_mapX;
		groundStep = 1 - groundWidth;

		for (;
			 processed < p_count && p_mapX >= 0 && p_mapY >= 0 && p_mapX < m_groundWidth && p_mapY < m_groundHeight;) {
			if ((ground->m_collision & GROUND_COLLISION_SPECIAL_RENDER) == 0) {
				groundData = ground->m_objectData;
				memcpy(&height, &ground->m_height, sizeof(height));
				cliff = ground->m_cliff;
				groundType = ground->m_objectType;
				if (height < 0) {
					height = 0;
					groundType = defaultGroundType;
					groundData = (unsigned short) defaultGroundData;
				}

				heightValue = height;
				groundY = screenY - heightValue;
				zOffset = 0;
				switch (groundType) {
				case TERRAIN_TREE:
					zOffset = 0x20;
					break;
				case TERRAIN_BLOX_1:
					zOffset = 0x10;
					break;
				case TERRAIN_BLOX_2:
				case TERRAIN_BLOX_5:
					zOffset = 8;
					break;
				}
				m_lemmingAnims->m_primitiveSequence = (unsigned short) (height + baseZ + zOffset);

				switch (groundType) {
				case TERRAIN_TREE:
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					DrawGround(screenX, groundY, groundType, groundData);
					break;
				case TERRAIN_BLOX_1:
				case TERRAIN_BLOX_2:
				case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
				case TERRAIN_BLOX_4:
				case TERRAIN_BLOX_5:
				case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
				case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
				case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
				case TERRAIN_CLIFF_ONLY_GROUND:
				case TERRAIN_EMBERS:
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					DrawGround(screenX, groundY, groundType, groundData);
					break;
				case TERRAIN_BLOX_6:
				case TERRAIN_BLOX_7:
					delayed = 1;
					break;
				case TERRAIN_ANIM:
				case TERRAIN_FLAME:
				case TERRAIN_ELECTRIC:
				case TERRAIN_CONVEYOR_VARIANT_A:
				case TERRAIN_CONVEYOR_VARIANT_B:
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					break;
				}

				if (delayed != 0) {
					if (height > 0x18) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					DrawGround(screenX, groundY, groundType, groundData);
					delayed = 0;
				}
			}

			p_mapX += m_clipMapStepX;
			p_mapY += m_clipMapStepY;
			screenX += 0x20;
			processed++;
			ground += groundStep;
		}
	}

	m_lemmingAnims->m_primitiveSequence = baseZ;
	if (processed < p_count) {
		remaining = p_count - processed;
		do {
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
		} while (--remaining != 0);
	}
}

// FUNCTION: LEMBALL 0x0043b4b0
int C2D::DrawClipData()
{
	// GLOBAL: LEMBALL 0x0049ee28
	static int g_clipDebug = 1;

	if (g_clipDebug != 0) {
		g_clipDebug = 0;
	}
	return 0;
}

// GLOBAL: LEMBALL 0x00497218
static const int g_clipMapStepXByOrientation[4] = {1, -1, -1, 1};
// GLOBAL: LEMBALL 0x00497228
static const int g_clipMapStepYByOrientation[4] = {-1, -1, 1, 1};
// GLOBAL: LEMBALL 0x00497238
static const int g_clipNeighborStepXByOrientation[4] = {0, 1, 0, -1};
// GLOBAL: LEMBALL 0x00497248
static const int g_clipNeighborStepYByOrientation[4] = {1, 0, -1, 0};
// GLOBAL: LEMBALL 0x00497258
static const int g_clipRowStepXByOrientation[4] = {1, 1, -1, -1};
// GLOBAL: LEMBALL 0x00497268
static const int g_clipRowStepYByOrientation[4] = {1, -1, -1, 1};

// FUNCTION: LEMBALL 0x0043b4d0
int C2D::DrawClippedRectangle(const CVSRect& p_rect)
{
	int orientationOffset;
	int x;
	int y;
	int width;
	int height;
	CVSRect clippedRect;
	int left;
	int right;
	int bottom;
	int gameX;
	int gameY;
	unsigned int rowCount;
	int result;
	int neighborStepX;
	int neighborStepY;
	int rowStepX;
	int rowStepY;

	orientationOffset = m_viewOrientation;
	m_clipMapStepX = g_clipMapStepXByOrientation[orientationOffset];
	m_clipMapStepY = g_clipMapStepYByOrientation[orientationOffset];
	neighborStepX = g_clipNeighborStepXByOrientation[orientationOffset];
	neighborStepY = g_clipNeighborStepYByOrientation[orientationOffset];
	rowStepX = g_clipRowStepXByOrientation[orientationOffset];
	rowStepY = g_clipRowStepYByOrientation[orientationOffset];
	m_unk0x1464 = 0;

	width = p_rect.m_width;
	height = p_rect.m_height;
	x = p_rect.m_x;
	y = p_rect.m_y;
	if (x + width > m_clipSize.m_x) {
		width = m_clipSize.m_x - x;
	}
	if (y + height > m_clipSize.m_y) {
		height = m_clipSize.m_y - y;
	}

	clippedRect.m_width = (short) width;
	clippedRect.m_height = (short) height;
	clippedRect.m_x = (short) x;
	clippedRect.m_y = (short) y;
	CClipRect& clipRect = m_clipRects[m_primitiveCount++];
	clipRect.m_bounds.m_width = clippedRect.m_width;
	clipRect.m_bounds.m_height = clippedRect.m_height;
	memcpy(&clipRect.m_bounds.m_x, &clippedRect.m_x, sizeof(short));
	memcpy(&clipRect.m_bounds.m_y, &clippedRect.m_y, sizeof(short));
	clipRect.m_flags = 0;
	clipRect.Draw(m_gdi);

	left = x - 0x10;
	if (left < -0x10) {
		left = -0x10;
	}
	y -= 0x18;
	if (y < -0x18) {
		y = -0x18;
	}
	right = left + width + 0x20;
	if (right > m_clipSize.m_x) {
		right = m_clipSize.m_x;
	}
	bottom = y + height + 0x30;
	if (bottom > m_clipSize.m_y) {
		bottom = m_clipSize.m_y;
	}
	width = right / 0x20 - left / 0x20 + 3;

	m_map->ScreenToGame(m_viewOriginX + left, m_viewOriginY + y, gameX, gameY);
	gameX /= 0x10;
	gameY /= 0x10;
	m_map->GameToScreen(gameX << GROUND_BLOCK_PIXEL_SHIFT,
						gameY << GROUND_BLOCK_PIXEL_SHIFT,
						m_clipScreenX,
						m_clipScreenY);
	m_clipScreenX -= m_viewOriginX;
	m_clipScreenY -= m_viewOriginY;

	if (y < bottom) {
		rowCount = ((unsigned int) (bottom - y) + GROUND_BLOCK_PIXEL_MASK) >> GROUND_BLOCK_PIXEL_SHIFT;
		y += rowCount << GROUND_BLOCK_PIXEL_SHIFT;
		do {
			DoClipWidth(gameX, gameY, width);
			m_clipScreenX -= 0x10;
			m_clipScreenY += 8;
			DoClipWidth(gameX + neighborStepX, gameY + neighborStepY, width + 1);
			gameX += rowStepX;
			gameY += rowStepY;
			m_clipScreenX += 0x10;
			m_clipScreenY += 8;
		} while (--rowCount != 0);
	}

	bottom = y + m_clipSearchHeight;
	m_map->ScreenToGame(m_viewOriginX + left, m_viewOriginY + y, gameX, gameY);
	gameX /= 0x10;
	gameY /= 0x10;
	m_map->GameToScreen(gameX << GROUND_BLOCK_PIXEL_SHIFT,
						gameY << GROUND_BLOCK_PIXEL_SHIFT,
						m_clipScreenX,
						m_clipScreenY);
	m_clipScreenX -= m_viewOriginX;
	m_clipScreenY -= m_viewOriginY;

	if (y < bottom) {
		rowCount = ((unsigned int) (bottom - y) + GROUND_BLOCK_PIXEL_MASK) >> GROUND_BLOCK_PIXEL_SHIFT;
		do {
			DoClipWidthSearch(gameX, gameY, width);
			m_clipScreenX -= 0x10;
			m_clipScreenY += 8;
			DoClipWidthSearch(gameX + neighborStepX, gameY + neighborStepY, width + 1);
			gameX += rowStepX;
			gameY += rowStepY;
			m_clipScreenX += 0x10;
			m_clipScreenY += 8;
		} while (--rowCount != 0);
	}

	result = DrawClipData();
	CCopyToBackBuff& bitmap = m_backBufferCopies[m_backBufferCopyCount];
	bitmap.m_x = clippedRect.m_x;
	bitmap.m_y = clippedRect.m_y;
	bitmap.m_destination.m_width = clippedRect.m_width;
	bitmap.m_destination.m_height = clippedRect.m_height;
	bitmap.m_destination.m_x = clippedRect.m_x;
	bitmap.m_destination.m_y = clippedRect.m_y;
	m_backBufferCopies[m_backBufferCopyCount].Draw(m_gdi);
	m_backBufferCopyCount++;
	return result;
}

// GLOBAL: LEMBALL 0x00497018
static const short g_lemmingFlyOffsets[8][2] = {
	{10, 16},
	{10, 16},
	{14, 17},
	{14, 17},
	{11, 18},
	{11, 18},
	{7, 18},
	{7, 18},
};

// GLOBAL: LEMBALL 0x0049eef8
static unsigned long g_lemmingFlyResources[] = {
	RES_GAME_JUMP_NE,
	RES_GAME_JUMP_NE,
	RES_GAME_JUMP_SE,
	RES_GAME_JUMP_SE,
	RES_GAME_JUMP_SW,
	RES_GAME_JUMP_SW,
	RES_GAME_JUMP_NW,
	RES_GAME_JUMP_NW,
};

// GLOBAL: LEMBALL 0x0049efa8
static unsigned long g_lemmingExternalResources[] = {
	RES_GAME_LEM_LASER_N,
	RES_GAME_LEM_LASER_E,
	RES_GAME_LEM_LASER_E,
	RES_GAME_LEM_LASER_S,
	RES_GAME_LEM_LASER_S,
	RES_GAME_LEM_LASER_W,
	RES_GAME_LEM_LASER_W,
	RES_GAME_LEM_LASER_N,
};

// FUNCTION: LEMBALL 0x0043bce0
unsigned long C2D::LemmingFly(CViewData& p_viewData, int& p_frame)
{
	unsigned int direction =
		((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	int frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;

	p_frame = 0;
	if (frameDelta < 0) {
		return g_lemmingFlyResources[direction];
	}

	CMap* map;
	int viewX;
	int viewY;
	int blockX;
	int blockY;
	viewY = (unsigned short) p_viewData.m_gameY;
	map = m_map;
	viewX = (unsigned short) p_viewData.m_gameX;
	blockY = viewY >> GROUND_BLOCK_PIXEL_SHIFT;
	blockX = viewX >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short groundZ;
	if (viewX >= 0 && viewY >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
		groundZ = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(viewX & GROUND_BLOCK_PIXEL_MASK,
																					   viewY & GROUND_BLOCK_PIXEL_MASK);
	}
	else {
		groundZ = 0;
	}

	if (p_viewData.m_positionZ <= groundZ) {
		p_frame = frameDelta * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND + 7;
		if (p_frame > 12) {
			p_frame = 12;
		}
	}
	else {
		p_frame = frameDelta * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (p_frame > 6) {
			p_frame = 6;
		}
	}
	return g_lemmingFlyResources[direction];
}

// FUNCTION: LEMBALL 0x0043bde0
void C2D::DrawLemmingFlyShadow(CViewData& p_viewData)
{
	int viewX;
	int viewY;
	int screenX;
	CMap* map;
	unsigned short groundZ;

	viewX = (unsigned short) p_viewData.m_gameX;
	map = m_map;
	viewY = (unsigned short) p_viewData.m_gameY;
	int blockX = viewX >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = viewY >> GROUND_BLOCK_PIXEL_SHIFT;
	if (viewX >= 0 && viewY >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
		groundZ = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(viewX & GROUND_BLOCK_PIXEL_MASK,
																					   viewY & GROUND_BLOCK_PIXEL_MASK);
	}
	else {
		groundZ = 0;
	}

	screenX = (viewX << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS;
	viewY = (viewY << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS;
	m_map->GameToScreen(screenX, viewY);
	viewX = viewY - ((int) ((unsigned int) groundZ << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS);
	int drawX = (screenX - m_viewOriginX) << FIXED_POINT_FRACTION_BITS;
	int drawY = (viewX - m_viewOriginY) << FIXED_POINT_FRACTION_BITS;

	m_lemmingAnims->DrawAnim((short) (drawX >> FIXED_POINT_FRACTION_BITS),
							 (short) (drawY >> FIXED_POINT_FRACTION_BITS),
							 RES_GAME_BALLOON_SHADOW,
							 0,
							 0,
							 NULL);
}

// FUNCTION: LEMBALL 0x0043bee0
void C2D::DrawLemmingJump(CViewData& p_viewData, unsigned int p_remapped)
{
	unsigned int direction;
	int frame;
	int frameDelta;
	int y;
	unsigned long resource;
	int x;
	unsigned int actionArgument;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	x = p_viewData.m_positionX - g_lemmingFlyOffsets[direction][0];
	y = p_viewData.m_positionY - g_lemmingFlyOffsets[direction][1];
	resource = g_lemmingFlyResources[direction];
	frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	actionArgument = (unsigned short) p_viewData.m_actionArgument;
	switch (actionArgument) {
	case JUMP_ANIMATION_EARLY_FRAMES:
		frame = frameDelta * 15 / 1024;
		if (frame > 6) {
			frame = 6;
		}
		break;
	case JUMP_ANIMATION_LATE_FRAMES:
		frame = frameDelta * 15 / 1024 + 7;
		if (frame > 12) {
			frame = 12;
		}
		break;
	}

	if (p_remapped != 0) {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, (CRemap*) m_paletteRemap);
	}
	else {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, NULL);
	}
}

// FUNCTION: LEMBALL 0x0043bfc0
void C2D::DrawLemmingLanding(CViewData& p_viewData, unsigned int p_remapped)
{
	int x;
	int y;
	unsigned long resource;
	unsigned int direction;
	int frame;
	int frameDelta;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	resource = g_lemmingFlyResources[direction];
	x = p_viewData.m_positionX - g_lemmingFlyOffsets[direction][0];
	y = p_viewData.m_positionY - g_lemmingFlyOffsets[direction][1];
	frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	frame = frameDelta * 15 / 1024 + 7;
	if (frame > 12) {
		frame = 12;
	}

	if (p_remapped != 0) {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, (CRemap*) m_paletteRemap);
	}
	else {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, NULL);
	}
}

// FUNCTION: LEMBALL 0x0043c070
void C2D::DrawLemmingFall(CViewData& p_viewData, unsigned int p_remapped)
{
	DrawLemmingJump(p_viewData, p_remapped);
}

// FUNCTION: LEMBALL 0x0043c090
void C2D::DrawLemmingExternal(CViewData& p_viewData, unsigned int p_remapped)
{
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	int frameDelta = (int) p_viewData.m_animationTime - (int) p_viewData.m_stateTimer;
	unsigned int frame = frameDelta * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
	unsigned int direction =
		((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	CRemap* remap;

	if ((int) frame < 0) {
		frame = 0;
	}
	if (p_remapped != 0) {
		remap = (CRemap*) m_paletteRemap;
	}
	else {
		remap = NULL;
	}

	switch ((unsigned short) p_viewData.m_actionArgument) {
	case EXTERNAL_CONTROL_ELECTROCUTED: {
		unsigned int frameIndex = (int) frame % 4;
		m_lemmingAnims->DrawAnim(x - 15, y - 22, g_lemmingExternalResources[direction], frameIndex, 0, remap);
		break;
	}
	case EXTERNAL_CONTROL_ON_FIRE:
		if ((int) frame <= 14) {
			m_lemmingAnims->DrawAnim(x - 7, y - 28, RES_GAME_ONFIRE, frame, 0, remap);
		}
		break;
	case EXTERNAL_CONTROL_ON_ICE: {
		unsigned int frameIndex = (int) frame % 8;
		m_lemmingAnims->DrawAnim(x - 15, y - 22, RES_GAME_LEMMING_SPIN, frameIndex, 0, remap);
		break;
	}
	}
}

// FUNCTION: LEMBALL 0x0043c1a0
void C2D::DrawLemmingOnConveyor(CViewData& p_viewData, int p_remapped)
{
	int x;
	int y;
	int frame;
	CBaseRemap* remap;

	frame = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	frame = (frame * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND) % LEMMING_CONVEYOR_ANIMATION_FRAME_COUNT;
	if (p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = NULL;
	}
	m_lemmingAnims->DrawAnim(x - 15, y - 22, RES_GAME_LEMMING_SPIN, frame, 0, (CRemap*) remap);
}

// GLOBAL: LEMBALL 0x00496fd8
static const short g_lemmingStandOffset[] = {8, 18};

// GLOBAL: LEMBALL 0x00496fdc
static const short g_lemmingWalkOffset[] = {8, 18};
// GLOBAL: LEMBALL 0x00496fe0
static const short g_lemmingAirOffset[] = {4, 12};
// GLOBAL: LEMBALL 0x00496fe8
static const short g_lemmingWaitOffsets[][2] = {{13, 25}, {10, 18}, {7, 14}, {7, 14}};
// GLOBAL: LEMBALL 0x00496ff8
static const short g_lemmingHitOffsets[][2] =
	{{28, 25}, {24, 24}, {24, 31}, {19, 25}, {23, 24}, {25, 21}, {32, 36}, {25, 26}};
// GLOBAL: LEMBALL 0x00497038
static const short g_lemmingSommersaultOffset[] = {14, 27};
// GLOBAL: LEMBALL 0x0049ee30
static int g_lemmingFireOffsets[][2] = {{11, 9}, {8, 5}, {10, 5}, {16, 4}, {19, 4}, {26, 6}, {26, 9}, {14, 13}};
// GLOBAL: LEMBALL 0x0049ee78
static unsigned long g_lemmingStandResources[] = {RES_GAME_LEMMINGSTANDNE,
												  RES_GAME_LEMMINGSTANDE,
												  RES_GAME_LEMMINGSTANDSE,
												  RES_GAME_LEMMINGSTANDS,
												  RES_GAME_LEMMINGSTANDSW,
												  RES_GAME_LEMMINGSTANDW,
												  RES_GAME_LEMMINGSTANDNW,
												  RES_GAME_LEMMINGSTANDN};
// GLOBAL: LEMBALL 0x0049ee98
static unsigned long g_lemmingFireResources[] = {RES_GAME_LEMMINGFIRENE,
												 RES_GAME_LEMMINGFIREE,
												 RES_GAME_LEMMINGFIRESE,
												 RES_GAME_LEMMINGFIRES,
												 RES_GAME_LEMMINGFIRESW,
												 RES_GAME_LEMMINGFIREW,
												 RES_GAME_LEMMINGFIRENW,
												 RES_GAME_LEMMINGFIREN};
// GLOBAL: LEMBALL 0x0049eed8
static unsigned long g_lemmingHitResources[] = {RES_GAME_HIT_NORTH_EAST,
												RES_GAME_HIT_EAST,
												RES_GAME_HIT_SOUTH_EAST,
												RES_GAME_HIT_SOUTH,
												RES_GAME_HIT_SOUTH_WEST,
												RES_GAME_HIT_WEST,
												RES_GAME_HIT_NORTH_WEST,
												RES_GAME_HIT_NORTH};
// GLOBAL: LEMBALL 0x0049ef18
static unsigned long g_lemmingWalkResources[] = {RES_GAME_LEMMINGWALKNE,
												 RES_GAME_LEMMINGWALKE,
												 RES_GAME_LEMMINGWALKSE,
												 RES_GAME_LEMMINGWALKS,
												 RES_GAME_LEMMINGWALKSW,
												 RES_GAME_LEMMINGWALKW,
												 RES_GAME_LEMMINGWALKNW,
												 RES_GAME_LEMMINGWALKN};
// GLOBAL: LEMBALL 0x0049ef98
static unsigned long g_lemmingWaitResources[] = {RES_GAME_WAIT_JIG,
												 RES_GAME_WAIT_TOSS,
												 RES_GAME_WAIT_LOOK,
												 RES_GAME_WAIT_LOOK};

// FUNCTION: LEMBALL 0x0043c200
void C2D::DrawLemming(CViewData& p_viewData, int p_objectNo, unsigned int p_remapped)
{
	int x;
	int y;
	int animationFrame;
	unsigned int direction;
	unsigned short playerIndex;
	int drawEquipment;
	int drawBody;
	unsigned long animationResourceId;
	int offsetX;
	int offsetY;

	animationFrame = p_viewData.m_stateTimer;
	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	y = p_viewData.m_positionY;
	drawEquipment = 1;
	drawBody = 1;
	x = p_viewData.m_positionX;
	playerIndex = p_viewData.m_playerIndex;

	switch (p_viewData.m_action) {
	case ACTION_NONE:
	case ACTION_TURNING:
	case ACTION_FINDING_ROUTE:
	case ACTION_PREPARING_SOMMERSAULT:
	case ACTION_LEAVING:
		animationResourceId = g_lemmingStandResources[direction];
		offsetX = g_lemmingStandOffset[0];
		offsetY = g_lemmingStandOffset[1];
		break;
	case ACTION_WALKING:
		animationResourceId = g_lemmingWalkResources[direction];
		offsetX = g_lemmingWalkOffset[0];
		offsetY = g_lemmingWalkOffset[1];
		break;
	case ACTION_FIRING:
		animationResourceId = g_lemmingFireResources[direction];
		offsetY = g_lemmingFireOffsets[direction][1] + g_lemmingStandOffset[1];
		offsetX = g_lemmingFireOffsets[direction][0] + g_lemmingStandOffset[0];
		drawEquipment = 0;
		break;
	case ACTION_FLYING:
		DrawLemmingFlyShadow(p_viewData);
		drawEquipment = 0;
		animationResourceId = LemmingFly(p_viewData, animationFrame);
		offsetX = g_lemmingAirOffset[0];
		offsetY = g_lemmingAirOffset[1];
		break;
	case ACTION_HIDDEN:
		drawBody = 0;
		drawEquipment = 0;
		break;
	case ACTION_IDLE_ANIMATION: {
		unsigned int waitAnimationIndex;
		if (p_remapped == 0) {
			waitAnimationIndex = (unsigned short) p_viewData.m_actionArgument;
		}
		else {
			waitAnimationIndex = 0;
		}
		offsetX = g_lemmingWaitOffsets[waitAnimationIndex][0];
		offsetY = g_lemmingWaitOffsets[waitAnimationIndex][1];
		animationResourceId = g_lemmingWaitResources[waitAnimationIndex];
		break;
	}
	case ACTION_HIT:
		drawEquipment = 0;
		animationResourceId = g_lemmingHitResources[direction];
		offsetX = g_lemmingHitOffsets[direction][0];
		offsetY = g_lemmingHitOffsets[direction][1];
		break;
	case ACTION_DEAD:
	case ACTION_WAITING_TO_SPAWN:
	case ACTION_WAITING_TO_DIE:
		return;
	case ACTION_JUMPING:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingJump(p_viewData, p_remapped);
		break;
	case ACTION_FALLING:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingFall(p_viewData, p_remapped);
		break;
	case ACTION_SOMMERSAULT:
		offsetX = g_lemmingSommersaultOffset[0];
		offsetY = g_lemmingSommersaultOffset[1];
		drawEquipment = 0;
		animationResourceId = p_viewData.m_actionArgument == SOMMERSAULT_DIRECTION_NORMAL ? RES_GAME_SOMMERSAULT
																						  : RES_GAME_SOMMERSAULT_REV;
		break;
	case ACTION_EXTERNAL_CONTROL:
		DrawLemmingExternal(p_viewData, p_remapped);
		return;
	case ACTION_ON_BALLOON:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingOnBalloon(p_viewData, (unsigned short) p_viewData.m_actionArgument, p_remapped);
		break;
	case ACTION_LANDING:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingLanding(p_viewData, p_remapped);
		break;
	case ACTION_ON_CONVEYOR:
		DrawLemmingOnConveyor(p_viewData, p_remapped);
		return;
	}
	if (drawEquipment && p_remapped == 0) {
		if (((unsigned short) p_viewData.m_statusFlags & LEMMING_VIEW_STATUS_GROUP_LEADER) == 0) {
			m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] - 1,
									 (short) y - g_lemmingStandOffset[1] + 14,
									 RES_GAME_CIRCLES,
									 0,
									 0,
									 (CRemap*) m_remaps[playerIndex]);
		}
		else if ((unsigned short) p_viewData.m_statusFlags & LEMMING_VIEW_STATUS_IN_GROUP) {
			m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] - 5,
									 (short) y - g_lemmingStandOffset[1] + 11,
									 RES_GAME_FILLED_STARS,
									 0,
									 0,
									 (CRemap*) m_remaps[playerIndex]);
		}
		else {
			m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] - 5,
									 (short) y - g_lemmingStandOffset[1] + 11,
									 RES_GAME_STARS,
									 0,
									 0,
									 (CRemap*) m_remaps[playerIndex]);
		}
	}
	if (drawBody) {
		if (p_remapped == 0) {
			m_lemmingAnims->DrawAnim((short) x - (short) offsetX,
									 (short) y - (short) offsetY,
									 animationResourceId,
									 animationFrame,
									 p_viewData.m_animationTime,
									 NULL);
		}
		else {
			m_lemmingAnims->DrawAnim((short) x - (short) offsetX,
									 (short) y - (short) offsetY,
									 animationResourceId,
									 animationFrame,
									 p_viewData.m_animationTime,
									 (CRemap*) m_paletteRemap);
		}
	}
	if (drawEquipment && p_remapped == 0 && InGroupByObjectNo(p_objectNo)) {
		m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] + 2,
								 (short) y - g_lemmingStandOffset[1] - 16,
								 RES_GAME_SPINARROW,
								 0,
								 p_viewData.m_animationTime,
								 NULL);
	}
}

// GLOBAL: LEMBALL 0x00497070
static const short g_bulletOffset[] = {4, 4};

// FUNCTION: LEMBALL 0x0043c610
void C2D::DrawBullet(CViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049ef38
	static unsigned long g_bulletResources[] = {
		RES_GAME_LEMMINGPELLETNE,
		RES_GAME_LEMMINGPELLETE,
		RES_GAME_LEMMINGPELLETSE,
		RES_GAME_LEMMINGPELLETS,
		RES_GAME_LEMMINGPELLETSW,
		RES_GAME_LEMMINGPELLETW,
		RES_GAME_LEMMINGPELLETNW,
		RES_GAME_LEMMINGPELLETN,
	};

	unsigned int direction;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_bulletOffset[0],
							 p_viewData.m_positionY - g_bulletOffset[1],
							 g_bulletResources[direction],
							 0,
							 p_viewData.m_animationTime,
							 NULL);
}

// GLOBAL: LEMBALL 0x00497064
static const short g_ammoOffset[] = {8, 16};

// GLOBAL: LEMBALL 0x00497068
static const short g_pelletOffset[] = {16, 16};

// FUNCTION: LEMBALL 0x0043c660
void C2D::DrawAmmo(CViewData& p_viewData, int p_objectNo)
{
	switch (p_viewData.m_action) {
	case ACTION_READY:
	case ACTION_ACTIVATING:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_ammoOffset[0],
								 p_viewData.m_positionY - g_ammoOffset[1],
								 RES_GAME_YELLOW_AMMO,
								 0,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	case ACTION_ACTIVATED:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_pelletOffset[0],
								 p_viewData.m_positionY - g_pelletOffset[1],
								 RES_GAME_EX_PELLET,
								 p_viewData.m_stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043c6e0
void C2D::DrawRocket(CViewData& p_viewData)
{
	enum {
		ROCKET_EXTRA_FRAME_NONE = -1
	};
	int elapsed;
	int frame;
	int extraFrame;

	elapsed =
		(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
	extraFrame = ROCKET_EXTRA_FRAME_NONE;
	if (elapsed <= 7) {
		frame = elapsed < 4 ? elapsed : 4;
	}
	else if (elapsed >= 8 && elapsed <= 13) {
		frame = elapsed < 11 ? elapsed - 3 : 8;
	}
	else if (elapsed >= 14 && elapsed <= 19) {
		frame = elapsed - 4;
		extraFrame = 9;
	}
	else if (elapsed >= 20 && elapsed <= 31) {
		frame = (elapsed - 20) % 2 + 17;
		extraFrame = 16;
	}
	else if (elapsed >= 32 && elapsed <= 42) {
		frame = elapsed - 11;
		if (frame > 25) {
			extraFrame = 26;
			frame++;
		}
	}
	else {
		frame = (elapsed & ROCKET_ANIMATION_ALTERNATE_FRAME_MASK) + 33;
		extraFrame = 32;
	}

	if (extraFrame != ROCKET_EXTRA_FRAME_NONE) {
		m_lemmingAnims
			->DrawAnim(p_viewData.m_positionX - 13, p_viewData.m_positionY - 73, RES_GAME_ROCKET, extraFrame, 0, NULL);
	}
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - 13, p_viewData.m_positionY - 73, RES_GAME_ROCKET, frame, 0, NULL);
}

// FUNCTION: LEMBALL 0x0043c7f0
void C2D::DrawHand(CViewData& p_viewData)
{
	int drawX;
	int drawY;
	int frame;
	CBaseRemap* remap;
	eAction action = p_viewData.m_action;

	drawX = p_viewData.m_positionX - 0x31;
	drawY = p_viewData.m_positionY - 0x14;
	remap = NULL;
	if (p_viewData.m_actionArgument != REMOTE_PALETTE_REMAP_DISABLED) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case ACTION_RECOVERY:
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(drawX, drawY, g_anGroundStyleResourceIds[2], 0, 0, NULL);
		break;
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		frame =
			(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(drawX, drawY, g_anGroundStyleResourceIds[2], frame, 0, (CRemap*) remap);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043c8a0
void C2D::DrawLemmingOnBalloon(CViewData& p_viewData, int p_balloonType, int p_remapped)
{
	unsigned int phase;
	int x;
	int y;
	int xOffset;
	int yOffset;
	CBaseRemap* remap;
	CBaseRemap* balloonRemap;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	phase = ((p_viewData.m_animationTime - p_viewData.m_stateTimer) & ANIMATION_PHASE_TIME_MASK) >>
			ANIMATION_PHASE_TIME_SHIFT;

	if (p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = NULL;
	}

	if (phase <= 7) {
		xOffset = phase - 4;
	}
	else {
		xOffset = 12 - phase;
	}

	yOffset = phase - 4;
	if (phase > 7) {
		yOffset = 12 - phase;
	}

	DrawLemmingFlyShadow(p_viewData);

	if (p_balloonType < 4) {
		balloonRemap = m_remaps[p_balloonType];
	}
	else {
		balloonRemap = NULL;
	}

	m_lemmingAnims->DrawAnim(x - 16, y - 64, RES_GAME_BALLOON, 0, 0, (CRemap*) balloonRemap);
	m_lemmingAnims->DrawAnim(x - g_lemmingStandOffset[0] - 14,
							 y - g_lemmingStandOffset[1] - 12,
							 RES_GAME_ONBALLOON,
							 0,
							 0,
							 (CRemap*) remap);
}

// FUNCTION: LEMBALL 0x0043c940
void C2D::DrawBalloon(CViewData& p_viewData, int p_playerIndex)
{
	C2D* view = this;
	CBaseRemap* remap;
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	int xOffset;
	int yOffset;
	unsigned int phase = ((p_viewData.m_animationTime - p_viewData.m_stateTimer) & ANIMATION_PHASE_TIME_MASK) >>
						 ANIMATION_PHASE_TIME_SHIFT;

	if (phase <= 7) {
		xOffset = phase - 4;
	}
	else {
		xOffset = 12 - phase;
	}

	yOffset = phase - 4;
	if (phase > 7) {
		yOffset = 12 - phase;
	}

	if (p_playerIndex < 4) {
		remap = view->m_remaps[p_playerIndex];
	}
	else {
		remap = NULL;
	}

	view->m_lemmingAnims->DrawAnim(x + xOffset - 16, y + yOffset / 4 - 64, RES_GAME_BALLOON, 0, 0, (CRemap*) remap);
	view->m_lemmingAnims->DrawAnim(x + xOffset - 9, y + yOffset / 4 - 9, RES_GAME_BALLOON_SHADOW, 0, 0, NULL);
}

// FUNCTION: LEMBALL 0x0043c9f0
void C2D::DrawBalloonPost(CViewData& p_viewData, int p_playerIndex)
{
	int x;
	int y;
	CBaseRemap* remap;

	x = p_viewData.m_positionX - 0x10;
	y = p_viewData.m_positionY - 0x40;
	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = NULL;
	}
	m_lemmingAnims->DrawAnim(x, y, RES_GAME_BALLOON_POST, 0, 0, (CRemap*) remap);
}

// GLOBAL: LEMBALL 0x00497098
static const short g_trampolineOffset[] = {22, 22};

// FUNCTION: LEMBALL 0x0043ca30
void C2D::DrawTrampoline(CViewData& p_viewData)
{
	int x;
	int y;
	int frame;

	x = p_viewData.m_positionX - g_trampolineOffset[0];
	y = p_viewData.m_positionY - g_trampolineOffset[1];

	switch (p_viewData.m_action) {
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_TRAMPOLINE, 0, 0, NULL);
		break;

	case ACTION_RUNNING:
		frame =
			(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_TRAMPOLINE, frame, 0, NULL);
		break;
	}
}

// GLOBAL: LEMBALL 0x004970b0
static const short g_moverOffset[] = {17, 30};

// FUNCTION: LEMBALL 0x0043cac0
void C2D::DrawMover(CViewData& p_viewData)
{
	unsigned short frame;
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	unsigned short state = p_viewData.m_actionArgument;
	switch (m_ai->m_mapType) {
	case GROUND_STYLE_GRASS:
		frame = 0x50;
		break;
	case GROUND_STYLE_LEGO:
		frame = 0x1a;
		break;
	case GROUND_STYLE_SNOW:
		frame = 0x52;
		break;
	case GROUND_STYLE_SPACE:
		frame = 0x38;
		break;
	}
	switch ((unsigned int) state) {
	case MOVER_VISUAL_GROUND:
		m_lemmingAnims
			->DrawAnim(x - g_groundOffset[0], y - g_groundOffset[1] - 12, g_groundBlox4ResourceId, frame, 0, NULL);
		break;
	case MOVER_VISUAL_STAR: {
		unsigned int animFrame =
			(g_dwSimulationTimestamp / SPECIAL_ANIMATION_FRAME_INTERVAL_MS) & SPECIAL_ANIMATION_FRAME_MASK;
		m_lemmingAnims->DrawAnim(x - g_moverOffset[0], y - g_moverOffset[1] - 8, RES_GAME_STAR, animFrame, 0, NULL);
		break;
	}
	}
}

// GLOBAL: LEMBALL 0x004970a0
extern const short g_slinkyOffsets[][2] = {{13, 25}, {30, 32}, {29, 26}, {14, 34}};

// FUNCTION: LEMBALL 0x0043cbb0
void C2D::DrawSlinky(CViewData& p_viewData)
{
	unsigned int direction = (unsigned short) p_viewData.m_actionArgument;
	int x = p_viewData.m_positionX - g_slinkyOffsets[direction][0];
	int y = p_viewData.m_positionY - g_slinkyOffsets[direction][1];
	int frame;
	switch (p_viewData.m_action) {
	case ACTION_READY:
		switch (direction) {
		case SLINKY_DIRECTION_EAST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_EAST, 0, 0, NULL);
			break;
		case SLINKY_DIRECTION_WEST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_WEST, 0, 0, NULL);
			break;
		case SLINKY_DIRECTION_SOUTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_SOUTH, 0, 0, NULL);
			break;
		case SLINKY_DIRECTION_NORTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_NORTH, 0, 0, NULL);
			break;
		}
		break;
	case ACTION_RUNNING:
		frame = (int) ((p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS) /
				MILLISECONDS_PER_SECOND;
		if (frame > 12) {
			frame = 12;
		}
		switch (direction) {
		case SLINKY_DIRECTION_EAST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_EAST, frame, 0, NULL);
			break;
		case SLINKY_DIRECTION_WEST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_WEST, frame, 0, NULL);
			break;
		case SLINKY_DIRECTION_SOUTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_SOUTH, frame, 0, NULL);
			break;
		case SLINKY_DIRECTION_NORTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_NORTH, frame, 0, NULL);
			break;
		}
		break;
	}
}

// GLOBAL: LEMBALL 0x0049709c
static const short g_paintGunOffset[] = {25, 27};

// FUNCTION: LEMBALL 0x0043cd50
void C2D::DrawPaintGun(CViewData& p_viewData)
{
	int x = p_viewData.m_positionX - g_paintGunOffset[0];
	int y = p_viewData.m_positionY - g_paintGunOffset[1];
	int frame;
	if (p_viewData.m_action == ACTION_FIRING || p_viewData.m_action == ACTION_RUNNING) {
		frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * PAINT_GUN_ANIMATION_FRAME_RATE_FPS /
				MILLISECONDS_PER_SECOND;
		if (frame > 57) {
			frame = 0;
		}
		if (frame < 12) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[1], frame, 0, NULL);
		}
		if (frame > 11 && frame < 47) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[1], 11, 0, NULL);
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_PAINTGUNSHOT, frame - 12, 0, NULL);
		}
		if (frame > 46) {
			m_lemmingAnims->DrawAnim(x, frame + y - 47, g_anGroundStyleResourceIds[1], 11, 0, NULL);
		}
	}
}

// FUNCTION: LEMBALL 0x0043ce30
void C2D::DrawLaserFire(CViewData& p_viewData)
{
	int x;
	int y;

	switch (p_viewData.m_objectType) {
	case OBJECT_LASER_HORIZONTAL_BEAM:
		x = p_viewData.m_positionX - 0xd;
		y = p_viewData.m_positionY - 9;
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_LASER_FIRE_NORTH, 0, 0, NULL);
		break;
	case OBJECT_LASER_VERTICAL_BEAM:
		x = p_viewData.m_positionX - 0x16;
		y = p_viewData.m_positionY - 0xf;
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_LASER_FIRE_EAST, 0, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043cea0
void C2D::DrawLaser(CViewData& p_viewData)
{
	eAction action;
	int x;
	int y;
	unsigned long resourceId;
	int frame;

	action = p_viewData.m_action;

	switch (p_viewData.m_objectType) {
	case OBJECT_LASER_HORIZONTAL:
	case OBJECT_LASER_EMITTER_H:
		resourceId = RES_GAME_LASER_EAST;
		x = p_viewData.m_positionX - 0x14;
		y = p_viewData.m_positionY - 0xa;
		break;
	case OBJECT_LASER_VERTICAL:
	case OBJECT_LASER_EMITTER_V:
		resourceId = RES_GAME_LASER_NORTH;
		x = p_viewData.m_positionX - 0x2e;
		y = p_viewData.m_positionY - 0xa;
		break;
	}

	switch (action) {
	case ACTION_RECOVERY:
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		break;
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		frame =
			(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 17) {
			frame = 17;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, frame, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043cfa0
void C2D::DrawDuplicator(CViewData& p_viewData)
{
	int x;
	int y;
	CBaseRemap* remap;
	eAction action;
	unsigned int elapsed;
	int frame;

	elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	action = p_viewData.m_action;
	x = p_viewData.m_positionX - 0x1c;
	y = p_viewData.m_positionY - 0x3f;
	remap = NULL;

	if (p_viewData.m_actionArgument != REMOTE_PALETTE_REMAP_DISABLED) {
		remap = m_paletteRemap;
	}
	m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, 0, 0, (CRemap*) remap);

	switch (action) {
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, 0x3f, 0, (CRemap*) remap);
		break;
	case ACTION_ACTIVATED:
		frame = elapsed * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 0x3e) {
			frame = 0x3e;
		}
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, frame + 1, 0, (CRemap*) remap);
		break;
	}
}

// GLOBAL: LEMBALL 0x0049703c
static const short g_crateOffset[] = {8, 24};

// GLOBAL: LEMBALL 0x00497040
static const short g_crateExplosionOffset[] = {34, 50};

// FUNCTION: LEMBALL 0x0043d070
void C2D::DrawCrate(CViewData& p_viewData, int p_objectNo)
{
	eAction action;
	unsigned int stateTimer;

	action = p_viewData.m_action;
	stateTimer = p_viewData.m_stateTimer;

	switch (action) {
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_crateOffset[0],
								 p_viewData.m_positionY - g_crateOffset[1],
								 RES_GAME_CRATE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_crateExplosionOffset[0],
								 p_viewData.m_positionY - g_crateExplosionOffset[1],
								 RES_GAME_CRATE_EXPLODE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// GLOBAL: LEMBALL 0x00497088
static const short g_timeBonusOffset[] = {16, 18};

// FUNCTION: LEMBALL 0x0043d0f0
void C2D::DrawTimeBonus(CViewData& p_viewData)
{
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_timeBonusOffset[0],
							 p_viewData.m_positionY - g_timeBonusOffset[1],
							 RES_GAME_TIME_BONUS,
							 p_viewData.m_stateTimer,
							 p_viewData.m_animationTime,
							 NULL);
}

// GLOBAL: LEMBALL 0x0049706c
extern const short g_sheepOffset[] = {9, 8};

// FUNCTION: LEMBALL 0x0043d370
void C2D::DrawSheep(CViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049ef58
	static unsigned long g_sheepWalkResources[] = {
		RES_GAME_SHEEP_WALK_NE,
		RES_GAME_SHEEP_WALK_E,
		RES_GAME_SHEEP_WALK_SE,
		RES_GAME_SHEEP_WALK_S,
		RES_GAME_SHEEP_WALK_SW,
		RES_GAME_SHEEP_WALK_W,
		RES_GAME_SHEEP_WALK_NW,
		RES_GAME_SHEEP_WALK_N,
	};
	// GLOBAL: LEMBALL 0x0049ef78
	static unsigned long g_sheepMunchResources[] = {
		RES_GAME_SHEEP_MUNCH_NE,
		RES_GAME_SHEEP_WALK_E,
		RES_GAME_SHEEP_MUNCH_SE,
		RES_GAME_SHEEP_WALK_S,
		RES_GAME_SHEEP_MUNCH_SW,
		RES_GAME_SHEEP_WALK_W,
		RES_GAME_SHEEP_MUNCH_NW,
		RES_GAME_SHEEP_WALK_N,
	};

	unsigned int direction;
	unsigned int stateTimer;
	int y;
	int x;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	stateTimer = p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case ACTION_NONE:
	case ACTION_TURNING:
	case ACTION_FLYING:
		m_lemmingAnims->DrawAnim(x - g_sheepOffset[0],
								 y - g_sheepOffset[1],
								 g_sheepMunchResources[direction],
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;

	case ACTION_WALKING:
		m_lemmingAnims->DrawAnim(x - g_sheepOffset[0],
								 y - g_sheepOffset[1],
								 g_sheepWalkResources[direction],
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// GLOBAL: LEMBALL 0x00497074
static const short g_ballOffset[] = {10, 15};

// GLOBAL: LEMBALL 0x00497078
static const short g_explosionOffset[] = {15, 17};

// FUNCTION: LEMBALL 0x0043d420
void C2D::DrawBall(CViewData& p_viewData)
{
	int x;
	int y;
	int elapsed;
	int frame;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case ACTION_BALL_MOVING:
		m_lemmingAnims
			->DrawAnim(x - g_ballOffset[0], y - g_ballOffset[1], RES_GAME_BALL, 0, p_viewData.m_animationTime, NULL);
		break;
	case ACTION_BALL_EXPLODING:
		elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
		frame = elapsed / 64;
		if (frame > 8) {
			frame = 8;
		}
		m_lemmingAnims
			->DrawAnim(x - g_explosionOffset[0], y - g_explosionOffset[1], RES_GAME_BALL_EXPLODE, frame, 0, NULL);
		break;
	}
}

// GLOBAL: LEMBALL 0x00497054
static const short g_keyOffset[] = {8, 32};

// FUNCTION: LEMBALL 0x0043d4b0
void C2D::DrawKey(CViewData& p_viewData, int p_playerIndex)
{
	CBaseRemap* remap;

	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = NULL;
	}

	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_keyOffset[0],
							 p_viewData.m_positionY - g_keyOffset[1],
							 RES_GAME_KEYS,
							 0,
							 0,
							 (CRemap*) remap);
}

// GLOBAL: LEMBALL 0x0049704c
static const short g_mineOffset[] = {30, 35};

// GLOBAL: LEMBALL 0x00497050
static const short g_mineStillOffset[] = {2, 2};

// FUNCTION: LEMBALL 0x0043d500
void C2D::DrawMine(CViewData& p_viewData)
{
	int x;
	int y;
	unsigned int stateTimer;
	eAction action;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	stateTimer = p_viewData.m_stateTimer;
	action = p_viewData.m_action;

	switch (action) {
	case ACTION_DEAD:
		break;
	case ACTION_READY:
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		m_lemmingAnims->DrawAnim(x - g_mineStillOffset[0], y - g_mineStillOffset[1], RES_GAME_MINE_STILL, 0, 0, NULL);
		break;
	case ACTION_RUNNING:
		m_lemmingAnims->DrawAnim(x - g_mineOffset[0],
								 y - g_mineOffset[1],
								 RES_GAME_MINE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// GLOBAL: LEMBALL 0x00497094
static const short g_doorOffset[] = {26, 24};

// FUNCTION: LEMBALL 0x0043d590
void C2D::DrawDoor(CViewData& p_viewData)
{
	int x;
	int y;
	int elapsed;
	eAction action;
	eObjectType objectType;
	unsigned long resourceId;
	int playerIndex;
	CBaseRemap* remap;
	int frame;

	x = p_viewData.m_positionX - g_doorOffset[0];
	y = p_viewData.m_positionY - g_doorOffset[1];
	action = p_viewData.m_action;
	objectType = p_viewData.m_objectType;
	elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;

	switch (objectType) {
	case OBJECT_DOOR_1:
		resourceId = RES_GAME_DOOR_2;
		break;
	case OBJECT_DOOR_2:
		resourceId = RES_GAME_DOOR;
		break;
	}

	switch (action) {
	case ACTION_DOOR_LOCKED_FEEDBACK:
		switch ((unsigned short) p_viewData.m_actionArgument) {
		case OBJECT_SWITCH:
			playerIndex = DOOR_LOCK_NO_PLAYER_REMAP;
			break;
		case OBJECT_KEY_1:
			playerIndex = 3;
			break;
		case OBJECT_KEY_2:
			playerIndex = 1;
			break;
		case OBJECT_KEY_3:
			playerIndex = 4;
			break;
		}

		if (playerIndex >= 0) {
			if (playerIndex < 4) {
				remap = m_remaps[playerIndex];
			}
			else {
				remap = NULL;
			}
			m_lemmingAnims->DrawAnim(x + 16, y - 20, RES_GAME_KEYS, 0, 0, (CRemap*) remap);
		}

		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 1, 0, NULL);
		break;

	case ACTION_DOOR_LOCKED:
	case ACTION_DOOR_CLOSED:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 1, 0, NULL);
		break;

	case (eAction) ACTION_DOOR_OPENING:
		frame = elapsed * 15 / 1024;
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, frame + 1, 0, NULL);
		break;

	case (eAction) ACTION_DOOR_OPEN:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 8, 0, NULL);
		break;

	case (eAction) ACTION_DOOR_CLOSING:
		frame = elapsed * 15 / 1024;
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 8 - frame, 0, NULL);
		break;
	}
}

// GLOBAL: LEMBALL 0x0049707c
static const short g_switchOffset[] = {5, 25};

// FUNCTION: LEMBALL 0x0043d7e0
void C2D::DrawSwitch(CViewData& p_viewData)
{
	int x;
	int y;
	unsigned int stateTimer;
	unsigned short actionArgument;
	eAction action;

	x = p_viewData.m_positionX - g_switchOffset[0];
	y = p_viewData.m_positionY - g_switchOffset[1];
	stateTimer = p_viewData.m_stateTimer;
	action = p_viewData.m_action;
	actionArgument = (unsigned short) p_viewData.m_actionArgument;

	switch (action) {
	case ACTION_HIT:
	case ACTION_READY:
		switch (actionArgument) {
		case SWITCH_STATE_INACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH, 0, 0, NULL);
			break;
		case SWITCH_STATE_ACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH, 0, 0, NULL);
			break;
		}
		break;

	case ACTION_ACTIVATED:
		switch (actionArgument) {
		case SWITCH_STATE_INACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH_ANIM, stateTimer, p_viewData.m_animationTime, NULL);
			break;
		case SWITCH_STATE_ACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH_ANIM, stateTimer, p_viewData.m_animationTime, NULL);
			break;
		}
		break;
	}
}

// GLOBAL: LEMBALL 0x00497080
static const short g_flagOffset[] = {15, 28};

// FUNCTION: LEMBALL 0x0043d8d0
void C2D::DrawFlag(CViewData& p_viewData, eObjectType p_objectType)
{
	int x;
	int y;

	x = p_viewData.m_positionX - g_flagOffset[0];
	y = p_viewData.m_positionY - g_flagOffset[1];

	switch (p_objectType) {
	case OBJECT_FLAG_1:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_FLAG_GREEN, 0, p_viewData.m_animationTime, (CRemap*) m_remaps[3]);
		break;

	case OBJECT_FLAG_2:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_FLAG_GREEN, 0, p_viewData.m_animationTime, (CRemap*) m_remaps[1]);
		break;
	}
}

// GLOBAL: LEMBALL 0x00497084
static const short g_bonusOffset[] = {16, 16};

// FUNCTION: LEMBALL 0x0043d950
void C2D::DrawBonus(CViewData& p_viewData)
{
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_bonusOffset[0],
							 p_viewData.m_positionY - g_bonusOffset[1],
							 RES_GAME_BONUS,
							 0,
							 p_viewData.m_animationTime,
							 NULL);
}

// GLOBAL: LEMBALL 0x00497090
static const short g_trapDoorOffset[] = {48, 40};

// FUNCTION: LEMBALL 0x0043d990
void C2D::DrawTrapDoor(CViewData& p_viewData)
{
	int frame;
	int x = p_viewData.m_positionX - g_trapDoorOffset[0];
	int y = p_viewData.m_positionY - g_trapDoorOffset[1];
	int shadowX = x + 16;
	int shadowY = y + 78;
	frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) / 66;
	switch (p_viewData.m_action) {
	case ACTION_ARRIVING:
		if (frame > 40) {
			frame = 40;
		}
		if (frame < 33) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], frame, 0, NULL);
		}
		else {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 33, 0, NULL);
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], frame + 1, 0, NULL);
		}
		if (frame > 23) {
			if (frame > 33) {
				frame = 33;
			}
			m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, frame - 23, 0, NULL);
		}
		break;
	case ACTION_OPENING:
		if (frame > 14) {
			frame = 14;
		}
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, frame + 1, 0, NULL);
		m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		break;
	case ACTION_OPEN:
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 15, 0, NULL);
		m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		break;
	case ACTION_CLOSING:
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 8 - frame, 0, NULL);
		m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		break;
	case ACTION_LEAVING:
		if (frame > 40) {
			frame = 40;
		}
		if (40 - frame < 33) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 40 - frame, 0, NULL);
		}
		else {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 33, 0, NULL);
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 41 - frame, 0, NULL);
		}
		if (frame < 13) {
			m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		}
		else if (frame < 23) {
			m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 23 - frame, 0, NULL);
		}
		break;
	}
}

extern const unsigned int* g_styleObjectClip;

// FUNCTION: LEMBALL 0x0043dc70
void C2D::DrawObject(CViewData& p_viewData)
{
	int objectNo = p_viewData.m_objectId;
	switch (p_viewData.m_objectType) {
	case OBJECT_PLAYER_1:
		DrawLemming(p_viewData, objectNo, 1);
		return;
	case OBJECT_PLAYER_2:
		DrawLemming(p_viewData, objectNo, 0);
		return;
	case OBJECT_BULLET:
		DrawBullet(p_viewData, objectNo);
		return;
	case OBJECT_CATAPULT:
		DrawCatapult(p_viewData, objectNo);
		return;
	case OBJECT_AMMO:
		DrawAmmo(p_viewData, objectNo);
		return;
	case OBJECT_SHEEP:
		DrawSheep(p_viewData, objectNo);
		return;
	case OBJECT_BALL:
		DrawBall(p_viewData);
		return;
	case OBJECT_FLAG_1:
		DrawFlag(p_viewData, OBJECT_FLAG_1);
		return;
	case OBJECT_FLAG_2:
		DrawFlag(p_viewData, OBJECT_FLAG_2);
		return;
	case OBJECT_TOWER:
		if (m_ai->m_mapType != GROUND_STYLE_SPACE) {
			m_lemmingAnims->DrawAnim((short) p_viewData.m_positionX - (short) g_styleObjectClip[0],
									 (short) p_viewData.m_positionY - (short) g_styleObjectClip[1],
									 g_anGroundStyleResourceIds[6],
									 0,
									 0,
									 NULL);
		}
		return;
	case OBJECT_CRATE:
		DrawCrate(p_viewData, objectNo);
		return;
	case OBJECT_BONUS:
		DrawBonus(p_viewData);
		return;
	case OBJECT_MINE:
		DrawMine(p_viewData);
		return;
	case OBJECT_SWITCH:
		DrawSwitch(p_viewData);
		return;
	case OBJECT_KEY_1:
		DrawKey(p_viewData, 3);
		return;
	case OBJECT_KEY_2:
		DrawKey(p_viewData, 1);
		return;
	case OBJECT_KEY_3:
		DrawKey(p_viewData, 4);
		return;
	case OBJECT_TRAP_DOOR:
		DrawTrapDoor(p_viewData);
		return;
	case OBJECT_DOOR_1:
	case OBJECT_DOOR_2:
		DrawDoor(p_viewData);
		return;
	case OBJECT_TIME_BONUS:
		DrawTimeBonus(p_viewData);
		return;
	case OBJECT_DUPLICATOR:
		DrawDuplicator(p_viewData);
		return;
	case OBJECT_LASER_HORIZONTAL:
	case OBJECT_LASER_VERTICAL:
	case OBJECT_LASER_EMITTER_H:
	case OBJECT_LASER_EMITTER_V:
		DrawLaser(p_viewData);
		return;
	case OBJECT_HAND:
		DrawHand(p_viewData);
		return;
	case OBJECT_ROCKET:
		DrawRocket(p_viewData);
		return;
	case OBJECT_PAINT_GUN:
		DrawPaintGun(p_viewData);
		return;
	case OBJECT_TRAMPOLINE:
		DrawTrampoline(p_viewData);
		return;
	case OBJECT_LASER_HORIZONTAL_BEAM:
	case OBJECT_LASER_VERTICAL_BEAM:
		DrawLaserFire(p_viewData);
		return;
	case OBJECT_BALLOON_0:
		DrawBalloon(p_viewData, 3);
		return;
	case OBJECT_BALLOON_1:
		DrawBalloonPost(p_viewData, 3);
		return;
	case OBJECT_BALLOON_2:
		DrawBalloon(p_viewData, 1);
		return;
	case OBJECT_BALLOON_3:
		DrawBalloonPost(p_viewData, 1);
		return;
	case OBJECT_BALLOON_4:
		DrawBalloon(p_viewData, 4);
		return;
	case OBJECT_BALLOON_5:
		DrawBalloonPost(p_viewData, 4);
		return;
	case OBJECT_BALLOON_6:
		DrawBalloon(p_viewData, 0);
		return;
	case OBJECT_BALLOON_7:
		DrawBalloonPost(p_viewData, 0);
		return;
	case OBJECT_MOVER:
		DrawMover(p_viewData);
		return;
	case OBJECT_SLINKY:
		DrawSlinky(p_viewData);
		return;
	}
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

// FUNCTION: LEMBALL 0x0043fce0
void C2D::DrawDemo()
{
	// GLOBAL: LEMBALL 0x004a78c4
	static unsigned long g_lastBlink = CurrentMilliTimer();
	// GLOBAL: LEMBALL 0x004a78c8
	// ?$S2@?1??DrawDemo@C2D@@QAEXXZ@4EA
	// GLOBAL: LEMBALL 0x0049efc8
	static int g_visible = 0;
	if (CurrentMilliTimer() - g_lastBlink > DEMO_TEXT_BLINK_INTERVAL_MS) {
		g_visible = !g_visible;
		g_lastBlink = CurrentMilliTimer();
	}
	if (g_visible) {
		CVSPoint& position = m_demoTextRect;
		CVSSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi,
								  position,
								  advance,
								  RES_BORDERS_LORES_CUTFONT,
								  g_demoText,
								  TEXT_ADVANCE_X_POSITIVE,
								  (CRemap*) m_remaps[4]);
	}
}

// FUNCTION: LEMBALL 0x0043fd80
void C2D::DrawTime()
{
	unsigned short baseTime = (unsigned short) m_ai->m_levelTimeRemaining;
	short time = (short) m_ai->m_gameTime;
	time = (short) (time + baseTime);
	if (time < 0) {
		time = 0;
	}
	if (time >= LEVEL_TIME_DISPLAY_LIMIT_SECONDS) {
		if (baseTime >= LEVEL_TIME_DISPLAY_LIMIT_SECONDS) {
			return;
		}
		if (time >= LEVEL_TIME_DISPLAY_LIMIT_SECONDS) {
			time = LEVEL_TIME_DISPLAY_MAX_SECONDS;
		}
	}

	if (time != g_lastDrawnTime) {
		g_lastDrawnTime = time;
		int seconds = time % SECONDS_PER_DISPLAY_MINUTE;
		g_timeText[0] = (char) (time / SECONDS_PER_DISPLAY_MINUTE) + '0';
		g_timeText[1] = ':';
		g_timeText[2] = (char) (seconds / SECONDS_PER_DISPLAY_TEN) + '0';
		g_timeText[3] = (char) (seconds % SECONDS_PER_DISPLAY_TEN) + '0';
		g_timeText[4] = 0;
	}

	CVSPoint& position = m_spriteGroundLookupRectA;
	CVSSize advance;
	advance.m_width = -4;
	advance.m_height = 0;
	m_textManager->DrawString(m_gdi,
							  position,
							  advance,
							  RES_NEWFRONT_FONTS_GAME_SCORETIME,
							  g_timeText,
							  TEXT_ADVANCE_X_POSITIVE,
							  NULL);
}

// FUNCTION: LEMBALL 0x0043fe70
void C2D::DrawPaused()
{
}

// FUNCTION: LEMBALL 0x0043fe80
void C2D::DrawScore()
{
	int targetScore = m_ai->m_score;
	int score = m_score;
	if (m_scoreTimestamp <= g_dwGameTick) {
		if (score != targetScore) {
			if (m_ai->m_gameStatus == GAME_STATUS_RUNNING) {
				score += 10;
				if (score >= targetScore) {
					score = targetScore;
				}
			}
			else {
				score += 100;
				if (score >= targetScore) {
					score = targetScore;
				}
			}
			m_score = score;
		}
		m_scoreTimestamp = g_dwGameTick + 1;
	}
	if (score >= GAME_SCORE_DISPLAY_VALUE_LIMIT) {
		score = GAME_SCORE_MAX_DISPLAY_VALUE;
	}

	CVSSize advance;
	char scoreText[8];
	scoreText[7] = 0;
	int i = 1;
	do {
		scoreText[7 - i] = (char) (score % 10) + '0';
		i++;
		score /= 10;
	} while (i <= 7);

	CVSPoint* position = &m_spriteGroundLookupRectB;
	advance.m_width = -4;
	advance.m_height = 0;
	m_textManager->DrawString(m_gdi,
							  *position,
							  advance,
							  RES_NEWFRONT_FONTS_GAME_SCORETIME,
							  scoreText,
							  TEXT_ADVANCE_X_POSITIVE,
							  NULL);
}

// FUNCTION: LEMBALL 0x0043ff70
void C2D::SortViewData()
{
	int index = 0;
	if (m_viewDataCount > 0) {
		do {
			if (m_redrawPending == 0 && m_viewData[index].m_transientFlags == 0) {
				m_redrawPending = 0;
			}
			else {
				m_redrawPending = 1;
			}

			m_viewData[index].m_sortZKey = CalcZValue_Sprite(index);
			index++;
		} while (index < (int) m_viewDataCount);
	}

	VSQSort(m_viewData, m_viewDataCount, sizeof(CViewData), ViewDataCmp);
}

// FUNCTION: LEMBALL 0x00440000
void C2D::Draw(const CVSRect& p_rect)
{
	if (m_gdi == NULL || m_clipSize.m_x <= 0 || m_clipSize.m_y <= 0) {
		return;
	}
	if (m_lemmingAnims->m_loaded == 0) {
		m_lemmingAnims->Draw();
		return;
	}

	unsigned long groundAnimationFrame = g_dwSimulationTimestamp / 100;
	m_frameCount++;
	m_groundAnimationFrame = (short) groundAnimationFrame;
	unsigned long startTime = timeGetTime();
	m_clipSearchHeight = 0x40;

	CVSRect* displayRect = &m_display->m_rect;
	CVSPoint* displayPosition = displayRect;
	int zoom = m_display->m_zoom;
	CVSPoint cursorPosition;
	cursorPosition.m_y = (short) ((short) (g_pCursor->m_position.m_y - displayPosition->m_y) / zoom);
	cursorPosition.m_x = (short) ((short) (g_pCursor->m_position.m_x - displayPosition->m_x) / zoom);
	m_spriteGroundTranslationPoint.m_x = cursorPosition.m_x;
	m_spriteGroundTranslationPoint.m_y = cursorPosition.m_y;
	ReplaceBackground();

	if (m_clipOffsetX != 0 || m_clipOffsetY != 0) {
		int left = m_clipOffsetX + m_spriteGroundTranslatedPointRect.m_x;
		int height = m_spriteGroundTranslatedPointRect.m_height;
		int top = m_clipOffsetY + m_spriteGroundTranslatedPointRect.m_y;
		int right = m_spriteGroundTranslatedPointRect.m_width + left;
		int bottom = height + top;
		int clipBottom = m_clipOffsetY + m_clipSize.m_y;
		int clipRight = m_clipOffsetX + m_clipSize.m_x;
		if (left < m_clipOffsetX || top < m_clipOffsetY || clipRight <= right || bottom >= clipBottom) {
			CVSRect translatedBounds;
			CVSSize& translatedSize = translatedBounds;
			CVSPoint& translatedPosition = translatedBounds;
			translatedPosition.m_x = (short) left;
			translatedPosition.m_y = (short) top;
			translatedSize.m_width = m_spriteGroundTranslatedPointRect.m_width;
			translatedSize.m_height = m_spriteGroundTranslatedPointRect.m_height;
			m_lineAt9a8.m_bounds.m_width = translatedSize.m_width;
			m_lineAt9a8.m_bounds.m_height = translatedSize.m_height;
			m_lineAt9a8.m_bounds.CVSPoint::operator=(translatedPosition);
			m_lineAt9a8.m_colour = 0;
			m_lineAt9a8.Draw(m_gdi);
		}
	}

	m_viewDataCount = (unsigned short) m_ai->GetData(m_viewData);
	m_pushActive.Draw(m_gdi);
	CVSRect backgroundBounds;

	{
		int primitiveIndex = m_primitiveCount++;
		CClipRect& background = m_clipRects[primitiveIndex];
		if (m_clipConfigured == 0 && m_redrawPending == 0) {
			backgroundBounds.m_width = m_clipSize.m_x;
			backgroundBounds.m_height = m_clipSize.m_y;
			backgroundBounds.m_x = 0;
			backgroundBounds.m_y = 0;
		}
		else {
			backgroundBounds.m_width = m_clipSize.m_x;
			backgroundBounds.m_height = m_clipSize.m_y;
			backgroundBounds.m_x = 0;
			backgroundBounds.m_y = 0;
		}
		static_cast<CVSSize&>(background.m_bounds) = backgroundBounds;
		background.m_bounds.CVSPoint::operator=(backgroundBounds);
		background.m_flags = 0;
		background.Draw(m_gdi);
	}

	if (m_clipConfigured != 0 || m_redrawPending != 0) {
		CVSRect translatedBounds;
		m_clipConfigured = 0;
		translatedBounds.m_width = m_clipSize.m_x;
		translatedBounds.m_height = m_clipSize.m_y;
		translatedBounds.m_x = 0;
		translatedBounds.m_y = 0;
		m_lineAt998.m_bounds = translatedBounds;
		m_lineAt998.m_colour = 0;
		m_lineAt998.Draw(m_gdi);
	}

	DrawObjects();
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		DrawDemo();
	}
	else {
		DrawTime();
		DrawScore();
	}
	if (m_paused != 0) {
		DrawPaused();
	}
	m_popActive.Draw(m_gdi);
	g_pSoundView->SoundEffect(m_viewData, m_viewDataCount, m_originPosition);

	{
		int surfacePrimitiveIndex = m_primitiveCount++;
		CClipRect& surfaceBackground = m_clipRects[surfacePrimitiveIndex];
		CVSRect& windowRect = m_gdi->m_renderTarget->m_windowRect;
		CVSRect translatedBounds(windowRect);
		translatedBounds.m_x = 0;
		translatedBounds.m_y = 0;
		memcpy(&surfaceBackground.m_bounds.m_width, &translatedBounds.m_width, sizeof(short));
		memcpy(&surfaceBackground.m_bounds.m_height, &translatedBounds.m_height, sizeof(short));
		memcpy(&surfaceBackground.m_bounds.m_x, &translatedBounds.m_x, sizeof(short));
		memcpy(&surfaceBackground.m_bounds.m_y, &translatedBounds.m_y, sizeof(short));
		surfaceBackground.m_flags = 0;
		surfaceBackground.Draw(m_gdi);
	}

	m_frameTime += timeGetTime() - startTime;
}

#include "Visos/Graphics/Surfaces/CChangeList.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/CVSSize.h"
#include "Visos/Queues/Message.h"
#include "Visos/Input/CBaseCursor.h"
#include "Visos/Graphics/Palettes/CBaseRemap.h"
#include "Visos/Graphics/Primitives/CClipRect.h"
#include "Visos/Graphics/Primitives/CCopyToBackBuff.h"
#include "Visos/Graphics/Primitives/CDrawingMark.h"
#include "Visos/Controls/CHotAreaHandler.h"
#include "Visos/Graphics/Primitives/CPopActive.h"
#include "Visos/Graphics/Primitives/CPushActive.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"

class CBaseQueueHandler;
class CRemap;

// FUNCTION: LEMBALL 0x00440400
void C2D::ReplaceBackground()
{
	CChangeList* changeList = m_gdi->m_renderTarget->GetChangeList();
	m_drawingMark.Draw(m_gdi);
	changeList->Reset();
}

// FUNCTION: LEMBALL 0x00440430
void C2D::ResetPrimitives()
{
	m_lemmingAnims->ResetPrimitives();
	m_textManager->ResetPrimitives();
	m_unk0xc90 = 0;
	m_primitiveCount = 0;
}

// FUNCTION: LEMBALL 0x00440490
void C2D::DrawZBuff_Anim(int p_index, unsigned short p_z)
{
	AnimSpecialEntry* animation = m_zBufferAnimations + p_index;
	int gameX = (unsigned short) animation->m_x << GROUND_BLOCK_PIXEL_SHIFT;
	int gameY = (unsigned short) animation->m_y << GROUND_BLOCK_PIXEL_SHIFT;
	CGround* ground = animation->m_groundEntry;
	int height = ground->m_height;
	unsigned short collision = ground->m_collision;
	unsigned short frame = ground->m_objectData;
	int cliff = (short) ground->m_cliff;
	eObjectType groundType = ground->m_objectType;
	int screenX;
	int screenY;

	m_map->GameToScreen(gameX, gameY, screenX, screenY);
	screenX -= m_viewOriginX;
	screenY -= m_viewOriginY;
	m_lemmingAnims->m_primitiveSequence = p_z;
	if ((collision & GROUND_COLLISION_SPECIAL_RENDER) != 0 && height > 0) {
		DrawCliff(screenX, screenY, height, cliff);
	}
	DrawGround(screenX, screenY - height, groundType, frame);
}

// FUNCTION: LEMBALL 0x00440560
void C2D::DrawObjectsZBuff()
{
	CVSRect backgroundBounds;
	backgroundBounds.m_width = m_clipSize.m_x;
	backgroundBounds.m_height = m_clipSize.m_y;
	backgroundBounds.m_y = 0;
	backgroundBounds.m_x = 0;
	int primitiveIndex = m_primitiveCount++;
	CClipRect& background = m_clipRects[primitiveIndex];
	memcpy(&background.m_bounds.m_width, &backgroundBounds.m_width, sizeof(short));
	memcpy(&background.m_bounds.m_height, &backgroundBounds.m_height, sizeof(short));
	memcpy(&background.m_bounds.m_x, &backgroundBounds.m_x, sizeof(short));
	memcpy(&background.m_bounds.m_y, &backgroundBounds.m_y, sizeof(short));
	background.m_flags = 0;
	background.Draw(m_gdi);

	{
		C3DVector position;
		int viewIndex = 0;
		for (;;) {
			if ((int) m_viewDataCount <= viewIndex) {
				break;
			}
			CViewData* viewData = m_viewData + viewIndex;
			viewData->m_gameX = (short) viewData->m_positionX;
			viewData->m_gameY = (short) viewData->m_positionY;
			memcpy(&position, &m_viewData[viewIndex].m_positionX, sizeof(position));
			m_map->GameToScreen(position.m_xFixed, position.m_yFixed);
			position.m_yFixed -= position.m_zFixed;
			position.m_xFixed -= m_viewOriginX;
			position.m_yFixed -= m_viewOriginY;
			memcpy(&m_viewData[viewIndex].m_positionX, &position, sizeof(position));
			viewIndex++;
		}
	}
	SortViewData();
	m_lemmingAnims->m_drawFlags = ZRLE_DRAW_FLAG_QUICK_Z_BUFFER;

	CAnimSpecial* animations = m_ai->m_animSpecial;
	m_zBufferAnimationCount = animations->m_entryCount;
	m_zBufferAnimations = animations->m_entries;
	int spriteIndex = 0;
	int animationIndex = 0;
	bool spriteZValid = false;
	bool animationZValid = false;
	unsigned short spriteZ;
	unsigned short animationZ;

	while (spriteIndex < (int) m_viewDataCount && animationIndex < m_zBufferAnimationCount) {
		if (!spriteZValid) {
			spriteZ = CalcZValue_Sprite(spriteIndex);
			spriteZValid = true;
		}
		if (!animationZValid) {
			AnimSpecialEntry* animation = m_zBufferAnimations + animationIndex;
			animationZ = animation->m_groundEntry->m_height + animation->m_sortKey;
			animationZValid = true;
		}
		if (animationZ < spriteZ) {
			DrawZBuff_Anim(animationIndex, animationZ);
			animationIndex++;
			animationZValid = false;
		}
		else {
			DrawZBuff_Sprite(spriteIndex, spriteZ);
			spriteIndex++;
			spriteZValid = false;
		}
	}

	while (spriteIndex < (int) m_viewDataCount) {
		unsigned short z = CalcZValue_Sprite(spriteIndex);
		DrawZBuff_Sprite(spriteIndex, z);
		spriteIndex++;
	}

	while (animationIndex < m_zBufferAnimationCount) {
		AnimSpecialEntry* animation = m_zBufferAnimations + animationIndex;
		unsigned short z = animation->m_groundEntry->m_height + animation->m_sortKey;
		DrawZBuff_Anim(animationIndex, z);
		animationIndex++;
	}
}

// FUNCTION: LEMBALL 0x004407e0
unsigned short C2D::CalcZValue_Sprite(int p_index)
{
	eObjectType objectType = m_viewData[p_index].m_objectType;
	if (objectType == OBJECT_TRAP_DOOR) {
		return SPRITE_SORT_CODE_TOPMOST;
	}

	CViewData* viewData = m_viewData + p_index;
	unsigned short z = (unsigned short) viewData->m_positionZ;
	return CalcGroundCode(objectType, (unsigned short) viewData->m_gameX, (unsigned short) viewData->m_gameY, z) + z +
		   1;
}

// FUNCTION: LEMBALL 0x00440840
unsigned short C2D::CalcGroundCode(eObjectType p_objectType, int p_x, int p_y, unsigned short p_z)
{
	bool baseCodeOnly = false;
	switch (p_objectType) {
	case OBJECT_TRAP_DOOR:
		return SPRITE_SORT_CODE_SPECIAL_OBJECT;
	case OBJECT_DOOR_1:
	case OBJECT_DOOR_2:
		p_x += GROUND_BLOCK_PIXEL_SIZE;
		p_y += GROUND_BLOCK_PIXEL_SIZE;
		baseCodeOnly = true;
		break;
	case OBJECT_HAND:
		return SPRITE_SORT_CODE_SPECIAL_OBJECT;
	case OBJECT_TRAMPOLINE:
		baseCodeOnly = true;
		break;
	case OBJECT_MOVER:
		p_x = (p_x - GROUND_BLOCK_PIXEL_HALF_SIZE) & ~GROUND_BLOCK_PIXEL_MASK;
		p_y = (p_y - GROUND_BLOCK_PIXEL_HALF_SIZE) & ~GROUND_BLOCK_PIXEL_MASK;
		break;
	}

	unsigned short tileX = (unsigned short) (p_x / GROUND_BLOCK_PIXEL_SIZE);
	unsigned short tileY = (unsigned short) (p_y / GROUND_BLOCK_PIXEL_SIZE);
	unsigned short code = (unsigned short) ((tileY + tileX) * 0x40 + 4);
	if (baseCodeOnly) {
		return code;
	}

	p_z += 2;

	unsigned short southZValue;
	unsigned short& southZ = southZValue;
	int southTileX = p_x >> GROUND_BLOCK_PIXEL_SHIFT;
	int southTileY = (p_y + GROUND_BLOCK_PIXEL_SIZE) >> GROUND_BLOCK_PIXEL_SHIFT;
	{
		CMap* map = m_map;
		int width;
		if (p_x < 0 || p_y + GROUND_BLOCK_PIXEL_SIZE < 0 || (width = map->m_ground.m_width, width <= southTileX) ||
			map->m_ground.m_height <= southTileY) {
			southZ = 0;
		}
		else {
			int sampleY;
			int& cellY = sampleY;
			int cellX = p_x;
			cellY = p_y;
			cellY &= 0xf;
			width *= southTileY;
			cellX &= 0xf;
			southZ = (map->m_ground.m_ground + width + southTileX)->GetZ(cellX, cellY);
		}
	}

	unsigned short eastZ;
	int eastTileX = (p_x + GROUND_BLOCK_PIXEL_SIZE) >> GROUND_BLOCK_PIXEL_SHIFT;
	int eastTileY = p_y >> GROUND_BLOCK_PIXEL_SHIFT;
	{
		CMap* map = m_map;
		if (p_x + GROUND_BLOCK_PIXEL_SIZE < 0 || p_y < 0 || map->m_ground.m_width <= eastTileX ||
			map->m_ground.m_height <= eastTileY) {
			eastZ = 0;
		}
		else {
			int sampleY;
			int& cellY = sampleY;
			int cellX = p_x;
			cellY = p_y;
			cellY &= 0xf;
			eastTileY *= map->m_ground.m_width;
			cellX &= 0xf;
			eastZ = (map->m_ground.m_ground + eastTileY + eastTileX)->GetZ(cellX, cellY);
		}
	}

	unsigned short southeastZ;
	{
		CMap* map = m_map;
		int width;
		if (p_x + GROUND_BLOCK_PIXEL_SIZE < 0 || p_y + GROUND_BLOCK_PIXEL_SIZE < 0 ||
			(width = map->m_ground.m_width, width <= eastTileX) || map->m_ground.m_height <= southTileY) {
			southeastZ = 0;
		}
		else {
			p_x &= 0xf;
			p_y &= 0xf;
			width *= southTileY;
			southeastZ = (map->m_ground.m_ground + width + eastTileX)->GetZ(p_x, p_y);
		}
	}

	{
		const int& southWithinZ = p_z >= southZ;
		bool eastWithinZ = p_z >= eastZ;
		bool southeastWithinZ = p_z >= southeastZ;
		int threshold = (int) p_z - 0x18;
		bool southSolid;
		if ((int) southZ < threshold) {
		southClear:
			southSolid = false;
		}
		else {
			int collisionY = tileY + 1;
			unsigned short collision;
			if (collisionY < 0) {
				collision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			else {
				CMap* map = m_map;
				int collisionX = tileX;
				int width = map->m_ground.m_width;
				if (width <= collisionX || map->m_ground.m_height <= collisionY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = map->m_ground.m_ground[width * collisionY + collisionX].m_collision;
				}
			}
			if ((collision & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				goto southClear;
			}
			southSolid = true;
		}

		bool eastSolid;
		if ((int) eastZ < threshold) {
		eastClear:
			eastSolid = false;
		}
		else {
			int collisionX = tileX;
			unsigned short collision;
			if (collisionX + 1 < 0) {
				collision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			else {
				CMap* map = m_map;
				int width = map->m_ground.m_width;
				if (width <= collisionX + 1 || map->m_ground.m_height <= tileY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = map->m_ground.m_ground[width * tileY + collisionX + 1].m_collision;
				}
			}
			if ((collision & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				goto eastClear;
			}
			eastSolid = true;
		}

		bool southeastSolid;
		if ((int) southeastZ < threshold) {
		southeastClear:
			southeastSolid = false;
		}
		else {
			int collisionX = tileX;
			int collisionY = tileY + 1;
			unsigned short collision;
			if (collisionX + 1 < 0 || collisionY < 0) {
				collision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			else {
				CMap* map = m_map;
				int width = map->m_ground.m_width;
				if (width <= collisionX + 1 || map->m_ground.m_height <= collisionY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = map->m_ground.m_ground[width * collisionY + collisionX + 1].m_collision;
				}
			}
			if ((collision & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				goto southeastClear;
			}
			southeastSolid = true;
		}
		if (southWithinZ && eastWithinZ && southeastWithinZ && !southeastSolid && !eastSolid && !southSolid) {
			code += 0x80;
		}
		else if (southWithinZ || eastWithinZ) {
			code += 0x40;
		}
		return code;
	}
}

// FUNCTION: LEMBALL 0x00440c00
void C2D::InitSpriteGroundLU()
{
}

// GLOBAL: LEMBALL 0x0049ee70
char* g_demoText = "Demo";
