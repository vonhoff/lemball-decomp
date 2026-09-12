#include "PanelButton.h"

#include "../../AI/Groups/PlayerLemmingGroup.h"
#include "../../AI/Groups/PlayerLemmingGroupManager.h"
#include "../../AI/Navigation/Ai.h"
#include "../../AI/Objects/PlayerLemming.h"
#include "../../Visos/Graphics/BaseRemap.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Graphics/PvSurface.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../Display/C2D.h"
#include "../Sound/SoundView.h"
#include "Panel.h"
#include "PanelLemming.h"

#include <memory.h>

extern char g_szButton[];

// 68K 0x10b0c854 __ct__12CPanelButtonFP13CPanelLemmingRC7CVSRectP7CPVGWnd
// FUNCTION: LEMBALL 0x00442390
PanelButton::PanelButton(PanelLemming* p_arg0, const VsRect& p_arg1, PvGWnd* p_arg2) : DepressedButton(p_arg1, p_arg2)
{
	m_lemming = p_arg0;
	{
		VsPoint point;

		point = p_arg0->m_panel->m_buttonSize;
		m_statusRect.m_width = point.m_x;
		m_statusRect.m_height = point.m_y;
		m_statusRect.m_x = 0;
		m_statusRect.m_y = 0;
	}
	m_unavailable = (unsigned int) (p_arg0->m_lemming->m_action == 8);
	m_alternatePlayer = p_arg0->m_lemming->HasObject((eObjectType) 0xc);
	m_lastAmmo = 0xffffffff;
	m_lastBalloon = (eObjectType) 0xffffffff;
	m_inventoryCount = 0;
	{
		VsPoint point;

		point = p_arg0->m_panel->m_balloonSize;
		m_gdiFlags += 7;
		m_inventoryRect.m_width = point.m_x;
		m_inventoryRect.m_height = point.m_y;
		m_inventoryRect.m_x = p_arg0->m_panel->m_buttonSize.m_x;
		m_inventoryRect.m_y = 0;
	}
	{
		VsRect createRect;
		createRect.m_width = m_width;
		createRect.m_height = m_height;
		createRect.m_x = m_buttonX;
		createRect.m_y = m_buttonY;
		Create(createRect, m_ownerWindow, g_szButton);
	}
	m_x = (short) (m_x + m_relativeTopLeft.m_x);
	m_y = (short) (m_y + m_relativeTopLeft.m_y);
	m_ownerWindow->m_hotAreaList->AddToList(static_cast<HotAreaHandler*>(this));
	m_gdi->m_renderTarget->m_flag70 = 0;
	m_externalEnabled = 1;
	m_pressedInside = 0;
}

// 68K 0x10b0cb74 __dt__12CPanelButtonFv
// FUNCTION: LEMBALL 0x004425e0
PanelButton::~PanelButton()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
}

// 68K 0x10b0cc70 OnInside__12CPanelButtonFRC8CVSPoint
// FUNCTION: LEMBALL 0x00442670
void PanelButton::OnInside(const VsPoint& p_point)
{
	CursorChangeType((eCursorDisplayType) 1, m_pressedInside);
}

// 68K 0x10b0ccb6 DrawButton__12CPanelButtonFv
// FUNCTION: LEMBALL 0x00442690
void PanelButton::DrawButton()
{
	BaseRemap* playerRemap;
	BaseRemap* balloonRemap = 0;
	PanelLemming* lemming = m_lemming;
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
	if (m_depressed != 0 && m_unavailable == 0) {
		frame = 0;
	}
	else {
		frame = 1;
	}
	m_gdi->m_renderTarget->GetCurrDb();
	const VsPoint* position = (const VsPoint*) &m_statusRect;
	ResAnim* resource = m_lemming->m_panel->m_resources[1];
	m_statusAnim[0].m_x = position->m_x;
	m_statusAnim[0].m_y = position->m_y;
	m_statusAnim[0].m_animResource = resource;
	m_statusAnim[0].m_animIndex = frame;
	m_statusAnim[0].m_flags = 0;
	m_statusAnim[0].m_remap = (Remap*) playerRemap;
	m_statusAnim[0].Draw(m_gdi);
	lemming = m_lemming;
	if (lemming->m_balloonType != -1 && m_unavailable == 0) {
		position = (const VsPoint*) &m_inventoryRect;
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
		position = (const VsPoint*) &m_inventoryRect;
		resource = lemming->m_panel->m_resources[2];
		m_inventoryAnim[0].m_x = position->m_x;
		m_inventoryAnim[0].m_y = position->m_y;
		m_inventoryAnim[0].m_animResource = resource;
	}
	m_inventoryAnim[0].m_animIndex = frame;
	m_inventoryAnim[0].m_flags = 0;
	m_inventoryAnim[0].m_remap = (Remap*) balloonRemap;
	m_inventoryAnim[0].Draw(m_gdi);
	int ammo = m_lemming->m_lemming->m_ammoCount;
	VsSize ammoSize;
	ammoSize.m_width = 27;
	ammoSize.m_height = 9;
	VsPoint ammoPosition(7, 11);
	unsigned int color;
	VsSize inventorySize;
	inventorySize.m_width = 6;
	inventorySize.m_height = 4;
	VsPoint inventoryPosition(7, 4);
	ammoSize.m_width = (short) (ammo * ammoSize.m_width / 50);
	if (m_depressed != 0 && m_unavailable == 0) {
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
	*(VsSize*) &m_statusLine[0].m_x1 = ammoSize;
	*(VsPoint*) &m_statusLine[0].m_x2 = ammoPosition;
	m_statusLine[0].m_color = mappedColor;
	m_statusLine[0].Draw(m_gdi);
	if (m_unavailable == 0) {
		Line* line = m_inventoryLines;
		for (int i = 0; i < (int) m_lemming->m_inventoryCount; i++) {
			int type = m_lemming->m_inventoryTypes[i];
			BaseRemap* remap;
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
			*(VsSize*) &line->m_x1 = inventorySize;
			*(VsPoint*) &line->m_x2 = inventoryPosition;
			line->m_color = mappedColor;
			line->Draw(m_gdi);
			line++;
			inventoryPosition.m_x += 11;
		}
	}
}

// 68K 0x10b0d146 OnPaint__12CPanelButtonFRC7CVSRect
// FUNCTION: LEMBALL 0x004429b0
void PanelButton::OnPaint(const VsRect& p_rect)
{
	PanelLemming* panelLemming = m_lemming;

	if (panelLemming->m_lemming->m_ammoCount != m_lastAmmo) {
		m_forceDrawCount = 1;
		m_lastAmmo = panelLemming->m_lemming->m_ammoCount;
	}
	if (panelLemming->m_inventoryCount != m_inventoryCount) {
		m_forceDrawCount = 1;
		m_inventoryCount = panelLemming->m_inventoryCount;
	}
	if ((unsigned int) (panelLemming->m_lemming->m_action == 8) != m_unavailable) {
		m_forceDrawCount = 1;
		m_unavailable = !m_unavailable;
	}
	if ((unsigned int) panelLemming->m_lemming->HasObject((eObjectType) 0xc) != m_alternatePlayer) {
		m_forceDrawCount = 1;
		m_alternatePlayer = !m_alternatePlayer;
	}
	if (m_lemming->m_balloonType != m_lastBalloon) {
		m_forceDrawCount = 1;
		m_lastBalloon = m_lemming->m_balloonType;
	}
	DepressedButton::OnPaint(p_rect);
}

// 68K 0x10b0d262 OnReleased__12CPanelButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442aa0
void PanelButton::OnReleased(int p_flags)
{
	m_pressedInside = 0;
	CursorChangeType((eCursorDisplayType) 1, 0);
}

// 68K 0x10b0d2b2 OnExternalButtonUp__12CPanelButtonFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442ac0
void PanelButton::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
{
	if (m_pressedInside != 0) {
		m_pressedInside = 0;
		CursorChangeType((eCursorDisplayType) 1, 0);
	}
}

// 68K 0x10b0d320 OnPressed__12CPanelButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442ae0
void PanelButton::OnPressed(int p_flags)
{
	PanelLemming* panelLemming = m_lemming;
	C2D* game = panelLemming->m_panel->m_game;

	if (game->m_paused != 0) {
		return;
	}
	PlayerLemming* lemming = panelLemming->m_lemming;
	eAction action = lemming->m_action;
	PlayerLemmingGroupManager* groupManager;
	PlayerLemmingGroup* group;
	if (action == 8) {
		return;
	}

	if (p_flags == 0) {
		goto normal;
	}
	if (p_flags == 1) {
		goto alternate;
	}
	goto pressed;

normal:
	if (panelLemming->m_balloonType != -1) {
		if ((short) m_inventoryRect.m_x <= (short) m_clickX &&
			(short) m_clickX < (short) (m_inventoryRect.m_width + m_inventoryRect.m_x)) {
			short inventoryY = m_inventoryRect.m_y;
			short clickY = m_clickY;
			if (inventoryY > clickY) {
				goto pressed;
			}
			if (clickY >= (short) (m_inventoryRect.m_height + inventoryY)) {
				goto pressed;
			}
			if (action == 0 || action == 2 || action == 6) {
				m_lemming->m_lemming->SetSndEffect((eSoundEffect) 0x1f);
				game->UseBalloon(m_lemming->m_lemming);
			}
			goto pressed;
		}
	}
	if (game->m_groupingActive == 1) {
		if (game->InGroupByObjectNo(m_lemming->m_lemming->m_objectId) == 0) {
			game->AddObjectToGroup(m_lemming->m_lemming->m_objectId, 1);
		}
		else {
			game->RemoveFromGroupByObjectNo(m_lemming->m_lemming->m_objectId);
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
	CursorChangeType((eCursorDisplayType) 1, 1);
	g_pSoundView->m_pendingEffect = (eSoundEffect) 3;
}

// 68K 0x1011cde6 OnEnterButton__12CPanelButtonFv
// FUNCTION: LEMBALL 0x00443930
void PanelButton::OnEnterButton()
{
}

// 68K 0x1011ce10 OnExitButton__12CPanelButtonFv
// FUNCTION: LEMBALL 0x00443940
void PanelButton::OnExitButton()
{
}
