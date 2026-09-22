#include "../C2D.h"

#include "../../../Control/Game/CDemo.h"
#include "../../Sound/CSoundView.h"
#include "../CMain2DDisplay.h"

// FUNCTION: LEMBALL 0x00437520
void C2D::NoStateLeftClick(const CVsPoint& p_screenPoint,
						   const CVsPoint& p_gamePoint,
						   unsigned int p_cancelMoves,
						   unsigned int p_alternate)
{
	CVsPoint destination(p_gamePoint);
	int index;
	if (FindGameObject(p_screenPoint, index, 0)) {
		CViewData& view = m_viewData[index];
		switch (view.m_objectType) {
		case OBJECT_PLAYER_2:
			if (p_alternate == 0) {
				m_groupCount = 0;
				m_groupSelectionCount = 0;
				m_groupingActive = 1;
				AddObjectToGroup(m_viewData[index].m_objectId, 0);
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
		case OBJECT_MOVER:
			destination.m_x = view.m_viewX;
			destination.m_y = view.m_viewY;
			break;
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
	if ((g_pDemo == 0 || g_pDemo->m_demoMode == 0) && !m_display->IsFocusWindow()) {
		return 0;
	}
	if (m_paused != 0) {
		return 0;
	}

	switch ((unsigned int) p_message->type) {
	case 4:
	case 0xc:
		switch (p_message->code) {
		case 3:
			PrevGroup();
			return 1;
		case 4:
			NextGroup();
			return 1;
		case 0x39:
			UseBalloon(3);
			return 1;
		case 0x3a:
			SelectLemming(0);
			return 1;
		case 0x3b:
			SelectLemming(1);
			return 1;
		case 0x3c:
			SelectLemming(2);
			return 1;
		case 0x3d:
			SelectLemming(3);
			return 1;
		case 0x40:
			UseBalloon(0);
			return 1;
		case 0x41:
			UseBalloon(1);
			return 1;
		case 0x42:
			UseBalloon(2);
			return 1;
		}
		return 0;
	default:
		m_processedCount++;
		return 0;
	}
}
