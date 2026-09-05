#include "C2D.h"

#include "../../AI/Navigation/Ai.h"
#include "../../AI/Objects/PlayerLemming.h"
#include "../../AI/Objects/ViewData.h"
#include "../../Control/Game/GameTime.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Animation/LemmingAnimsManager.h"
#include "../Panel/Panel.h"
#include "../Sound/SoundView.h"
#include "Main2DDisplay.h"

#include <string.h>

// 68K 0x10b06778 __ct__3C2DFP14CMain2DDisplayP3CAIP4CGDIP4CMapRC7CVSRect
// STUB: LEMBALL 0x004358d0
C2D::C2D(Main2DDisplay* p_arg0, Ai* p_arg1, Gdi* p_arg2, Map* p_arg3, const VsRect& p_arg4)
{
}

// 68K 0x10b06e3e __dt__3C2DFv
// STUB: LEMBALL 0x00436050
C2D::~C2D()
{
}

// 68K 0x10b070a2 ShutDown__3C2DFv
// STUB: LEMBALL 0x00436190
void C2D::ShutDown()
{
}

// 68K 0x10b0735a RegisterRemaps__3C2DFv
// STUB: LEMBALL 0x004363c0
void C2D::RegisterRemaps()
{
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
// STUB: LEMBALL 0x004364d0
void C2D::OnLoaded()
{
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
// STUB: LEMBALL 0x004366b0
void C2D::OnSize(const VsRect& p_rect)
{
}

// 68K 0x10b07924 SetUpRemapPalettes__3C2DFv
// STUB: LEMBALL 0x00436760
void C2D::SetUpRemapPalettes()
{
}

// 68K 0x10b079c8 KillRemapPalettes__3C2DFv
// FUNCTION: LEMBALL 0x00436830
void C2D::KillRemapPalettes()
{
	g_pBasePalManager->UnRegisterRemap(m_paletteRemap);
}

// 68K 0x10b07a00 Restart__3C2DFv
// STUB: LEMBALL 0x00436850
void C2D::Restart()
{
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
void C2D::AddObjectToGroup(int p_objectNo, unsigned char p_markSelection)
{
	m_groupObjectIds[m_groupCount] = (unsigned short) p_objectNo;
	m_groupCount++;
	if (*(unsigned int*) &p_markSelection != 0) {
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
	m_lemmingManager->ProcessMsg(&msg);
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
	m_lemmingManager->ProcessMsg(&msg);
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
	m_lemmingManager->ProcessMsg(&msg);
	m_groupCount = 0;
	m_groupingActive = 0;
	g_pSoundView->m_pendingEffect = (eSoundEffect) 0x1b;
}

// 68K 0x108084fe SelectLemming__3C2DFi
// FUNCTION: LEMBALL 0x00437340
void C2D::SelectLemming(int p_playerIndex)
{
	Message msg;
	msg.type = 8;
	msg.time = 0;
	msg.code = m_ai->m_networkLemmings[p_playerIndex]->m_objectId;
	memset(&msg.payload, 0, sizeof(msg.payload) + sizeof(msg.source));
	m_lemmingManager->ProcessMsg(&msg);
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
	m_lemmingManager->ProcessMsg(&msg);
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
	unsigned int i;
	unsigned short* write;
	unsigned short* read;
	unsigned short id;

	i = 0;
	if (m_groupCount <= i) {
	}
	else {
		write = m_groupObjectIds;
		read = write;
		do {
			id = *read;
			if ((unsigned int) id != (unsigned int) p_objectNo) {
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
// STUB: LEMBALL 0x00437970
bool C2D::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
{
	return 0;
}

// 68K 0x10b08e94 ProcessMsg__3C2DFP10tagMESSAGE
// STUB: LEMBALL 0x00437b60
int C2D::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b09008 NewPauseWindow__3C2DF20ePauseWindowMessages
// STUB: LEMBALL 0x00437d00
void C2D::NewPauseWindow(int p_message)
{
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

// GLOBAL: LEMBALL 0x0049ea14
int g_nMouseShapeGameX = 0;

// GLOBAL: LEMBALL 0x0049ea18
int g_nMouseShapeGameY = 0;

// GLOBAL: LEMBALL 0x0049ea1c
unsigned int g_nMouseShapeOnGround = 0;

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
// STUB: LEMBALL 0x004380c0
void C2D::SendCursorMsg()
{
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
// STUB: LEMBALL 0x00438500
void C2D::SetClipSize()
{
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
// STUB: LEMBALL 0x0043a880
void C2D::DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame)
{
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
// STUB: LEMBALL 0x0043b4b0
void C2D::DrawClipData()
{
}

// 68K 0x10b01d3a DrawClippedRectangle__3C2DFRC7CVSRect
// STUB: LEMBALL 0x0043b4d0
void C2D::DrawClippedRectangle(const VsRect& p_rect)
{
}

// 68K 0x10b02196 LemmingFly__3C2DFR9CViewDataRi
// STUB: LEMBALL 0x0043bce0
unsigned long C2D::LemmingFly(ViewData& p_viewData, int& p_frame)
{
	return 0;
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
void C2D::DrawLemmingOnConveyor(ViewData& p_viewData, unsigned char p_remapped)
{
	int x;
	int y;
	int frame;
	BaseRemap* remap;

	frame = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	frame = (frame * 15 / 1000) % 8;
	if (*(unsigned int*) &p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = 0;
	}
	m_lemmingAnims->DrawAnim(x - 15, y - 22, RES_GAME_LEMMING_SPIN, frame, 0, (Remap*) remap);
}

// 68K 0x10b027f2 DrawLemming__3C2DFR9CViewDataiUc
// STUB: LEMBALL 0x0043c200
void C2D::DrawLemming(ViewData& p_viewData, int p_objectNo, unsigned char p_remapped)
{
}

// 68K 0x10b02c40 DrawBullet__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043c610
void C2D::DrawBullet(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b02cce DrawAmmo__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043c660
void C2D::DrawAmmo(ViewData& p_viewData, int p_objectNo)
{
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

#include "../../Frontend/Resources/FrontendResourceLoader.h"

// 68K 0x10b02ede DrawHand__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043c7f0
void C2D::DrawHand(ViewData& p_viewData)
{
	int x;
	int y;
	int frame;
	BaseRemap* remap;
	eAction action = p_viewData.m_action;

	x = p_viewData.m_positionX - 0x31;
	y = p_viewData.m_positionY - 0x14;
	remap = 0;
	if (p_viewData.m_actionArgument != 0) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case (eAction) 0x17:
	case (eAction) 0x18:
		m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[2], 0, 0, 0);
		break;
	case (eAction) 0x19:
	case (eAction) 0x1a:
		frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * 15 / 1000;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[2], frame, 0, (Remap*) remap);
		break;
	}
}

// 68K 0x10b02fb0 DrawLemmingOnBalloon__3C2DFR9CViewDataiUc
// STUB: LEMBALL 0x0043c8a0
void C2D::DrawLemmingOnBalloon(ViewData& p_viewData, int p_balloonType, unsigned char p_remapped)
{
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
// STUB: LEMBALL 0x0043ca30
void C2D::DrawTrampoline(ViewData& p_viewData)
{
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
// STUB: LEMBALL 0x0043d070
void C2D::DrawCrate(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b03a4e DrawTimeBonus__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d0f0
void C2D::DrawTimeBonus(ViewData& p_viewData)
{
}

// 68K 0x10b03acc DrawCatapult__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043d130
void C2D::DrawCatapult(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b03d7e DrawSheep__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043d370
void C2D::DrawSheep(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b03e6e DrawBall__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d420
void C2D::DrawBall(ViewData& p_viewData)
{
	int x;
	int y;
	int elapsed;
	int frame;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case (eAction) 0x25:
		m_lemmingAnims->DrawAnim(x - 10, y - 15, RES_GAME_BALL, 0, p_viewData.m_animationTime, 0);
		break;
	case (eAction) 0x26:
		elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
		frame = elapsed / 64;
		if (frame > 8) {
			frame = 8;
		}
		m_lemmingAnims->DrawAnim(x - 15, y - 17, RES_GAME_BALL_EXPLODE, frame, 0, 0);
		break;
	}
}

// 68K 0x10b03f56 DrawKey__3C2DFR9CViewDatai
// FUNCTION: LEMBALL 0x0043d4b0
void C2D::DrawKey(ViewData& p_viewData, int p_playerIndex)
{
	BaseRemap* remap;

	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = 0;
	}

	m_lemmingAnims
		->DrawAnim(p_viewData.m_positionX - 8, p_viewData.m_positionY - 32, RES_GAME_KEYS, 0, 0, (Remap*) remap);
}

// 68K 0x10b03fc8 DrawMine__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d500
void C2D::DrawMine(ViewData& p_viewData)
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
	case (eAction) 8:
		break;
	case (eAction) 0x18:
	case (eAction) 0x19:
	case (eAction) 0x1a:
		m_lemmingAnims->DrawAnim(x - 2, y - 2, RES_GAME_MINE_STILL, 0, 0, 0);
		break;
	case (eAction) 0x1b:
		m_lemmingAnims->DrawAnim(x - 30, y - 35, RES_GAME_MINE, stateTimer, p_viewData.m_animationTime, 0);
		break;
	}
}

// 68K 0x10b0409a DrawDoor__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d590
void C2D::DrawDoor(ViewData& p_viewData)
{
}

// 68K 0x10b042fa DrawSwitch__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d7e0
void C2D::DrawSwitch(ViewData& p_viewData)
{
}

// 68K 0x10b0440c DrawFlag__3C2DFR9CViewData11eObjectType
// STUB: LEMBALL 0x0043d8d0
void C2D::DrawFlag(ViewData& p_viewData, eObjectType p_objectType)
{
}

// 68K 0x10b044dc DrawBonus__3C2DFR9CViewData
// FUNCTION: LEMBALL 0x0043d950
void C2D::DrawBonus(ViewData& p_viewData)
{
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - 16,
							 p_viewData.m_positionY - 16,
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
// STUB: LEMBALL 0x0043ed20
void C2D::SetOrigin()
{
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
// STUB: LEMBALL 0x0043fd80
void C2D::DrawTime()
{
}

// 68K 0x10b05914 DrawPaused__3C2DFv
// FUNCTION: LEMBALL 0x0043fe70
void C2D::DrawPaused()
{
}

// 68K 0x10b05932 DrawScore__3C2DFv
// STUB: LEMBALL 0x0043fe80
void C2D::DrawScore()
{
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

// 68K 0x10b05fa8 ReplaceBackground__3C2DFv
// STUB: LEMBALL 0x00440400
void C2D::ReplaceBackground()
{
}

// 68K 0x10b0600e ResetPrimitives__3C2DFv
// STUB: LEMBALL 0x00440430
void C2D::ResetPrimitives()
{
}

// 68K 0x10b06056 DrawZBuff_Sprite__3C2DFiUs
// STUB: LEMBALL 0x00440460
void C2D::DrawZBuffSprite(int p_index, unsigned short p_z)
{
}

// 68K 0x10b060ac DrawZBuff_Anim__3C2DFiUs
// STUB: LEMBALL 0x00440490
void C2D::DrawZBuffAnim(int p_index, unsigned short p_z)
{
}

// 68K 0x10b061ac DrawObjectsZBuff__3C2DFv
// STUB: LEMBALL 0x00440560
void C2D::DrawObjectsZBuff()
{
}

// 68K 0x10b06416 CalcZValue_Sprite__3C2DFi
// STUB: LEMBALL 0x004407e0
int C2D::CalcZValueSprite(int p_index)
{
	return 0;
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
