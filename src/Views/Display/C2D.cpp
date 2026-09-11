#include "C2D.h"

#include "../../AI/Base/ObjectActions.h"
#include "../../AI/Navigation/Ai.h"
#include "../../AI/Objects/AnimSpecial.h"
#include "../../AI/Objects/AnimSpecialEntry.h"
#include "../../AI/Objects/PlayerLemming.h"
#include "../../AI/Objects/ViewData.h"
#include "../../Control/Game/Demo.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameMain.h"
#include "../../Control/Game/GameTime.h"
#include "../../Control/Level/LevelLoader.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Frontend/Resources/FrontendResourceLoader.h"
#include "../../Map/Base/Map.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/ObjSq.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResFont.h"
#include "../../Visos/Resources/ResPalette.h"
#include "../Animation/LemmingAnimsManager.h"
#include "../Input/PadToButton.h"
#include "../Panel/Panel.h"
#include "../Pause/PauseWindow.h"
#include "../Sound/SoundView.h"
#include "../Target/TargetObjectClipGrid.h"
#include "../Target/TargetSpriteGroundLookup.h"
#include "Main2DDisplay.h"

#include <new.h>
#include <string.h>

extern int g_anC2DRemapSourceIndices[17];
extern int g_anC2DRemapTargetIndices[4][17];
extern unsigned char g_abC2DType2Remap[5];

// 68K 0x10b06778 __ct__3C2DFP14CMain2DDisplayP3CAIP4CGDIP4CMapRC7CVSRect
// FUNCTION: LEMBALL 0x004358d0
C2D::C2D(Main2DDisplay* p_arg0, Ai* p_arg1, Gdi* p_arg2, Map* p_arg3, const VsRect& p_arg4) : HotAreaHandler(p_arg4)
{
	void* storage;
	BaseQueueHandler* queueHandler;
	TargetObjectClipGrid* objectClipGrid;
	unsigned int cellCount;
	int groundWidth;
	int groundHeight;

	m_frameCount = 0;
	m_frameTime = 0;
	m_paused = 0;
	m_pauser = 0;
	m_connectionTimeoutActive = 0;
	m_cursorBlinkPhase = 0;
	m_pad0x920 = 0;
	m_mouseButtonDown = 0;
	m_zBufferEnabled = 1;
	InitSpriteGroundLu();
	m_groundHitMode = 0;
	m_pauseWindow = 0;
	m_optionSelection = 0;
	m_cursorState = 0;
	m_returnState = 2;
	m_cursorTimestamp = g_dwSimulationTimestamp;
	if (g_nTestAllLevels != 0) {
		m_testLevel = 0;
	}
	m_ai = p_arg1;
	m_gdi = p_arg2;
	m_display = p_arg0;
	m_map = p_arg3;
	m_viewOriginX = 0;
	m_viewOriginY = 0;
	m_unk0x90c = 0;
	p_arg3->m_orientation = 0;
	m_groupCount = 0;
	m_quitRequested = 0;
	m_groupSelectionCount = 0;
	m_groupingActive = 0;
	m_externalEnabled = 1;
	m_lemmingManager = m_ai->m_aiQueue;

	m_display->m_hotAreaList->AddToList(this);

	storage = operator new(0xb4);
	if (storage != 0) {
		m_lemmingAnims = new (storage) LemmingAnimsManager(m_gdi, m_display, m_ai);
	}
	else {
		m_lemmingAnims = 0;
	}

	storage = operator new(0x24);
	if (storage != 0) {
		m_textManager = new (storage) TextManager(0x2b6, 2, 2, 10);
	}
	else {
		m_textManager = 0;
	}

	ResetPrimitives();
	RegisterRemaps();
	m_cursorState = 0;
	m_clipConfigured = 1;
	m_viewDataCount = 0;

	m_viewData = new ViewData[200];

	m_zBuffer = (unsigned char*) operator new(0x800);
	queueHandler = this;
	g_pMasterInputQueue->Attach(queueHandler, 0);
	ClockEditMode(0);

	storage = operator new(0x1c);
	if (storage != 0) {
		m_padToButton = new (storage) PadToButton(3);
	}
	else {
		m_padToButton = 0;
	}

	m_unk0xc90 = 0;
	m_primitiveCount = 0;
	SetUpRemapPalettes();
	m_textManager->LoadFont(0x115);
	if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
		m_textManager->LoadFont(0xf8);
	}

	m_spriteGroundLookup = 0;
	objectClipGrid = (TargetObjectClipGrid*) operator new(0x1a4);
	if (objectClipGrid != 0) {
		groundHeight = m_map->m_ground.m_height << 4;
		groundWidth = m_map->m_ground.m_width << 4;
		objectClipGrid->m_touchedCount = 0;
		objectClipGrid->m_cells = 0;
		objectClipGrid->m_cellWidth = 0x10;
		objectClipGrid->m_cellHeight = 0x10;
		if (objectClipGrid->m_cells != 0) {
			operator delete(objectClipGrid->m_cells);
			objectClipGrid->m_cells = 0;
		}
		objectClipGrid->m_width =
			(short) ((groundWidth + objectClipGrid->m_cellWidth - 1) / objectClipGrid->m_cellWidth);
		objectClipGrid->m_height =
			(short) ((groundHeight + objectClipGrid->m_cellHeight - 1) / objectClipGrid->m_cellHeight);
		cellCount = (unsigned int) (int) objectClipGrid->m_width * (unsigned int) (int) objectClipGrid->m_height;
		objectClipGrid->m_cellCount = cellCount;
		objectClipGrid->m_cells = new ObjSq[cellCount];
		m_objectClipGrid = objectClipGrid;
	}
	else {
		m_objectClipGrid = 0;
	}

	m_panel = 0;
	m_pad0x8c8 = 0;
	m_pad0x8cc = 0;
	m_groundWidth = (unsigned short) m_map->m_ground.m_width;
	m_groundHeight = (unsigned short) m_map->m_ground.m_height;
	m_viewSize.m_x = p_arg4.m_width;
	m_viewSize.m_y = p_arg4.m_height;
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

// 68K 0x10b06e3e __dt__3C2DFv
// STUB: LEMBALL 0x00436050
C2D::~C2D()
{
}

// 68K 0x10b070a2 ShutDown__3C2DFv
// FUNCTION: LEMBALL 0x00436190
void C2D::ShutDown()
{
	TextManager* textManager;
	TargetObjectClipGrid* objectClipGrid;
	TargetSpriteGroundLookup* spriteGroundLookup;
	LemmingAnimsManager* lemmingAnims;
	unsigned long started;
	unsigned long now;

	m_gdi->m_renderTarget->EnableZBuff(0);
	textManager = m_textManager;
	m_textManager->UnLoadFont(0x115);
	if (textManager != 0) {
		textManager->~TextManager();
		operator delete(textManager);
	}
	if (m_panel != 0) {
		delete m_panel;
		m_panel = 0;
	}
	if (m_pauseWindow != 0) {
		delete m_pauseWindow;
		m_pauseWindow = 0;
	}
	KillRemapPalettes();
	objectClipGrid = m_objectClipGrid;
	if (objectClipGrid != 0) {
		operator delete(objectClipGrid->m_cells);
		operator delete(objectClipGrid);
	}
	spriteGroundLookup = m_spriteGroundLookup;
	if (spriteGroundLookup != 0) {
		operator delete(spriteGroundLookup->m_maskA);
		operator delete(spriteGroundLookup->m_maskB);
		operator delete(spriteGroundLookup);
	}
	if (m_padToButton != 0) {
		delete m_padToButton;
	}
	g_pMasterInputQueue->Detach(this, 0);
	if (m_display->m_lifecycleRefs == 1) {
		m_display->m_hotAreaList->RemoveFromList(this);
	}
	operator delete(m_zBuffer);
	operator delete(m_viewData);
	CursorChangeType(0, 0);
	lemmingAnims = m_lemmingAnims;
	if (lemmingAnims != 0) {
		lemmingAnims->~LemmingAnimsManager();
		operator delete(lemmingAnims);
	}
	UnRegisterRemaps();
	if (m_ai->m_networkMode != 0 && m_returnState == 2) {
		if (g_pNetworkManager != 0) {
			g_pNetworkManager->Stop();
		}
		if (g_pBaseNetwork != 0) {
			started = CurrentMilliTimer();
			do {
				now = CurrentMilliTimer();
				if (now - started >= 2000) {
					break;
				}
			} while (g_pBaseNetwork->m_queueTransitionPending != 0);
		}
		if (g_pNetworkManager != 0) {
			delete g_pNetworkManager;
			g_pNetworkManager = 0;
		}
	}
	m_display->m_gdi->m_renderTarget->SetWorldWidth(0);
	VsRect rect;
	rect.m_x = 0;
	rect.m_y = 0;
	rect.m_width = 0;
	rect.m_height = 0;
	m_display->SetInnerWindow(rect);
}

// 68K 0x10b0735a RegisterRemaps__3C2DFv
// FUNCTION: LEMBALL 0x004363c0
void C2D::RegisterRemaps()
{
	ResPalette* palette;
	int paletteSize;
	int* targets;
	int remapIndex;

	targets = g_anC2DRemapTargetIndices[0];
	palette = ResPalette::Load(RES_GAME_GAMEPALETTE);
	paletteSize = (int) palette->m_paletteState;
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
		} while (sources < g_anC2DRemapTargetIndices[0]);

		BaseRemap* remap =
			g_pBasePalManager->RegisterRemap(RES_GAME_GAMEPALETTE, m_remapTables[remapIndex], (ePaletteTypes) 0);
		m_remaps[remapIndex] = remap;
		remapIndex = remapIndex + 1;
	} while (remapIndex < 4);

	m_remaps[4] = g_pBasePalManager->RegisterRemap(RES_GAME_GAMEPALETTE, g_abC2DType2Remap, (ePaletteTypes) 2);
	palette->UnLoad();
}

// 68K 0x10b07444 UnRegisterRemaps__3C2DFv
// FUNCTION: LEMBALL 0x00436480
void C2D::UnRegisterRemaps()
{
	int i;
	for (i = 0; i < 5; i++) {
		g_pBasePalManager->UnRegisterRemap(m_remaps[i]);
	}
}

#include "../../Visos/Graphics/Cursor.h"

// 68K 0x10b07492 CursorChangeType__3C2DF18eCursorDisplayTypei
// FUNCTION: LEMBALL 0x004364b0
void C2D::CursorChangeType(int p_cursorType, int p_value)
{
	::CursorChangeType((eCursorDisplayType) p_cursorType, p_value);
}

// 68K 0x10b074d8 OnLoaded__3C2DFv
// FUNCTION: LEMBALL 0x004364d0
void C2D::OnLoaded()
{
	int zoom;
	unsigned int zoomDivisor;
	unsigned int oldZoom;
	Panel* panel;

	CursorChangeType(1, 0);
	if (g_nCompactPrimaryContextLayout != 0 || g_nEditLevelMode != 0 || g_nZoomEnabled != 0) {
		zoom = 1;
	}
	else {
		zoom = 2;
	}

	oldZoom = m_zoom;
	m_zoom = (unsigned short) zoom;
	zoomDivisor = (unsigned short) zoom;
	VsRect* displayRect = &m_display->m_rect;
	m_viewSize.m_x = displayRect->m_width;
	m_viewSize.m_y = displayRect->m_height;
	m_viewSize.m_x = (short) ((int) m_viewSize.m_x / (int) zoomDivisor);
	m_viewSize.m_y = (short) ((int) m_viewSize.m_y / (int) zoomDivisor);
	SetClipSize();

	if (m_viewSize.m_x != m_clipSize.m_x || m_viewSize.m_y != m_clipSize.m_y) {
		VsRect innerRect((short) m_clipOffsetX, (short) m_clipOffsetY, m_clipSize.m_x, m_clipSize.m_y);
		m_display->SetInnerWindow(innerRect);
	}

	panel = (Panel*) operator new(0x58);
	if (panel != 0) {
		m_panel = new (panel) Panel(this);
	}
	else {
		m_panel = 0;
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
	if (g_pDemo != 0) {
		g_pDemo->m_window = m_display;
	}
	m_display->Clear(0);
	m_ai->Start();
}

// 68K 0x10b0780e DoButtons__3C2DFv
// FUNCTION: LEMBALL 0x00436690
void C2D::DoButtons()
{
}

// 68K 0x10b0782a OnZoom__3C2DFRC7CVSRect
// FUNCTION: LEMBALL 0x004366a0
void C2D::OnZoom(const VsRect& p_rect)
{
}

// 68K 0x10b0784c OnSize__3C2DFRC7CVSRect
// FUNCTION: LEMBALL 0x004366b0
void C2D::OnSize(const VsRect& p_rect)
{
	unsigned int zoomDivisor;

	if (m_display->GetSizeStatus() == 0) {
		if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
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
		if (m_panel != 0) {
			m_panel->OnSize();
		}
	}
}

// 68K 0x10b07924 SetUpRemapPalettes__3C2DFv
// FUNCTION: LEMBALL 0x00436760
void C2D::SetUpRemapPalettes()
{
	unsigned char* mapping = (unsigned char*) operator new(0x100);
	int value;
	int i = 0;
	do {
		switch (i) {
		case 0x37:
			value = 0x52;
			break;
		case 0x5c:
		case 0x71:
		case 0x75:
			value = 0x74;
			break;
		case 0x80:
			value = 0x8c;
			break;
		default:
			value = i;
			break;
		}
		mapping[i] = value;
		i++;
	} while (i < 0x100);
	m_paletteRemap = g_pBasePalManager->RegisterRemap(RES_GAME_GAMEPALETTE, mapping, (ePaletteTypes) 0);
}

// 68K 0x10b079c8 KillRemapPalettes__3C2DFv
// FUNCTION: LEMBALL 0x00436830
void C2D::KillRemapPalettes()
{
	g_pBasePalManager->UnRegisterRemap(m_paletteRemap);
}

#include "../../Control/Game/GameStatus.h"

// 68K 0x10b07a00 Restart__3C2DFv
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

// 68K 0x10b07a86 CheckValidFormGroup__3C2DFv
// STUB: LEMBALL 0x004369b0
void C2D::CheckValidFormGroup()
{
}

// 68K 0x10b07b18 Process__3C2DFv
// STUB: LEMBALL 0x00436a10
void C2D::Process()
{
	SetMouseShape();
}

// 68K 0x10b07f46 FindGameObject__3C2DFRC8CVSPointRiUc
// STUB: LEMBALL 0x00436e40
bool C2D::FindGameObject(const VsPoint& p_point, int& p_index, unsigned char p_preferLemming)
{
	return 0;
}

// 68K 0x10b0827e AddObjectToGroup__3C2DFiUc
// FUNCTION: LEMBALL 0x00437130
void C2D::AddObjectToGroup(int p_objectNo, int p_markSelection)
{
	m_groupObjectIds[m_groupCount] = (unsigned short) p_objectNo;
	m_groupCount++;
	if (p_markSelection != 0) {
		m_groupSelectionCount = m_groupCount;
	}
}

// 68K 0x10b082cc FormGroup__3C2DFv
// STUB: LEMBALL 0x00437170
void C2D::FormGroup()
{
}

// 68K 0x10b08354 MoveGroup__3C2DFRC8CVSPoint
// STUB: LEMBALL 0x004371e0
void C2D::MoveGroup(const VsPoint& p_point)
{
}

// 68K 0x10b083d6 CancelMoves__3C2DFv
// FUNCTION: LEMBALL 0x00437250
void C2D::CancelMoves()
{
	Message msg;
	msg.type = 3;
	memset(&msg.time, 0, sizeof(msg.time) + sizeof(msg.code) + sizeof(msg.payload) + sizeof(msg.source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = 0;
	g_pSoundView->m_pendingEffect = (eSoundEffect) 0x25;
}

// 68K 0x10b0843a NextGroup__3C2DFv
// FUNCTION: LEMBALL 0x004372a0
void C2D::NextGroup()
{
	Message msg;
	msg.type = 7;
	memset(&msg.time, 0, sizeof(msg.time) + sizeof(msg.code) + sizeof(msg.payload) + sizeof(msg.source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = 0;
	g_pSoundView->m_pendingEffect = (eSoundEffect) 0x1b;
}

// 68K 0x10b0849c PrevGroup__3C2DFv
// FUNCTION: LEMBALL 0x004372f0
void C2D::PrevGroup()
{
	Message msg;
	msg.type = 6;
	memset(&msg.time, 0, sizeof(msg.time) + sizeof(msg.code) + sizeof(msg.payload) + sizeof(msg.source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = 0;
	g_pSoundView->m_pendingEffect = (eSoundEffect) 0x1b;
}

// 68K 0x10b084fe SelectLemming__3C2DFi
// FUNCTION: LEMBALL 0x00437340
void C2D::SelectLemming(int p_playerIndex)
{
	Message msg;
	msg.type = 8;
	msg.time = 0;
	msg.code = m_ai->m_networkLemmings[p_playerIndex]->m_objectId;
	memset(&msg.payload, 0, sizeof(msg.payload) + sizeof(msg.source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = 0;
	g_pSoundView->m_pendingEffect = (eSoundEffect) 3;
}

// 68K 0x10b08588 SelectObject__3C2DFi
// FUNCTION: LEMBALL 0x004373b0
void C2D::SelectObject(int p_viewIndex)
{
	Message msg;
	msg.type = 8;
	msg.time = 0;
	msg.code = m_viewData[p_viewIndex].m_objectId;
	memset(&msg.payload, 0, sizeof(msg.payload) + sizeof(msg.source));
	m_lemmingManager->Post(msg);
	m_groupCount = 0;
	m_groupingActive = 0;
	g_pSoundView->m_pendingEffect = (eSoundEffect) 3;
}

// 68K 0x10b0860e InGroupByObjectNo__3C2DFi
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

// 68K 0x10b08662 RemoveFromGroupByObjectNo__3C2DFi
// FUNCTION: LEMBALL 0x00437460
void C2D::RemoveFromGroupByObjectNo(int p_objectNo)
{
	int objectNo = p_objectNo;
	unsigned short id;
	unsigned short* write;
	unsigned int i;
	unsigned short* read;

	i = 0;
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
		m_groupingActive = 0;
	}
}

// 68K 0x10b086f8 IsInGrouping__3C2DFP11CGameObject
// FUNCTION: LEMBALL 0x004374e0
bool C2D::IsInGrouping(GameObject* p_object)
{
	int i;
	for (i = 0; i < m_groupCount; i++) {
		if (p_object->m_objectId == m_groupObjectIds[i]) {
			return true;
		}
	}
	return false;
}

// 68K 0x10b0875a NoStateLeftClick__3C2DFRC8CVSPointRC8CVSPointUcUc
// STUB: LEMBALL 0x00437520
void C2D::NoStateLeftClick(const VsPoint& p_screenPoint,
						   const VsPoint& p_gamePoint,
						   unsigned char p_commitMoves,
						   unsigned char p_alternate)
{
}

// 68K 0x10b088fa GroupingLeftClick__3C2DFRC8CVSPointRC8CVSPointUc
// STUB: LEMBALL 0x004376b0
void C2D::GroupingLeftClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint, unsigned char p_alternate)
{
}

// 68K 0x10b08aae LeftClick__3C2DFRC8CVSPointRC8CVSPointUcUc
// STUB: LEMBALL 0x00437840
void C2D::LeftClick(const VsPoint& p_screenPoint,
					const VsPoint& p_gamePoint,
					unsigned char p_commitMoves,
					unsigned char p_alternate)
{
}

// 68K 0x10b08b30 NoStateRightClick__3C2DFRC8CVSPointRC8CVSPoint
// STUB: LEMBALL 0x00437890
void C2D::NoStateRightClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint)
{
}

// 68K 0x10b08c02 RightClick__3C2DFRC8CVSPointRC8CVSPoint
// STUB: LEMBALL 0x00437930
void C2D::RightClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint)
{
}

// 68K 0x10b08c70 ScreenToGame__3C2DFiiRiRi
// FUNCTION: LEMBALL 0x00437970
bool C2D::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
{
	Map* initialMap = m_map;
	int maxGameX = initialMap->m_ground.m_width * 0x10 - 1;
	int maxGameY = initialMap->m_ground.m_height * 0x10 - 1;
	int searchY = p_screenY + 0x50;
	if (searchY >= p_screenY) {
		int searchMinX = p_screenX - 0x20;
		int searchMaxX = p_screenX + 0x20;
		do {
			int searchX = searchMinX;
			if (searchMaxX >= searchX) {
				do {
					int gameX;
					int gameY;
					m_map->ScreenToGame(searchX, searchY, gameX, gameY);
					if (gameX >= 0 && maxGameX >= gameX && gameY >= 0 && maxGameY >= gameY) {
						gameX /= 0x10;
						gameY /= 0x10;

						int groundScreenX;
						int groundScreenY;
						m_map->GameToScreen(gameX << 4, gameY << 4, groundScreenX, groundScreenY);
						groundScreenY -= m_map->m_ground.m_ground[m_map->m_ground.m_width * gameY + gameX].m_height;

						int left = groundScreenX - 0x10;
						int right = groundScreenX + 0xf;
						int top = groundScreenY - 0x10;
						int bottom = groundScreenY + 0xf;
						if (left <= p_screenX && right >= p_screenX && top <= p_screenY && bottom >= p_screenY) {
							Ground* ground = m_map->m_ground.m_ground + m_map->m_ground.m_width * gameY + gameX;
							int hitX = p_screenX - left;
							int hitY = p_screenY - top;
							if (hitX >= 0 && hitY >= 0 && hitX <= 0x1f && hitY <= 0x1f) {
								unsigned int includeSpecial = m_groundHitMode >= 1;
								if (ground->IsHit(hitX, hitY, (unsigned char) includeSpecial)) {
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

// 68K 0x10b08e94 ProcessMsg__3C2DFP10tagMESSAGE
// STUB: LEMBALL 0x00437b60
int C2D::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b09008 NewPauseWindow__3C2DF20ePauseWindowMessages
// FUNCTION: LEMBALL 0x00437d00
void C2D::NewPauseWindow(int p_message)
{
	m_previousPauseMessage = m_pauseMessage;
	m_pauseMessage = p_message;
	if (m_pauseWindow != 0) {
		BaseQueueHandler& queueHandler = *m_pauseWindow;
		delete &queueHandler;
		m_pauseWindow = 0;
	}
	if (m_pauseMessage != 5) {
		m_pauseWindow = new PauseWindow(this, m_display, (ePauseWindowMessages) m_pauseMessage);
	}
	if (m_pauseMessage == 3) {
		m_pauseSelection = m_optionSelection;
	}
}

// 68K 0x10b090e0 TriggerPause__3C2DFUc
// FUNCTION: LEMBALL 0x00437da0
void C2D::TriggerPause(unsigned char p_paused)
{
	if (p_paused != 0) {
		if (m_ai->m_gameStatus >= 1 && m_ai->m_gameStatus <= 2) {
			m_ai->GameState(1);
		}
	}
	else {
		SetPause(p_paused);
	}
}

// 68K 0x10b0914c SetPause__3C2DFUc
// FUNCTION: LEMBALL 0x00437de0
void C2D::SetPause(unsigned char p_paused)
{
	m_pauser = m_ai->m_isSinglePlayer >= 1;
	if (p_paused != 0) {
		if (m_ai->m_gameStatus < 1 || m_ai->m_gameStatus > 2) {
			return;
		}
	}
	m_paused = p_paused;
	ClockEditMode(p_paused);
	m_ai->m_paused = m_paused;
	if (p_paused != 0) {
		m_ai->GameState(1);
	}
	else {
		m_ai->GameState(2);
		m_pauser = 0;
	}
	if (m_paused != 0) {
		NewPauseWindow(0);
	}
	else {
		NewPauseWindow(5);
	}
}

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

// 68K 0x10b09208 SetMouseShape__3C2DFv
// FUNCTION: LEMBALL 0x00437e90
void C2D::SetMouseShape()
{
	VsPoint screen;
	VsPoint game;
	int objectIndex;
	int zoom;
	short originX;
	short originY;
	unsigned int groundFlag;

	groundFlag = g_nMouseShapeOnGround;
	if (m_paused != 0) {
		g_nMouseShapeOnGround = groundFlag;
		return;
	}
	zoom = (int) m_display->m_zoom;
	originX = m_display->m_rect.m_x;
	originY = m_display->m_rect.m_y;
	screen.m_x = (short) ((int) (short) (g_pCursor->m_position.m_x - originX) / zoom);
	screen.m_y = (short) ((int) (short) (g_pCursor->m_position.m_y - originY) / zoom);
	if (m_panel->MouseInPanel(screen) != 0) {
		m_cursorState = 3;
		return;
	}
	game.m_x = (short) (m_viewOriginX + m_cursorGamePoint.m_x);
	game.m_y = (short) (m_cursorGamePoint.m_y + (short) m_viewOriginY);
	if (screen.m_x < m_x || (short) (m_width + m_x) <= screen.m_x || screen.m_y < m_y ||
		(short) (m_height + m_y) <= screen.m_y) {
		CursorChangeType(1, 0);
		groundFlag = g_nMouseShapeOnGround;
	}
	else if (FindGameObject(game, objectIndex, 0) != 0) {
		if (m_cursorState != 2) {
			m_cursorState = 2;
			m_cursorTimestamp = g_dwSimulationTimestamp;
			CursorChangeType(1, 4);
			return;
		}
	}
	else {
		g_nMouseShapeOnGround = ScreenToGame((int) game.m_x, (int) game.m_y, g_nMouseShapeGameX, g_nMouseShapeGameY);
		if (g_nMouseShapeOnGround != 0) {
			m_cursorState = 0;
			m_cursorTimestamp = g_dwSimulationTimestamp;
			if (m_mouseButtonDown != 0) {
				CursorChangeType(1, 1);
				return;
			}
			CursorChangeType(1, 0);
			return;
		}
		if (m_cursorState != 1) {
			m_cursorState = 1;
			m_cursorTimestamp = g_dwSimulationTimestamp;
			CursorChangeType(1, 2);
			return;
		}
		groundFlag = 0;
		if (m_cursorTimestamp + 100 < g_dwSimulationTimestamp) {
			m_cursorBlinkPhase = (unsigned short) (m_cursorBlinkPhase ^ 1);
			m_cursorTimestamp = g_dwSimulationTimestamp;
			CursorChangeType(1, m_cursorBlinkPhase + 2);
			return;
		}
	}
	g_nMouseShapeOnGround = groundFlag;
}

// 68K 0x10b09436 SendCursorMsg__3C2DFv
// FUNCTION: LEMBALL 0x004380c0
void C2D::SendCursorMsg()
{
	Message message;
	VsPoint screenPoint;
	int gameX;
	int gameY;

	message.type = 1;
	memset(&message.time,
		   0,
		   sizeof(message.time) + sizeof(message.code) + sizeof(message.payload) + sizeof(message.source));
	screenPoint.m_x = (short) m_viewOriginX + m_cursorGamePoint.m_x;
	screenPoint.m_y = m_cursorGamePoint.m_y + (short) m_viewOriginY;
	if (!ScreenToGame(screenPoint.m_x, screenPoint.m_y, gameX, gameY)) {
		m_map->ScreenToGame(screenPoint.m_x, screenPoint.m_y, gameX, gameY);
	}
	message.code = gameX;
	message.payload = (void*) gameY;
	m_lemmingManager->Post(message);
}

// 68K 0x10b09514 OnInside__3C2DFRC8CVSPoint
// STUB: LEMBALL 0x00438170
void C2D::OnInside(const VsPoint& p_point)
{
}

// 68K 0x10b0958c OnButtonUp__3C2DFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x004381c0
void C2D::OnButtonUp(const VsPoint& p_point, int p_flags)
{
}

// 68K 0x10b0960a OnButtonDown__3C2DFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00438210
void C2D::OnButtonDown(const VsPoint& p_point, int p_flags)
{
}

// 68K 0x10b09786 UseBalloon__3C2DFi
// FUNCTION: LEMBALL 0x00438330
void C2D::UseBalloon(int p_playerIndex)
{
	PlayerLemming** pLemming = &m_ai->m_networkLemmings[p_playerIndex];
	if ((*pLemming)->GetLastBalloon() != 0xffff && (*pLemming)->m_action != 8) {
		(*pLemming)->SetSndEffect((eSoundEffect) 31);
		UseBalloon(*pLemming);
	}
}

// 68K 0x10b09802 UseBalloon__3C2DFP14CPlayerLemming
// FUNCTION: LEMBALL 0x00438380
void C2D::UseBalloon(PlayerLemming* p_lemming)
{
	if (p_lemming->m_action != 8) {
		m_groupCount = 0;
		CursorChangeType((eCursorDisplayType) p_lemming->m_objectId, 0);
		SendCursorMsg();
		p_lemming->RequestBalloon();
	}
}

// 68K 0x10b09876 OnDriverChange__3C2DFv
// STUB: LEMBALL 0x004383c0
void C2D::OnDriverChange()
{
}

// 68K 0x10b09a30 SetClipSize__3C2DFv
// FUNCTION: LEMBALL 0x00438500
void C2D::SetClipSize()
{
	int width;
	int height;
	int count;
	TargetSpriteGroundLookup* lookup;
	ResFont* font;
	VsSize size;
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
	if (g_pDemo != 0) {
		short demoOffsetY = (short) m_clipOffsetY;
		g_pDemo->m_offsetX = (short) m_clipOffsetX;
		g_pDemo->m_offsetY = demoOffsetY;
	}
	lookup = m_spriteGroundLookup;
	if (lookup != 0) {
		width = (m_clipSize.m_x + 0xf) / 0x10;
		height = (m_clipSize.m_y + 0xf) / 0x10;
		if (lookup->m_width != width || lookup->m_height != height) {
			if (lookup->m_maskA != 0) {
				operator delete(lookup->m_maskA);
				lookup->m_maskA = 0;
			}
			if (lookup->m_maskB != 0) {
				operator delete(lookup->m_maskB);
				lookup->m_maskB = 0;
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
	if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
		font = m_textManager->GetFont(0xf8);
		font->GetSize(&size, "Demo", 0x20);
		m_demoTextPosition.m_y = 0;
		m_demoTextPosition.m_x = (short) ((m_clipSize.m_x - size.m_width) / 2);
	}
}

// 68K 0x1011cbe0 QuitYet__3C2DFv
// FUNCTION: LEMBALL 0x00439450
bool C2D::QuitYet()
{
	return m_quitRequested;
}

// 68K 0x1011cc02 GetReturnState__3C2DFv
// FUNCTION: LEMBALL 0x00439460
int C2D::GetReturnState()
{
	return m_returnState;
}

// 68K 0x1011cc2c GetPauser__3C2DFv
// FUNCTION: LEMBALL 0x00439470
bool C2D::GetPauser()
{
	return m_pauser;
}

// 68K 0x10b01120 DrawGround__3C2DFii11eObjectTypeUs
// FUNCTION: LEMBALL 0x0043a880
void C2D::DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame)
{
	// GLOBAL: LEMBALL 0x0049705c
	static const short g_treeGroundOffset[] = {0x20, 0x30};
	// GLOBAL: LEMBALL 0x00497060
	static const short g_groundOffset[] = {0x10, 0x10};

	int frame;

	switch (p_groundType) {
	case TERRAIN_TREE:
		m_lemmingAnims->DrawAnim(p_x - g_treeGroundOffset[0],
								 p_y - g_treeGroundOffset[1],
								 g_anGroundStyleResourceIds[3],
								 p_frame,
								 0,
								 0);
		return;
	case TERRAIN_BLOX_1:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox1ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_2:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox2ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox3ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_4:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox4ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_5:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox5ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_6:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox6ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_7:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox7ResourceId, p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_anGroundStyleResourceIds[0], p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_anGroundStyleResourceIds[4], p_frame, 0, 0);
		return;
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_anGroundStyleResourceIds[5], p_frame, 0, 0);
		return;
	case TERRAIN_ANIM:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_ANIM, p_frame, 0, 0);
		return;
	case TERRAIN_FLAME:
		frame = ((unsigned short) p_x >> 4) + (unsigned short) m_groundAnimationFrame;
		m_lemmingAnims->DrawAnim(p_x - 0x10, p_y - 0x20, RES_GAME_FLAME, frame % 9, 0, 0);
		return;
	case TERRAIN_ELECTRIC:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 RES_GAME_ELECTRIC,
								 (unsigned short) m_groundAnimationFrame & 3,
								 0,
								 0);
		return;
	case TERRAIN_EMBERS:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_EMBERS, p_frame, 0, 0);
		return;
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_CONVEYOR, p_frame, 0, 0);
		return;
	default:
		return;
	}
}

// 68K 0x10b015a6 DrawCliff__3C2DFiiii
// STUB: LEMBALL 0x0043ace0
void C2D::DrawCliff(int p_x, int p_y, int p_height, int p_count)
{
}

// 68K 0x10b0162c DoClipWidth__3C2DFiii
// STUB: LEMBALL 0x0043ad40
void C2D::DoClipWidth(int p_mapX, int p_mapY, int p_count)
{
}

// 68K 0x10b01988 DoClipWidthSearch__3C2DFiii
// STUB: LEMBALL 0x0043b0e0
void C2D::DoClipWidthSearch(int p_mapX, int p_mapY, int p_count)
{
}

// 68K 0x10b01cf0 DrawClipData__3C2DFv
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

// 68K 0x10b01d3a DrawClippedRectangle__3C2DFRC7CVSRect
// STUB: LEMBALL 0x0043b4d0
void C2D::DrawClippedRectangle(const VsRect& p_rect)
{
}

// 68K 0x10b02196 LemmingFly__3C2DFR9CViewDataRi
// FUNCTION: LEMBALL 0x0043bce0
unsigned long C2D::LemmingFly(ViewData& p_viewData, int& p_frame)
{
	// GLOBAL: LEMBALL 0x0049eef8
	static unsigned long flyResources[] = {
		RES_GAME_JUMP_NE,
		RES_GAME_JUMP_NE,
		RES_GAME_JUMP_SE,
		RES_GAME_JUMP_SE,
		RES_GAME_JUMP_SW,
		RES_GAME_JUMP_SW,
		RES_GAME_JUMP_NW,
		RES_GAME_JUMP_NW,
	};

	unsigned int direction = ((unsigned short) p_viewData.m_facingDirection + m_unk0x90c * 2) & 7;
	int frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;

	p_frame = 0;
	if (frameDelta < 0) {
		return flyResources[direction];
	}

	Map* map = m_map;
	int viewX = (unsigned short) p_viewData.m_viewX;
	int viewY = (unsigned short) p_viewData.m_viewY;
	int blockX = viewX >> 4;
	int blockY = viewY >> 4;
	int groundZ;
	if (viewX >= 0 && viewY >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
		groundZ = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(viewX & 0xf, viewY & 0xf);
	}
	else {
		groundZ = 0;
	}

	int frame = frameDelta * 15;
	if (p_viewData.m_positionZ <= groundZ) {
		p_frame = frame / 1000 + 7;
		if (p_frame > 12) {
			p_frame = 12;
		}
	}
	else {
		p_frame = frame / 1000;
		if (p_frame > 6) {
			p_frame = 6;
		}
	}
	return flyResources[direction];
}

// 68K 0x10b022ac DrawLemmingFlyShadow__3C2DFR9CViewData
// STUB: LEMBALL 0x0043bde0
void C2D::DrawLemmingFlyShadow(ViewData& p_viewData)
{
}

// 68K 0x10b023d4 DrawLemmingJump__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043bee0
void C2D::DrawLemmingJump(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b024ea DrawLemmingLanding__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043bfc0
void C2D::DrawLemmingLanding(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b025d2 DrawLemmingFall__3C2DFR9CViewDataUc
// FUNCTION: LEMBALL 0x0043c070
void C2D::DrawLemmingFall(ViewData& p_viewData, unsigned char p_remapped)
{
	DrawLemmingJump(p_viewData, p_remapped);
}

// 68K 0x10b02612 DrawLemmingExternal__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043c090
void C2D::DrawLemmingExternal(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b0274e DrawLemmingOnConveyor__3C2DFR9CViewDataUc
// FUNCTION: LEMBALL 0x0043c1a0
void C2D::DrawLemmingOnConveyor(ViewData& p_viewData, int p_remapped)
{
	int x;
	int y;
	int frame;
	BaseRemap* remap;

	frame = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	frame = (frame * 15 / 1000) % 8;
	if (p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = 0;
	}
	m_lemmingAnims->DrawAnim(x - 15, y - 22, RES_GAME_LEMMING_SPIN, frame, 0, (Remap*) remap);
}

// Shared with DrawLemming; the original source name is unavailable.
// GLOBAL: LEMBALL 0x00496fd8
static const short g_unk0x496fd8[] = {8, 18};

// 68K 0x10b027f2 DrawLemming__3C2DFR9CViewDataiUc
// STUB: LEMBALL 0x0043c200
void C2D::DrawLemming(ViewData& p_viewData, int p_objectNo, unsigned char p_remapped)
{
}

// 68K 0x10b02c40 DrawBullet__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043c610
void C2D::DrawBullet(ViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049ef38
	static unsigned long bulletResources[] = {
		RES_GAME_LEMMINGPELLETNE,
		RES_GAME_LEMMINGPELLETE,
		RES_GAME_LEMMINGPELLETSE,
		RES_GAME_LEMMINGPELLETS,
		RES_GAME_LEMMINGPELLETSW,
		RES_GAME_LEMMINGPELLETW,
		RES_GAME_LEMMINGPELLETNW,
		RES_GAME_LEMMINGPELLETN,
	};
	// GLOBAL: LEMBALL 0x00497070
	static const short bulletOffset[] = {4, 4};

	unsigned int direction;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_unk0x90c * 2) & 7;
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - bulletOffset[0],
							 p_viewData.m_positionY - bulletOffset[1],
							 bulletResources[direction],
							 0,
							 p_viewData.m_animationTime,
							 0);
}

// 68K 0x10b02cce DrawAmmo__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043c660
void C2D::DrawAmmo(ViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x00497064
	static const short ammoOffset[] = {8, 16};
	// GLOBAL: LEMBALL 0x00497068
	static const short pelletOffset[] = {16, 16};

	switch (p_viewData.m_action) {
	case (eAction) 0x18:
	case (eAction) 0x19:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - ammoOffset[0],
								 p_viewData.m_positionY - ammoOffset[1],
								 RES_GAME_YELLOW_AMMO,
								 0,
								 p_viewData.m_animationTime,
								 0);
		break;
	case (eAction) 0x1a:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - pelletOffset[0],
								 p_viewData.m_positionY - pelletOffset[1],
								 RES_GAME_EX_PELLET,
								 p_viewData.m_stateTimer,
								 p_viewData.m_animationTime,
								 0);
		break;
	}
}

// 68K 0x10b02d9e DrawRocket__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043c6e0
void C2D::DrawRocket(ViewData& p_viewData)
{
	int elapsed;
	int frame;
	int extraFrame = -1;

	elapsed = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * 15 / 1000;
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
		frame = (elapsed & 1) + 33;
		extraFrame = 32;
	}

	if (extraFrame != -1) {
		m_lemmingAnims
			->DrawAnim(p_viewData.m_positionX - 13, p_viewData.m_positionY - 73, RES_GAME_ROCKET, extraFrame, 0, 0);
	}
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - 13, p_viewData.m_positionY - 73, RES_GAME_ROCKET, frame, 0, 0);
}

// 68K 0x10b02ede DrawHand__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043c7f0
void C2D::DrawHand(ViewData& p_viewData)
{
	int drawX;
	int drawY;
	int frame;
	BaseRemap* remap;
	eAction action = p_viewData.m_action;

	drawX = p_viewData.m_positionX - 0x31;
	drawY = p_viewData.m_positionY - 0x14;
	remap = 0;
	if (p_viewData.m_actionArgument != 0) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case (eAction) 0x17:
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(drawX, drawY, g_anGroundStyleResourceIds[2], 0, 0, 0);
		break;
	case (eAction) 0x19:
	case (eAction) 0x1a:
		frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * 15 / 1000;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(drawX, drawY, g_anGroundStyleResourceIds[2], frame, 0, (Remap*) remap);
		break;
	}
}

// 68K 0x10b02fb0 DrawLemmingOnBalloon__3C2DFR9CViewDataiUc
// FUNCTION: LEMBALL 0x0043c8a0
void C2D::DrawLemmingOnBalloon(ViewData& p_viewData, int p_balloonType, int p_remapped)
{
	int x;
	int y;
	int xOffset;
	int yOffset;
	unsigned int phase;
	BaseRemap* remap;
	BaseRemap* balloonRemap;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	phase = ((p_viewData.m_animationTime - p_viewData.m_stateTimer) & 0x7ff) >> 7;

	if (p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = 0;
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
		balloonRemap = 0;
	}

	m_lemmingAnims->DrawAnim(x - 16, y - 64, RES_GAME_BALLOON, 0, 0, (Remap*) balloonRemap);
	m_lemmingAnims
		->DrawAnim(x - g_unk0x496fd8[0] - 14, y - g_unk0x496fd8[1] - 12, RES_GAME_ONBALLOON, 0, 0, (Remap*) remap);
}

// 68K 0x10b030b6 DrawBalloon__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043c940
void C2D::DrawBalloon(ViewData& p_viewData, int p_playerIndex)
{
	BaseRemap* remap;
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	int xOffset;
	int yOffset;
	unsigned int phase = ((p_viewData.m_animationTime - p_viewData.m_stateTimer) & 0x7ff) >> 7;

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
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = 0;
	}

	m_lemmingAnims->DrawAnim(x + xOffset - 16, y + yOffset / 4 - 64, RES_GAME_BALLOON, 0, 0, (Remap*) remap);
	m_lemmingAnims->DrawAnim(x + xOffset - 9, y + yOffset / 4 - 9, RES_GAME_BALLOON_SHADOW, 0, 0, 0);
}

// 68K 0x10b031aa DrawBalloonPost__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043c9f0
void C2D::DrawBalloonPost(ViewData& p_viewData, int p_playerIndex)
{
	int x;
	int y;
	BaseRemap* remap;

	x = p_viewData.m_positionX - 0x10;
	y = p_viewData.m_positionY - 0x40;
	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = 0;
	}
	m_lemmingAnims->DrawAnim(x, y, RES_GAME_BALLOON_POST, 0, 0, (Remap*) remap);
}

// 68K 0x10b03222 DrawTrampoline__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043ca30
void C2D::DrawTrampoline(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x00497098
	static const short trampolineOffset[] = {22, 22};

	int x;
	int y;
	int frame;

	x = p_viewData.m_positionX - trampolineOffset[0];
	y = p_viewData.m_positionY - trampolineOffset[1];

	switch (p_viewData.m_action) {
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_TRAMPOLINE, 0, 0, 0);
		break;

	case (eAction) 0x1b:
		frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * 15 / 1000;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_TRAMPOLINE, frame, 0, 0);
		break;
	}
}

// 68K 0x10b032ec DrawMover__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cac0
void C2D::DrawMover(ViewData& p_viewData)
{
}

// 68K 0x10b033f0 DrawSlinky__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cbb0
void C2D::DrawSlinky(ViewData& p_viewData)
{
}

// 68K 0x10b035b4 DrawPaintGun__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cd50
void C2D::DrawPaintGun(ViewData& p_viewData)
{
}

// 68K 0x10b036dc DrawLaserFire__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043ce30
void C2D::DrawLaserFire(ViewData& p_viewData)
{
	int x;
	int y;

	switch (p_viewData.m_objectType) {
	case (eObjectType) 0x26:
		x = p_viewData.m_positionX - 0xd;
		y = p_viewData.m_positionY - 9;
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_LASER_FIRE_NORTH, 0, 0, 0);
		break;
	case (eObjectType) 0x32:
		x = p_viewData.m_positionX - 0x16;
		y = p_viewData.m_positionY - 0xf;
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_LASER_FIRE_EAST, 0, 0, 0);
		break;
	}
}

// 68K 0x10b03788 DrawLaser__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043cea0
void C2D::DrawLaser(ViewData& p_viewData)
{
	eAction action;
	int x;
	int y;
	unsigned long resourceId;
	int frame;

	action = p_viewData.m_action;

	switch (p_viewData.m_objectType) {
	case (eObjectType) 0x1e:
	case (eObjectType) 0x30:
		resourceId = RES_GAME_LASER_EAST;
		x = p_viewData.m_positionX - 0x14;
		y = p_viewData.m_positionY - 0xa;
		break;
	case (eObjectType) 0x2f:
	case (eObjectType) 0x31:
		resourceId = RES_GAME_LASER_NORTH;
		x = p_viewData.m_positionX - 0x2e;
		y = p_viewData.m_positionY - 0xa;
		break;
	}

	switch (action) {
	case (eAction) 0x17:
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, 0);
		break;
	case (eAction) 0x19:
	case (eAction) 0x1a:
		frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * 15 / 1000;
		if (frame > 17) {
			frame = 17;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, frame, 0, 0);
		break;
	}
}

// 68K 0x10b03882 DrawDuplicator__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043cfa0
void C2D::DrawDuplicator(ViewData& p_viewData)
{
	int x;
	int y;
	BaseRemap* remap;
	eAction action;
	unsigned int elapsed;
	int frame;

	elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	action = p_viewData.m_action;
	x = p_viewData.m_positionX - 0x1c;
	y = p_viewData.m_positionY - 0x3f;
	remap = 0;

	if (p_viewData.m_actionArgument != 0) {
		remap = m_paletteRemap;
	}
	m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, 0, 0, (Remap*) remap);

	switch (action) {
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, 0x3f, 0, (Remap*) remap);
		break;
	case (eAction) 0x1a:
		frame = elapsed * 15 / 1000;
		if (frame > 0x3e) {
			frame = 0x3e;
		}
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, frame + 1, 0, (Remap*) remap);
		break;
	}
}

// 68K 0x10b0397e DrawCrate__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043d070
void C2D::DrawCrate(ViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049703c
	static const short crateOffset[] = {8, 24};
	// GLOBAL: LEMBALL 0x00497040
	static const short crateExplosionOffset[] = {34, 50};

	eAction action;
	unsigned int stateTimer;

	action = p_viewData.m_action;
	stateTimer = p_viewData.m_stateTimer;

	switch (action) {
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - crateOffset[0],
								 p_viewData.m_positionY - crateOffset[1],
								 RES_GAME_CRATE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 0);
		break;
	case (eAction) 0x19:
	case (eAction) 0x1a:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - crateExplosionOffset[0],
								 p_viewData.m_positionY - crateExplosionOffset[1],
								 RES_GAME_CRATE_EXPLODE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 0);
		break;
	}
}

// 68K 0x10b03a4e DrawTimeBonus__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d0f0
void C2D::DrawTimeBonus(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x00497088
	static const short timeBonusOffset[] = {16, 18};

	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - timeBonusOffset[0],
							 p_viewData.m_positionY - timeBonusOffset[1],
							 RES_GAME_TIME_BONUS,
							 p_viewData.m_stateTimer,
							 p_viewData.m_animationTime,
							 0);
}

// 68K 0x10b03acc DrawCatapult__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043d130
void C2D::DrawCatapult(ViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x00497044
	static const short baseOffset[] = {40, 60};
	// GLOBAL: LEMBALL 0x00497048
	static const short animOffset[] = {40, 60};

	int x;
	int y;
	BaseRemap* remap;
	eAction action;
	unsigned int stateTimer;

	action = p_viewData.m_action;
	stateTimer = p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	remap = 0;

	if (p_viewData.m_actionArgument != 0) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(x - baseOffset[0], y - baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		m_lemmingAnims->DrawAnim(x - baseOffset[0], y - baseOffset[1], g_anGroundStyleResourceIds[8], 0, 0, 0);
		break;

	case (eAction) 0x19:
		m_lemmingAnims->DrawAnim(x - baseOffset[0], y - baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		m_lemmingAnims->DrawAnim(x - baseOffset[0], y - baseOffset[1], g_anGroundStyleResourceIds[8], 0, 0, 0);
		m_lemmingAnims->DrawAnim(x - animOffset[0] - 8,
								 y - animOffset[1],
								 RES_GAME_CATMOUNT_SE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 (Remap*) remap);
		break;

	case (eAction) 0x1a:
		m_lemmingAnims->DrawAnim(x - baseOffset[0], y - baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		m_lemmingAnims->DrawAnim(x - animOffset[0],
								 y - animOffset[1],
								 g_anGroundStyleResourceIds[9],
								 stateTimer + 0x640,
								 p_viewData.m_animationTime,
								 0);
		m_lemmingAnims->DrawAnim(x - animOffset[0] - 8,
								 y - animOffset[1],
								 RES_GAME_CATMOUNT_SE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 (Remap*) remap);
		break;

	case (eAction) 0x1b:
		m_lemmingAnims->DrawAnim(x - baseOffset[0], y - baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		m_lemmingAnims->DrawAnim(x - animOffset[0],
								 y - animOffset[1],
								 g_anGroundStyleResourceIds[9],
								 stateTimer + 0x640,
								 p_viewData.m_animationTime,
								 0);
		break;
	}
}

// 68K 0x10b03d7e DrawSheep__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043d370
void C2D::DrawSheep(ViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049ef58
	static unsigned long sheepWalkResources[] = {
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
	static unsigned long sheepMunchResources[] = {
		RES_GAME_SHEEP_MUNCH_NE,
		RES_GAME_SHEEP_WALK_E,
		RES_GAME_SHEEP_MUNCH_SE,
		RES_GAME_SHEEP_WALK_S,
		RES_GAME_SHEEP_MUNCH_SW,
		RES_GAME_SHEEP_WALK_W,
		RES_GAME_SHEEP_MUNCH_NW,
		RES_GAME_SHEEP_WALK_N,
	};
	// GLOBAL: LEMBALL 0x0049706c
	static const short sheepOffset[] = {9, 8};

	unsigned int direction;
	unsigned int stateTimer;
	int y;
	int x;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_unk0x90c * 2) & 7;
	stateTimer = p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case (eAction) 0:
	case (eAction) 1:
	case (eAction) 4:
		m_lemmingAnims->DrawAnim(x - sheepOffset[0],
								 y - sheepOffset[1],
								 sheepMunchResources[direction],
								 stateTimer,
								 p_viewData.m_animationTime,
								 0);
		break;

	case (eAction) 2:
		m_lemmingAnims->DrawAnim(x - sheepOffset[0],
								 y - sheepOffset[1],
								 sheepWalkResources[direction],
								 stateTimer,
								 p_viewData.m_animationTime,
								 0);
		break;
	}
}

// 68K 0x10b03e6e DrawBall__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d420
void C2D::DrawBall(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x00497074
	static const short ballOffset[] = {10, 15};
	// GLOBAL: LEMBALL 0x00497078
	static const short explosionOffset[] = {15, 17};

	int x;
	int y;
	int elapsed;
	int frame;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case (eAction) 0x25:
		m_lemmingAnims->DrawAnim(x - ballOffset[0], y - ballOffset[1], RES_GAME_BALL, 0, p_viewData.m_animationTime, 0);
		break;
	case (eAction) 0x26:
		elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
		frame = elapsed / 64;
		if (frame > 8) {
			frame = 8;
		}
		m_lemmingAnims->DrawAnim(x - explosionOffset[0], y - explosionOffset[1], RES_GAME_BALL_EXPLODE, frame, 0, 0);
		break;
	}
}

// 68K 0x10b03f56 DrawKey__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043d4b0
void C2D::DrawKey(ViewData& p_viewData, int p_playerIndex)
{
	// GLOBAL: LEMBALL 0x00497054
	static const short keyOffset[] = {8, 32};

	BaseRemap* remap;

	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = 0;
	}

	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - keyOffset[0],
							 p_viewData.m_positionY - keyOffset[1],
							 RES_GAME_KEYS,
							 0,
							 0,
							 (Remap*) remap);
}

// 68K 0x10b03fc8 DrawMine__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d500
void C2D::DrawMine(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x0049704c
	static const short mineOffset[] = {30, 35};
	// GLOBAL: LEMBALL 0x00497050
	static const short mineStillOffset[] = {2, 2};

	unsigned int stateTimer;
	int x;
	int y;
	eAction action;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	stateTimer = p_viewData.m_stateTimer;
	action = p_viewData.m_action;

	switch (action) {
	case (eAction) 8:
		break;
	case (eAction) 0x18:
	case (eAction) 0x19:
	case (eAction) 0x1a:
		m_lemmingAnims->DrawAnim(x - mineStillOffset[0], y - mineStillOffset[1], RES_GAME_MINE_STILL, 0, 0, 0);
		break;
	case (eAction) 0x1b:
		m_lemmingAnims
			->DrawAnim(x - mineOffset[0], y - mineOffset[1], RES_GAME_MINE, stateTimer, p_viewData.m_animationTime, 0);
		break;
	}
}

// 68K 0x10b0409a DrawDoor__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d590
void C2D::DrawDoor(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x00497094
	static const short doorOffset[] = {26, 24};

	int x;
	int y;
	int elapsed;
	eObjectType objectType;
	eAction action;
	unsigned long resourceId;
	int playerIndex;
	BaseRemap* remap;
	int frame;

	x = p_viewData.m_positionX - doorOffset[0];
	y = p_viewData.m_positionY - doorOffset[1];
	action = p_viewData.m_action;
	objectType = p_viewData.m_objectType;
	elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;

	switch (objectType) {
	case (eObjectType) 0x19:
		resourceId = RES_GAME_DOOR_2;
		break;
	case (eObjectType) 0x1a:
		resourceId = RES_GAME_DOOR;
		break;
	}

	switch (action) {
	case (eAction) 0x1c:
		switch ((unsigned short) p_viewData.m_actionArgument) {
		case 0x14:
			playerIndex = -1;
			break;
		case 0x15:
			playerIndex = 3;
			break;
		case 0x16:
			playerIndex = 1;
			break;
		case 0x17:
			playerIndex = 4;
			break;
		}

		if (playerIndex >= 0) {
			if (playerIndex < 4) {
				remap = m_remaps[playerIndex];
			}
			else {
				remap = 0;
			}
			m_lemmingAnims->DrawAnim(x + 16, y - 20, RES_GAME_KEYS, 0, 0, (Remap*) remap);
		}

		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, 0);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 1, 0, 0);
		break;

	case (eAction) 0x1d:
	case (eAction) 0x1e:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, 0);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 1, 0, 0);
		break;

	case (eAction) DOOR_ACTION_OPENING:
		frame = elapsed * 15 / 1024;
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, 0);
		m_lemmingAnims->DrawAnim(x, y, resourceId, frame + 1, 0, 0);
		break;

	case (eAction) DOOR_ACTION_OPEN:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, 0);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 8, 0, 0);
		break;

	case (eAction) DOOR_ACTION_CLOSING:
		frame = elapsed * 15 / 1024;
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, 0);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 8 - frame, 0, 0);
		break;
	}
}

// 68K 0x10b042fa DrawSwitch__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d7e0
void C2D::DrawSwitch(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x0049707c
	static const short switchOffset[] = {5, 25};

	int x;
	int y;
	unsigned int stateTimer;
	eAction action;
	unsigned short actionArgument;

	x = p_viewData.m_positionX - switchOffset[0];
	y = p_viewData.m_positionY - switchOffset[1];
	stateTimer = p_viewData.m_stateTimer;
	action = p_viewData.m_action;
	actionArgument = (unsigned short) p_viewData.m_actionArgument;

	switch (action) {
	case (eAction) 7:
	case (eAction) 0x18:
		switch (actionArgument) {
		case 0:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH, 0, 0, 0);
			break;
		case 1:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH, 0, 0, 0);
			break;
		}
		break;

	case (eAction) 0x1a:
		switch (actionArgument) {
		case 0:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH_ANIM, stateTimer, p_viewData.m_animationTime, 0);
			break;
		case 1:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH_ANIM, stateTimer, p_viewData.m_animationTime, 0);
			break;
		}
		break;
	}
}

// 68K 0x10b0440c DrawFlag__3C2DFR9CViewData11eObjectType
// FUNCTION: LEMBALL 0x0043d8d0
void C2D::DrawFlag(ViewData& p_viewData, eObjectType p_objectType)
{
	// GLOBAL: LEMBALL 0x00497080
	static const short flagOffset[] = {15, 28};

	int x;
	int y;

	x = p_viewData.m_positionX - flagOffset[0];
	y = p_viewData.m_positionY - flagOffset[1];

	switch (p_objectType) {
	case (eObjectType) 0xb:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_FLAG_GREEN, 0, p_viewData.m_animationTime, (Remap*) m_remaps[3]);
		break;

	case (eObjectType) 0xc:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_FLAG_GREEN, 0, p_viewData.m_animationTime, (Remap*) m_remaps[1]);
		break;
	}
}

// 68K 0x10b044dc DrawBonus__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d950
void C2D::DrawBonus(ViewData& p_viewData)
{
	// GLOBAL: LEMBALL 0x00497084
	static const short bonusOffset[] = {16, 16};

	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - bonusOffset[0],
							 p_viewData.m_positionY - bonusOffset[1],
							 RES_GAME_BONUS,
							 0,
							 p_viewData.m_animationTime,
							 0);
}

// 68K 0x10b0454a DrawTrapDoor__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d990
void C2D::DrawTrapDoor(ViewData& p_viewData)
{
}

// 68K 0x10b0487c DrawObject__3C2DFR9CViewData
// STUB: LEMBALL 0x0043dc70
void C2D::DrawObject(ViewData& p_viewData)
{
}

// 68K 0x10b04c66 SetOrigin__3C2DFv
// FUNCTION: LEMBALL 0x0043ed20
void C2D::SetOrigin()
{
	AiCoord origin;
	unsigned int player;
	int changed = 0;

	if (m_groupingActive == 1 && m_groupSelectionCount != 0) {
		m_ai->GetPlayerPos(m_groupObjectIds[m_groupSelectionCount - 1], origin);
	}
	else if (m_ai->GetOrigin(origin, player) == 0) {
		return;
	}

	int gameX = origin.m_xFixed >> 12;
	int gameY = origin.m_yFixed >> 12;
	int blockX = origin.m_xFixed >> 16;
	int blockY = origin.m_yFixed >> 16;
	unsigned short z = 0;
	if (gameX >= 0 && gameY >= 0 && blockX < m_map->m_ground.m_width && blockY < m_map->m_ground.m_height) {
		z = m_map->m_ground.m_ground[blockY * m_map->m_ground.m_width + blockX].GetZ((origin.m_xFixed >> 12) & 0xf,
																					 (origin.m_yFixed >> 12) & 0xf);
	}
	origin.m_zFixed = (int) z << 12;

	if (m_ai->m_gameStatus == 0 || m_ai->m_gameStatus == 2) {
		int marginX = m_clipSize.m_x * 2 / 5;
		int marginY = m_clipSize.m_y * 2 / 5;
		int oldViewOriginX = m_viewOriginX;
		int oldViewOriginY = m_viewOriginY;
		m_originPosition = origin;

		int screenX = origin.m_xFixed >> 12;
		int screenY = origin.m_yFixed >> 12;
		int screenZ = origin.m_zFixed >> 12;
		m_map->GameToScreen(screenX, screenY);
		int projectedX = screenX;
		int projectedY = (screenY - screenZ) * 0x1000;
		int projectedYGame = projectedY >> 12;
		int differenceX = projectedX - m_viewOriginX;
		int differenceY = projectedYGame - m_viewOriginY;
		if (differenceX < marginX) {
			changed = 1;
			m_viewOriginX = projectedX - marginX;
		}
		if (differenceY < marginY) {
			changed = 1;
			m_viewOriginY = projectedYGame - marginY;
		}
		if (m_clipSize.m_x - marginX < differenceX) {
			changed = 1;
			m_viewOriginX = projectedX - m_clipSize.m_x + marginX;
		}
		if (m_clipSize.m_y - marginY < differenceY) {
			changed = 1;
			m_viewOriginY = projectedYGame - m_clipSize.m_y + marginY;
		}
		if (changed != 0) {
			SendCursorMsg();
			m_scrollPending = 1;
			m_scrollDeltaX = (short) oldViewOriginX - (short) m_viewOriginX;
			m_scrollDeltaY = (short) oldViewOriginY - (short) m_viewOriginY;
		}
	}
}

// 68K 0x10b04ede DrawObjects__3C2DFv
// STUB: LEMBALL 0x0043f620
void C2D::DrawObjects()
{
}

// 68K 0x10b05744 DrawDemo__3C2DFv
// STUB: LEMBALL 0x0043fce0
void C2D::DrawDemo()
{
}

// 68K 0x10b05804 DrawTime__3C2DFv
// FUNCTION: LEMBALL 0x0043fd80
void C2D::DrawTime()
{
	unsigned short baseTime = (unsigned short) m_ai->m_unk0xe4;
	short time = (short) m_ai->m_gameTime;
	time = (short) (time + baseTime);
	if (time < 0) {
		time = 0;
	}
	if (time >= 600) {
		if (baseTime >= 600) {
			return;
		}
		if (time >= 600) {
			time = 599;
		}
	}

	if (time != g_lastDrawnTime) {
		g_lastDrawnTime = time;
		int seconds = time % 60;
		g_timeText[0] = (char) (time / 60) + '0';
		g_timeText[1] = ':';
		g_timeText[2] = (char) (seconds / 10) + '0';
		g_timeText[3] = (char) (seconds % 10) + '0';
		g_timeText[4] = 0;
	}

	VsPoint& position = m_spriteGroundLookupRectA;
	VsSize advance;
	advance.m_width = -4;
	advance.m_height = 0;
	m_textManager->DrawString(m_gdi, position, advance, RES_NEWFRONT_FONTS_GAME_SCORETIME, g_timeText, 0x20, 0);
}

// 68K 0x10b05914 DrawPaused__3C2DFv
// FUNCTION: LEMBALL 0x0043fe70
void C2D::DrawPaused()
{
}

// 68K 0x10b05932 DrawScore__3C2DFv
// FUNCTION: LEMBALL 0x0043fe80
void C2D::DrawScore()
{
	int targetScore = m_ai->m_score;
	int score = m_score;
	if (m_scoreTimestamp <= g_dwGameTick) {
		if (score != targetScore) {
			if (m_ai->m_gameStatus == 2) {
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
	if (score >= 10000000) {
		score = 9999999;
	}

	VsSize advance;
	char scoreText[8];
	scoreText[7] = 0;
	int i = 1;
	do {
		scoreText[7 - i] = (char) (score % 10) + '0';
		i++;
		score /= 10;
	} while (i <= 7);

	VsPoint* position = &m_spriteGroundLookupRectB;
	advance.m_width = -4;
	advance.m_height = 0;
	m_textManager->DrawString(m_gdi, *position, advance, RES_NEWFRONT_FONTS_GAME_SCORETIME, scoreText, 0x20, 0);
}

// 68K 0x10b05a64 SortViewData__3C2DFv
// STUB: LEMBALL 0x0043ff70
void C2D::SortViewData()
{
}

// 68K 0x10b05b06 Draw__3C2DFRC7CVSRect
// STUB: LEMBALL 0x00440000
void C2D::Draw(const VsRect& p_rect)
{
}

#include "../../Visos/Foundation/ChangeList.h"

// 68K 0x10b05fa8 ReplaceBackground__3C2DFv
// FUNCTION: LEMBALL 0x00440400
void C2D::ReplaceBackground()
{
	ChangeList* changeList = m_gdi->m_renderTarget->GetChangeList();
	m_drawingMark.Draw(m_gdi);
	changeList->Reset();
}

// 68K 0x10b0600e ResetPrimitives__3C2DFv
// FUNCTION: LEMBALL 0x00440430
void C2D::ResetPrimitives()
{
	m_lemmingAnims->ResetPrimitives();
	m_textManager->ResetPrimitives();
	m_unk0xc90 = 0;
	m_primitiveCount = 0;
}

// 68K 0x10b06056 DrawZBuff_Sprite__3C2DFiUs
// FUNCTION: LEMBALL 0x00440460
void C2D::DrawZBuffSprite(int p_index, unsigned short p_z)
{
	m_lemmingAnims->m_primitiveSequence = p_z;
	DrawObject(m_viewData[p_index]);
}

// 68K 0x10b060ac DrawZBuff_Anim__3C2DFiUs
// FUNCTION: LEMBALL 0x00440490
void C2D::DrawZBuffAnim(int p_index, unsigned short p_z)
{
	AnimSpecialEntry* animation = m_zBufferAnimations + p_index;
	int gameX = (unsigned short) animation->m_x << 4;
	int gameY = (unsigned short) animation->m_y << 4;
	Ground* ground = animation->m_groundEntry;
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
	if ((collision & 0x20) != 0 && height > 0) {
		DrawCliff(screenX, screenY, height, cliff);
	}
	DrawGround(screenX, screenY - height, groundType, frame);
}

// 68K 0x10b061ac DrawObjectsZBuff__3C2DFv
// FUNCTION: LEMBALL 0x00440560
void C2D::DrawObjectsZBuff()
{
	VsRect backgroundBounds(0, 0, m_clipSize.m_x, m_clipSize.m_y);
	SolidRect& background = m_solidRects[m_primitiveCount++];
	background.m_left = backgroundBounds.m_width;
	background.m_top = backgroundBounds.m_height;
	background.m_right = backgroundBounds.m_x;
	background.m_bottom = backgroundBounds.m_y;
	background.m_color = 0;
	background.Draw(m_gdi);

	int viewIndex = 0;
	if ((int) m_viewDataCount > 0) {
		do {
			m_viewData[viewIndex].m_viewX = (short) m_viewData[viewIndex].m_positionX;
			m_viewData[viewIndex].m_viewY = (short) m_viewData[viewIndex].m_positionY;

			C3DVector position;
			position.m_xFixed = m_viewData[viewIndex].m_positionX;
			position.m_yFixed = m_viewData[viewIndex].m_positionY;
			position.m_zFixed = m_viewData[viewIndex].m_positionZ;
			m_map->GameToScreen(position.m_xFixed, position.m_yFixed);
			position.m_xFixed -= m_viewOriginX;
			position.m_yFixed -= position.m_zFixed;
			position.m_yFixed -= m_viewOriginY;
			m_viewData[viewIndex].m_positionX = position.m_xFixed;
			m_viewData[viewIndex].m_positionY = position.m_yFixed;
			m_viewData[viewIndex].m_positionZ = position.m_zFixed;
			viewIndex++;
		} while ((int) m_viewDataCount > viewIndex);
	}

	SortViewData();
	m_lemmingAnims->m_reservedac = 0x80000;

	int spriteIndex = 0;
	int animationIndex = 0;
	bool spriteZValid = false;
	bool animationZValid = false;
	unsigned short spriteZ;
	unsigned short animationZ;
	int animationOffset = 0;
	AnimSpecial* animations = m_ai->m_animSpecial;
	m_zBufferAnimationCount = animations->m_entryCount;
	m_zBufferAnimations = animations->m_entries;

	while (spriteIndex < (int) m_viewDataCount && animationIndex < m_zBufferAnimationCount) {
		if (!spriteZValid) {
			spriteZ = (unsigned short) CalcZValueSprite(spriteIndex);
			spriteZValid = true;
		}
		if (!animationZValid) {
			AnimSpecialEntry* animation = (AnimSpecialEntry*) ((unsigned char*) m_zBufferAnimations + animationOffset);
			animationZ = animation->m_groundEntry->m_height + animation->m_sortKey;
			animationZValid = true;
		}
		if (animationZ < spriteZ) {
			DrawZBuffAnim(animationIndex, animationZ);
			animationOffset += sizeof(AnimSpecialEntry);
			animationIndex++;
			animationZValid = false;
		}
		else {
			DrawZBuffSprite(spriteIndex, spriteZ);
			spriteIndex++;
			spriteZValid = false;
		}
	}

	while (spriteIndex < (int) m_viewDataCount) {
		unsigned short z = (unsigned short) CalcZValueSprite(spriteIndex);
		DrawZBuffSprite(spriteIndex, z);
		spriteIndex++;
	}

	while (animationIndex < m_zBufferAnimationCount) {
		AnimSpecialEntry* animation = m_zBufferAnimations + animationIndex;
		unsigned short z = animation->m_groundEntry->m_height + animation->m_sortKey;
		DrawZBuffAnim(animationIndex, z);
		animationIndex++;
	}
}

// 68K 0x10b06416 CalcZValue_Sprite__3C2DFi
// FUNCTION: LEMBALL 0x004407e0
unsigned short C2D::CalcZValueSprite(int p_index)
{
	eObjectType objectType = m_viewData[p_index].m_objectType;
	if (objectType == 0x18) {
		return 0x7fff;
	}

	ViewData* viewData = m_viewData + p_index;
	unsigned short z = (unsigned short) viewData->m_positionZ;
	return CalcGroundCode(objectType, (unsigned short) viewData->m_viewX, (unsigned short) viewData->m_viewY, z) + z +
		   1;
}

// 68K 0x10b064c6 CalcGroundCode__3C2DF11eObjectTypeiiUs
// STUB: LEMBALL 0x00440840
unsigned short C2D::CalcGroundCode(eObjectType p_objectType, int p_x, int p_y, unsigned short p_z)
{
	return 0;
}

// 68K 0x10b06752 InitSpriteGroundLU__3C2DFv
// FUNCTION: LEMBALL 0x00440c00
void C2D::InitSpriteGroundLu()
{
}
