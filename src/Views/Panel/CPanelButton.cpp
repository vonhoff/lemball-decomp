#include "CPanelButton.h"

#include "../../AI/Groups/CPlayerLemmingGroup.h"
#include "../../AI/Groups/CPlayerLemmingGroupManager.h"
#include "../../AI/Navigation/CAI.h"
#include "../../AI/Objects/CPlayerLemming.h"
#include "../../Visos/Graphics/CBaseRemap.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CHotAreaList.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../Display/C2D.h"
#include "../Sound/CSoundView.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "CPanel.h"
#include "CPanelLemming.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnim.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/CVSSize.h"
#include "Visos/Graphics/CBaseCursor.h"
#include "Visos/Graphics/CDepressedButton.h"
#include "Visos/Graphics/CPVGWnd.h"
#include "Visos/Graphics/CSolidRect.h"

class CRemap;
class CResANIM;

extern char g_szButton[];

// FUNCTION: LEMBALL 0x004425e0
CPanelButton::~CPanelButton()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
}

// FUNCTION: LEMBALL 0x00442670
void CPanelButton::OnInside(const CVSPoint& p_point)
{
	CursorChangeType(CURSOR_DISPLAY_HAND, m_pressedInside);
}

// FUNCTION: LEMBALL 0x00442690
void CPanelButton::DrawButton()
{
	CBaseRemap* playerRemap;
	CBaseRemap* balloonRemap = NULL;
	CPanelLemming* lemming = m_lemming;
	if ((int) lemming->m_playerIndex < 4) {
		playerRemap = lemming->m_panel->m_game->m_remaps[lemming->m_playerIndex];
	}
	else {
		playerRemap = NULL;
	}
	if (lemming->m_balloonType != -1) {
		if ((int) lemming->m_balloonType < 4) {
			balloonRemap = lemming->m_panel->m_game->m_remaps[lemming->m_balloonType];
		}
		else {
			balloonRemap = NULL;
		}
	}
	unsigned int frame;
	if (m_enabled != 0 && m_unavailable == 0) {
		frame = 0;
	}
	else {
		frame = 1;
	}
	m_gdi->m_renderTarget->GetCurrDB();
	const CVSPoint* position = (const CVSPoint*) &m_statusRect;
	CResANIM* resource = m_lemming->m_panel->m_resources[1];
	m_statusAnim[0].m_x = position->m_x;
	m_statusAnim[0].m_y = position->m_y;
	m_statusAnim[0].m_animResource = resource;
	m_statusAnim[0].m_animIndex = frame;
	m_statusAnim[0].m_flags = 0;
	m_statusAnim[0].m_remap = (CRemap*) playerRemap;
	m_statusAnim[0].Draw(m_gdi);
	lemming = m_lemming;
	if (lemming->m_balloonType != -1 && m_unavailable == 0) {
		CResANIM* resource;
		const CVSPoint* position = (const CVSPoint*) &m_inventoryRect;
		resource = lemming->m_panel->m_resources[3];
		m_inventoryAnim[0].m_x = position->m_x;
		m_inventoryAnim[0].m_y = position->m_y;
		m_inventoryAnim[0].m_animResource = resource;
	}
	else {
		if (m_unavailable != 0) {
			frame = 2;
		}
		if (m_alternatePlayer != 0) {
			frame += 3;
		}
		CResANIM* resource;
		const CVSPoint* position = (const CVSPoint*) &m_inventoryRect;
		resource = lemming->m_panel->m_resources[2];
		m_inventoryAnim[0].m_x = position->m_x;
		m_inventoryAnim[0].m_y = position->m_y;
		m_inventoryAnim[0].m_animResource = resource;
	}
	m_inventoryAnim[0].m_animIndex = frame;
	m_inventoryAnim[0].m_flags = 0;
	m_inventoryAnim[0].m_remap = (CRemap*) balloonRemap;
	m_inventoryAnim[0].Draw(m_gdi);
	int ammo = m_lemming->m_lemming->m_ammoCount;
	CVSRect ammoRect(7, 11, 27, 9);
	CVSSize& ammoSize = ammoRect;
	CVSPoint& ammoPosition = ammoRect;
	unsigned int colour;
	CVSRect inventoryRect(7, 4, 6, 4);
	CVSSize& inventorySize = inventoryRect;
	CVSPoint& inventoryPosition = inventoryRect;
	ammoSize.m_width = (short) (ammo * ammoSize.m_width / 50);
	if (m_enabled != 0 && m_unavailable == 0) {
		colour = 0x76;
	}
	else {
		colour = 0x45;
		ammoPosition.m_x++;
		ammoPosition.m_y++;
		inventoryPosition.m_x++;
		inventoryPosition.m_y++;
	}
	unsigned int mappedColour;
	if (playerRemap != NULL) {
		mappedColour = playerRemap->m_remap[colour];
	}
	else {
		mappedColour = *(const unsigned int*) &ammoSize;
	}
	static_cast<CVSSize&>(m_statusLine[0].m_bounds) = ammoSize;
	m_statusLine[0].m_bounds.CVSPoint::operator=(ammoPosition);
	m_statusLine[0].m_colour = mappedColour;
	m_statusLine[0].Draw(m_gdi);
	if (m_unavailable == 0) {
		for (int i = 0; i < (int) m_lemming->m_inventoryCount; i++) {
			CSolidRect* line = &m_inventoryLines[i];
			int type = m_lemming->m_inventoryTypes[i];
			CBaseRemap* remap;
			if (type < 4) {
				remap = m_lemming->m_panel->m_game->m_remaps[type];
			}
			else {
				remap = NULL;
			}
			if (remap == NULL) {
				mappedColour = colour;
			}
			else {
				mappedColour = remap->m_remap[colour];
			}
			static_cast<CVSSize&>(line->m_bounds) = inventorySize;
			line->m_bounds.CVSPoint::operator=(inventoryPosition);
			line->m_colour = mappedColour;
			line->Draw(m_gdi);
			inventoryPosition.m_x += 11;
		}
	}
}

// FUNCTION: LEMBALL 0x004429b0
void CPanelButton::OnPaint(const CVSRect& p_rect)
{
	CPanelLemming* panelLemming = m_lemming;

	if (panelLemming->m_lemming->m_ammoCount != m_lastAmmo) {
		m_forceDrawCount = 1;
		m_lastAmmo = panelLemming->m_lemming->m_ammoCount;
	}
	if (panelLemming->m_inventoryCount != m_inventoryCount) {
		m_forceDrawCount = 1;
		m_inventoryCount = panelLemming->m_inventoryCount;
	}
	if ((unsigned int) (panelLemming->m_lemming->m_action == ACTION_DEAD) != m_unavailable) {
		m_forceDrawCount = 1;
		m_unavailable = !m_unavailable;
	}
	if ((unsigned int) panelLemming->m_lemming->HasObject(OBJECT_FLAG_2) != m_alternatePlayer) {
		m_forceDrawCount = 1;
		m_alternatePlayer = !m_alternatePlayer;
	}
	if (m_lemming->m_balloonType != m_lastBalloon) {
		m_forceDrawCount = 1;
		m_lastBalloon = m_lemming->m_balloonType;
	}
	CDepressedButton::OnPaint(p_rect);
}

// FUNCTION: LEMBALL 0x00442aa0
void CPanelButton::OnReleased(int p_flags)
{
	m_pressedInside = 0;
	CursorChangeType(CURSOR_DISPLAY_HAND, 0);
}

// FUNCTION: LEMBALL 0x00442ac0
void CPanelButton::OnExternalButtonUp(const CVSPoint& p_point, int p_flags)
{
	if (m_pressedInside != 0) {
		m_pressedInside = 0;
		CursorChangeType(CURSOR_DISPLAY_HAND, 0);
	}
}

// FUNCTION: LEMBALL 0x00442ae0
void CPanelButton::OnPressed(int p_flags)
{
	CPanelLemming* panelLemming = m_lemming;
	C2D* game = panelLemming->m_panel->m_game;

	if (game->m_paused != 0) {
		return;
	}
	CPlayerLemming* lemming = panelLemming->m_lemming;
	eAction action = lemming->m_action;
	CPlayerLemmingGroupManager* groupManager;
	CPlayerLemmingGroup* controlledGroup;
	CPlayerLemmingGroup* group;
	if (action == ACTION_DEAD) {
		return;
	}

	switch (p_flags) {
	case 0:
		goto normal;
	case 1:
		goto alternate;
	default:
		goto pressed;
	}

normal:
	if (panelLemming->m_balloonType != OBJECT_BALLOON_NONE) {
		if (m_inventoryRect.m_x <= m_clickX && m_clickX < (short) (m_inventoryRect.m_width + m_inventoryRect.m_x)) {
			short inventoryY = m_inventoryRect.m_y;
			short clickY = m_clickY;
			if (inventoryY > clickY) {
				goto groupSelection;
			}
			if (clickY >= (short) (m_inventoryRect.m_height + inventoryY)) {
				goto groupSelection;
			}
			if (action == ACTION_NONE || action == ACTION_WALKING || action == ACTION_IDLE_ANIMATION) {
				m_lemming->m_lemming->SetSndEffect(SFX_BALLOON);
				m_lemming->m_panel->m_game->UseBalloon(m_lemming->m_lemming);
			}
			goto pressed;
		}
	}
groupSelection:
	if (game->m_groupingActive == 1) {
		if (game->InGroupByObjectNo(m_lemming->m_lemming->m_objectId) != 0) {
			game->RemoveFromGroupByObjectNo(m_lemming->m_lemming->m_objectId);
		}
		else {
			game->AddObjectToGroup(m_lemming->m_lemming->m_objectId, 1);
		}
	}
	else {
		game->m_groupingActive = 1;
		game->m_groupCount = 0;
		game->m_groupSelectionCount = 0;
		game->AddObjectToGroup(m_lemming->m_lemming->m_objectId, 1);
	}
	m_lemming->UpdateStatus();
	goto pressed;

alternate:
	groupManager = game->m_ai->m_playerGroupManager;
	game->FormGroup();
	group = m_lemming->m_lemming->GetGroup();
	controlledGroup = groupManager->GetPlayerControlledGroup();
	if (group != controlledGroup) {
		groupManager->GetPlayerControlledGroup()->SetPlayerControlled(0, NULL);
	}
	m_lemming->m_lemming->GetGroup()->SetPlayerControlled(1, m_lemming->m_lemming);

pressed:
	m_pressedInside = 1;
	CursorChangeType(CURSOR_DISPLAY_HAND, 1);
	g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
}

// FUNCTION: LEMBALL 0x00443930
void CPanelButton::OnEnterButton()
{
}

// FUNCTION: LEMBALL 0x00443940
void CPanelButton::OnExitButton()
{
}
