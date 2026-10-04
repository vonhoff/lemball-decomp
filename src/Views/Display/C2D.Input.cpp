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
#include "Frontend/CBaseFrontendProcess.h"
#include "Frontend/Loading/CFrontendResourceLoader.h"
#include "Map/CMap.h"
#include "Network/CNetworkManager.h"
#include "Visos/Queues/CBaseQueue.h"
#include "CObjSq.h"
#include "Visos/Text/CTextManager.h"
#include "Gameplay/Geometry/Facing.h"
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
#include "Frontend/FlowProcesses.h"
#include "Visos/Math/FixedPoint.h"

#include <new.h>
#include <string.h>

#include "Game/CGameStatus.h"

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

#include "Visos/Math/CFixed.h"

#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Mechanisms/LiftEndpointRecord.h"

#include "Visos/Streams/CVSOStream.h"
#include "Visos/Network/CConnect.h"
#include "Visos/Network/NetworkMode.h"
#include "Views/Animation/CLemmingAnimsManager.h"
#include "Views/Panel/CPanel.h"

#include <stddef.h>

#include "Views/Sound/CSoundView.h"

#include "Visos/Resources/Manifest.h"

#include <stdlib.h>

extern int g_anC2DHitBounds[11][4];

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

// GLOBAL: LEMBALL 0x0049ea14
int g_nMouseShapeGameX = 0;

// GLOBAL: LEMBALL 0x0049ea18
int g_nMouseShapeGameY = 0;

// GLOBAL: LEMBALL 0x0049ea1c
unsigned int g_nMouseShapeOnGround = 0;

// FUNCTION: LEMBALL 0x004364b0
void C2D::CursorChangeType(int p_cursorType, int p_value)
{
	::CursorChangeType((eCursorDisplayType) p_cursorType, p_value);
}

// FUNCTION: LEMBALL 0x00436690
void C2D::DoButtons()
{
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

// FUNCTION: LEMBALL 0x00437520
void C2D::NoStateLeftClick(const CVSPoint& p_screenPoint,
						   const CVSPoint& p_gamePoint,
						   unsigned int p_cancelMoves,
						   unsigned int p_alternate)
{
	CVSPoint destination(p_gamePoint);
	int index;
	if (FindGameObject(p_screenPoint, index, 0)) {
		CViewData* views = m_viewData;
		CViewData& view = views[index];
		switch (view.m_objectType) {
		case OBJECT_PLAYER_2:
			if (p_alternate == 0) {
				m_groupCount = 0;
				m_groupSelectionCount = 0;
				m_groupingActive = 1;
				AddObjectToGroup(views[index].m_objectId, 0);
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
			if (p_cancelMoves != 0) {
				CancelMoves();
			}
			SelectObject(index);
			return;
		case OBJECT_MOVER: {
			short y = view.m_gameY;
			short x = view.m_gameX;
			destination.m_x = x;
			destination.m_y = y;
			break;
		}
		default:
			return;
		}
	}
	if (p_cancelMoves != 0) {
		CancelMoves();
		MoveGroup(destination);
		return;
	}
	MoveGroup(destination);
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

// FUNCTION: LEMBALL 0x00437b60
int C2D::ProcessMsg(Message* p_message)
{
	if ((g_pDemo == NULL || g_pDemo->m_demoMode == 0) && !m_display->IsFocusWindow()) {
		return 0;
	}
	if (m_paused != 0) {
		return 0;
	}

	switch ((unsigned int) p_message->m_type) {
	case MESSAGE_KEY_DOWN:
	case MESSAGE_BUTTON_RELEASED:
		switch (p_message->m_code) {
		case INPUT_KEY_LEFT:
			PrevGroup();
			return 1;
		case INPUT_KEY_RIGHT:
			NextGroup();
			return 1;
		case INPUT_KEY_0:
			UseBalloon(3);
			return 1;
		case INPUT_KEY_1:
			SelectLemming(0);
			return 1;
		case INPUT_KEY_2:
			SelectLemming(1);
			return 1;
		case INPUT_KEY_3:
			SelectLemming(2);
			return 1;
		case INPUT_KEY_4:
			SelectLemming(3);
			return 1;
		case INPUT_KEY_7:
			UseBalloon(0);
			return 1;
		case INPUT_KEY_8:
			UseBalloon(1);
			return 1;
		case INPUT_KEY_9:
			UseBalloon(2);
			return 1;
		}
		return 0;
	default:
		m_processedCount++;
		return 0;
	}
}

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
