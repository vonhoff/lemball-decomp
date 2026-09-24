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
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Graphics/CBaseCursor.h"
#include "Visos/Graphics/CDepressedButton.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CLine.h"
#include "Visos/Graphics/CPVGWnd.h"

#include <memory.h>

class CRemap;
class CResANIM;

extern char g_szButton[];

// FUNCTION: LEMBALL 0x00442390
CPanelButton::CPanelButton(CPanelLemming* p_lemming, const CVsRect& p_rect, CPVGWnd* p_parent)
	: CDepressedButton(p_rect, p_parent)
{
	m_lemming = p_lemming;
	{
		CVsPoint point;

		point = p_lemming->m_panel->m_buttonSize;
		m_statusRect.m_width = point.m_x;
		m_statusRect.m_height = point.m_y;
		m_statusRect.m_x = 0;
		m_statusRect.m_y = 0;
	}
	m_unavailable = (unsigned int) (m_lemming->m_lemming->m_action == ACTION_8);
	m_alternatePlayer = m_lemming->m_lemming->HasObject(OBJECT_FLAG_2);
	m_lastAmmo = 0xffffffff;
	m_lastBalloon = OBJECT_BALLOON_NONE;
	m_inventoryCount = 0;
	{
		CVsPoint point;

		short x = m_lemming->m_panel->m_buttonSize.m_x;
		point = m_lemming->m_panel->m_balloonSize;
		m_gdiFlags += 7;
		m_inventoryRect.m_width = point.m_x;
		m_inventoryRect.m_height = point.m_y;
		m_inventoryRect.m_x = x;
		m_inventoryRect.m_y = 0;
	}
	{
		CVsRect createRect;
		createRect.m_width = m_bounds.m_width;
		createRect.m_height = m_bounds.m_height;
		createRect.m_x = m_buttonX;
		createRect.m_y = m_buttonY;
		CGWnd* window = this;
		window->Create(createRect, m_ownerWindow, g_szButton);
	}
	m_bounds.m_x = (short) (m_bounds.m_x + m_relativeTopLeft.m_x);
	m_bounds.m_y = (short) (m_bounds.m_y + m_relativeTopLeft.m_y);
	m_ownerWindow->m_hotAreaList->AddToList(static_cast<CHotAreaHandler*>(this));
	m_gdi->m_renderTarget->m_flag70 = 0;
	m_externalEnabled = 1;
	m_pressedInside = 0;
}

// FUNCTION: LEMBALL 0x004425e0
CPanelButton::~CPanelButton()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
}

// FUNCTION: LEMBALL 0x00442670
void CPanelButton::OnInside(const CVsPoint& p_point)
{
	CursorChangeType(CURSOR_DISPLAY_HAND, m_pressedInside);
}

// FUNCTION: LEMBALL 0x00442690
void CPanelButton::DrawButton()
{
	CBaseRemap* playerRemap;
	CBaseRemap* balloonRemap = 0;
	CPanelLemming* lemming = m_lemming;
	if ((int) lemming->m_playerIndex < 4) {
		playerRemap = lemming->m_panel->m_game->m_remaps[lemming->m_playerIndex];
	}
	else {
		playerRemap = 0;
	}
	if (lemming->m_balloonType != -1) {
		if ((int) lemming->m_balloonType < 4) {
			balloonRemap = lemming->m_panel->m_game->m_remaps[lemming->m_balloonType];
		}
		else {
			balloonRemap = 0;
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
	const CVsPoint* position = (const CVsPoint*) &m_statusRect;
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
		position = (const CVsPoint*) &m_inventoryRect;
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
		position = (const CVsPoint*) &m_inventoryRect;
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
	CVsSize ammoSize;
	ammoSize.m_width = 27;
	ammoSize.m_height = 9;
	CVsPoint ammoPosition(7, 11);
	unsigned int color;
	CVsSize inventorySize;
	inventorySize.m_width = 6;
	inventorySize.m_height = 4;
	CVsPoint inventoryPosition(7, 4);
	ammoSize.m_width = (short) (ammo * ammoSize.m_width / 50);
	if (m_enabled != 0 && m_unavailable == 0) {
		color = 0x76;
	}
	else {
		color = 0x45;
		ammoPosition.m_x++;
		ammoPosition.m_y++;
		inventoryPosition.m_x++;
		inventoryPosition.m_y++;
	}
	unsigned int mappedColor;
	if (playerRemap != 0) {
		mappedColor = playerRemap->m_remap[color];
	}
	else {
		// Original fallback reads the packed ammunition size at 0x004428c1.
		memcpy(&mappedColor, &ammoSize, sizeof(mappedColor));
	}
	static_cast<CVsSize&>(m_statusLine[0].m_bounds) = ammoSize;
	static_cast<CVsPoint&>(m_statusLine[0].m_bounds) = ammoPosition;
	m_statusLine[0].m_color = mappedColor;
	m_statusLine[0].Draw(m_gdi);
	if (m_unavailable == 0) {
		CLine* line = m_inventoryLines;
		for (int i = 0; i < (int) m_lemming->m_inventoryCount; i++) {
			int type = m_lemming->m_inventoryTypes[i];
			CBaseRemap* remap;
			if (type < 4) {
				remap = m_lemming->m_panel->m_game->m_remaps[type];
			}
			else {
				remap = 0;
			}
			if (remap == 0) {
				mappedColor = color;
			}
			else {
				mappedColor = remap->m_remap[color];
			}
			static_cast<CVsSize&>(line->m_bounds) = inventorySize;
			static_cast<CVsPoint&>(line->m_bounds) = inventoryPosition;
			line->m_color = mappedColor;
			line->Draw(m_gdi);
			line++;
			inventoryPosition.m_x += 11;
		}
	}
}

// FUNCTION: LEMBALL 0x004429b0
void CPanelButton::OnPaint(const CVsRect& p_rect)
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
	if ((unsigned int) (panelLemming->m_lemming->m_action == ACTION_8) != m_unavailable) {
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
void CPanelButton::OnExternalButtonUp(const CVsPoint& p_point, int p_flags)
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
	CPlayerLemmingGroup* group;
	if (action == ACTION_8) {
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
		if ((short) m_inventoryRect.m_x <= (short) m_clickX &&
			(short) m_clickX < (short) (m_inventoryRect.m_width + m_inventoryRect.m_x)) {
			short inventoryY = m_inventoryRect.m_y;
			short clickY = m_clickY;
			if (inventoryY > clickY) {
				goto groupSelection;
			}
			if (clickY >= (short) (m_inventoryRect.m_height + inventoryY)) {
				goto groupSelection;
			}
			if (action == ACTION_NONE || action == ACTION_2 || action == ACTION_6) {
				m_lemming->m_lemming->SetSndEffect(SFX_BALLOON);
				game->UseBalloon(m_lemming->m_lemming);
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
	if (group != groupManager->GetPlayerControlledGroup()) {
		groupManager->GetPlayerControlledGroup()->SetPlayerControlled(0, 0);
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
