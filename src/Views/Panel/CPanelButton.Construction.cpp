#include "CPanelButton.h"

#include "Gameplay/Groups/CPlayerLemmingGroup.h"
#include "Gameplay/Groups/CPlayerLemmingGroupManager.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Controls/CHotAreaList.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Views/Display/C2D.h"
#include "CPanel.h"
#include "CPanelLemming.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/CVSSize.h"
#include "Visos/Input/CBaseCursor.h"
#include "Visos/Controls/CDepressedButton.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

class CRemap;
class CResANIM;

extern char g_szButton[];

enum eAmmoCountCacheState {
	AMMO_COUNT_CACHE_UNSET = 0xffffffff
};

// FUNCTION: LEMBALL 0x00442390
CPanelButton::CPanelButton(CPanelLemming* p_lemming, const CVSRect& p_rect, CPVGWnd* p_parent)
	: CDepressedButton(p_rect, p_parent)
{
	m_lemming = p_lemming;
	{
		CVSSize size;

		size = (const CVSSize&) p_lemming->m_panel->m_ammoButtonSize;
		m_statusRect.m_width = size.m_width;
		m_statusRect.m_height = size.m_height;
		m_statusRect.m_x = 0;
		m_statusRect.m_y = 0;
	}
	m_unavailable = (unsigned int) (m_lemming->m_lemming->m_action == ACTION_DEAD);
	m_alternatePlayer = m_lemming->m_lemming->HasObject(OBJECT_FLAG_2);
	m_lastAmmo = AMMO_COUNT_CACHE_UNSET;
	m_lastBalloon = OBJECT_BALLOON_NONE;
	m_inventoryCount = 0;
	{
		CVSSize size;

		short x = m_lemming->m_panel->m_ammoButtonSize.m_x;
		size = (const CVSSize&) m_lemming->m_panel->m_lemmingButtonSize;
		m_gdiFlags += 7;
		m_inventoryRect.m_width = size.m_width;
		m_inventoryRect.m_height = size.m_height;
		m_inventoryRect.m_x = x;
		m_inventoryRect.m_y = 0;
	}
	{
		CVSRect createRect;
		createRect.CVSSize::operator=(m_bounds);
		createRect.CVSPoint::operator=(*(const CVSPoint*) &m_buttonX);
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
