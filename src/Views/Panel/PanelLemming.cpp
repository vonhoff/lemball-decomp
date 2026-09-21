#include "PanelLemming.h"

#include "../../AI/Groups/PlayerLemmingGroupManager.h"
#include "../../AI/Navigation/Ai.h"
#include "../../AI/Objects/PlayerLemming.h"
#include "../Display/C2D.h"
#include "../Display/Main2DDisplay.h"
#include "AI/Base/ObjectTypes.h"
#include "Panel.h"
#include "PanelButton.h"
#include "Visos/Foundation/VsPoint.h"
#include "Visos/Foundation/VsRect.h"

class PlayerLemmingGroup;

// 68K 0x10b0d5c6 Move__13CPanelLemmingFRC8CVSPoint
// FUNCTION: LEMBALL 0x00442ca0
void PanelLemming::Move(const VsPoint& p_position)
{
	VsPoint position = p_position;
	position.m_x += (m_panel->m_balloonSize.m_x + m_panel->m_buttonSize.m_x) * (short) m_playerIndex;
	m_button->Move(position);
}

// 68K 0x10b0d654 __ct__13CPanelLemmingFP14CPlayerLemmingRC8CVSPointP6CPanel
// FUNCTION: LEMBALL 0x00442ce0
PanelLemming::PanelLemming(PlayerLemming* p_arg0, const VsPoint& p_arg1, Panel* p_arg2)
{
	m_panel = p_arg2;
	m_window = p_arg2->m_game->m_display;
	m_lemming = p_arg0;
	VsRect rect;
	m_playerIndex = p_arg0->m_playerIndex;
	m_balloonType = OBJECT_BALLOON_NONE;
	m_inventoryCount = 0;

	rect.m_x = p_arg1.m_x;
	rect.m_y = p_arg1.m_y;
	rect.m_x += (p_arg2->m_balloonSize.m_x + p_arg2->m_buttonSize.m_x) * m_playerIndex;
	rect.m_width = m_panel->m_balloonSize.m_x + m_panel->m_buttonSize.m_x;
	rect.m_height = m_panel->m_balloonSize.m_y;
	m_button = new PanelButton(this, rect, m_window);
}

// 68K 0x10b0d77a __dt__13CPanelLemmingFv
// FUNCTION: LEMBALL 0x00442db0
PanelLemming::~PanelLemming()
{
	delete m_button;
}

// 68K 0x10b0d7d8 UpdateStatus__13CPanelLemmingFv
// FUNCTION: LEMBALL 0x00442dd0
void PanelLemming::UpdateStatus()
{
	unsigned int selected;
	PlayerLemming* lemming = m_lemming;

	if (lemming->m_action == ACTION_8) {
		selected = 0;
	}
	else {
		C2D* game = m_panel->m_game;
		if (game->m_groupingActive != 1) {
			PlayerLemmingGroup* controlled = game->m_ai->m_playerGroupManager->GetPlayerControlledGroup();
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
