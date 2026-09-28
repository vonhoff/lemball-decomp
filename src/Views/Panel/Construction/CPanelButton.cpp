#include "../CPanelButton.h"

#include "../../../AI/Groups/CPlayerLemmingGroup.h"
#include "../../../AI/Groups/CPlayerLemmingGroupManager.h"
#include "../../../AI/Navigation/CAI.h"
#include "../../../AI/Objects/CPlayerLemming.h"
#include "../../../Visos/Graphics/CBaseRemap.h"
#include "../../../Visos/Graphics/CCursor.h"
#include "../../../Visos/Graphics/CGDI.h"
#include "../../../Visos/Graphics/CHotAreaList.h"
#include "../../../Visos/Graphics/CSurface.h"
#include "../../Display/C2D.h"
#include "../../Sound/CSoundView.h"
#include "../CPanel.h"
#include "../CPanelLemming.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
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
		CVsSize size;

		size = (const CVsSize&) p_lemming->m_panel->m_buttonSize;
		m_statusRect.m_width = size.m_width;
		m_statusRect.m_height = size.m_height;
		m_statusRect.m_x = 0;
		m_statusRect.m_y = 0;
	}
	m_unavailable = (unsigned int) (m_lemming->m_lemming->m_action == ACTION_DEAD);
	m_alternatePlayer = m_lemming->m_lemming->HasObject(OBJECT_FLAG_2);
	m_lastAmmo = 0xffffffff;
	m_lastBalloon = OBJECT_BALLOON_NONE;
	m_inventoryCount = 0;
	{
		CVsSize size;

		short x = m_lemming->m_panel->m_buttonSize.m_x;
		size = (const CVsSize&) m_lemming->m_panel->m_balloonSize;
		m_gdiFlags += 7;
		m_inventoryRect.m_width = size.m_width;
		m_inventoryRect.m_height = size.m_height;
		m_inventoryRect.m_x = x;
		m_inventoryRect.m_y = 0;
	}
	{
		CVsRect createRect;
		createRect.CVsSize::operator=(m_bounds);
		createRect.CVsPoint::operator=(*(const CVsPoint*) &m_buttonX);
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
