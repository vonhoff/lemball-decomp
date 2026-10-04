#include "C2D.h"

#include "Game/CDemo.h"
#include "Views/Sound/CSoundView.h"
#include "CMain2DDisplay.h"

#include <stddef.h>

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
