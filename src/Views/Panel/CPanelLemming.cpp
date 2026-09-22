#include "CPanelLemming.h"

#include "../../AI/Groups/CPlayerLemmingGroupManager.h"
#include "../../AI/Navigation/CAI.h"
#include "../../AI/Objects/CPlayerLemming.h"
#include "../Display/C2D.h"
#include "../Display/CMain2DDisplay.h"
#include "AI/Base/ObjectTypes.h"
#include "CPanel.h"
#include "CPanelButton.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"

class CPlayerLemmingGroup;

// FUNCTION: LEMBALL 0x00442ca0
void CPanelLemming::Move(const CVsPoint& p_position)
{
	CVsPoint position = p_position;
	position.m_x += (m_panel->m_balloonSize.m_x + m_panel->m_buttonSize.m_x) * (short) m_playerIndex;
	m_button->Move(position);
}

// FUNCTION: LEMBALL 0x00442ce0
CPanelLemming::CPanelLemming(CPlayerLemming* p_lemming, const CVsPoint& p_position, CPanel* p_panel)
{
	m_panel = p_panel;
	m_window = p_panel->m_game->m_display;
	m_lemming = p_lemming;
	CVsRect rect;
	m_playerIndex = p_lemming->m_playerIndex;
	m_balloonType = OBJECT_BALLOON_NONE;
	m_inventoryCount = 0;

	rect.m_x = p_position.m_x;
	rect.m_y = p_position.m_y;
	rect.m_x += (p_panel->m_balloonSize.m_x + p_panel->m_buttonSize.m_x) * m_playerIndex;
	rect.m_width = m_panel->m_balloonSize.m_x + m_panel->m_buttonSize.m_x;
	rect.m_height = m_panel->m_balloonSize.m_y;
	m_button = new CPanelButton(this, rect, m_window);
}

// FUNCTION: LEMBALL 0x00442db0
CPanelLemming::~CPanelLemming()
{
	delete m_button;
}

// FUNCTION: LEMBALL 0x00442dd0
void CPanelLemming::UpdateStatus()
{
	unsigned int selected;
	CPlayerLemming* lemming = m_lemming;

	if (lemming->m_action == ACTION_8) {
		selected = 0;
	}
	else {
		C2D* game = m_panel->m_game;
		if (game->m_groupingActive != 1) {
			CPlayerLemmingGroup* controlled = game->m_ai->m_playerGroupManager->GetPlayerControlledGroup();
			selected = lemming->GetGroup() == controlled;
		}
		else {
			selected = game->IsInGrouping(lemming);
		}
	}

	switch (m_lemming->GetLastBalloon()) {
	case OBJECT_BALLOON_0:
		m_balloonType = OBJECT_BULLET;
		break;
	case OBJECT_BALLOON_2:
		m_balloonType = OBJECT_PLAYER_1;
		break;
	case OBJECT_BALLOON_4:
		m_balloonType = OBJECT_CATAPULT;
		break;
	case OBJECT_BALLOON_6:
		m_balloonType = OBJECT_NONE;
		break;
	default:
		m_balloonType = OBJECT_BALLOON_NONE;
		break;
	}

	m_inventoryCount = 0;
	for (int i = 0; i < (int) m_lemming->m_inventoryCount; i++) {
		switch (m_lemming->GetObject(i)) {
		case OBJECT_KEY_1:
			m_inventoryTypes[m_inventoryCount] = OBJECT_BULLET;
			m_inventoryCount++;
			break;
		case OBJECT_KEY_2:
			m_inventoryTypes[m_inventoryCount] = OBJECT_PLAYER_1;
			m_inventoryCount++;
			break;
		case OBJECT_KEY_3:
			m_inventoryTypes[m_inventoryCount] = OBJECT_CATAPULT;
			m_inventoryCount++;
			break;
		}
	}

	m_button->m_enabled = selected;
}
