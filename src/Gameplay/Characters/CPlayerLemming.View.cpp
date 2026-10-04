#include "CPlayerLemming.h"

#include "Gameplay/Objects/CViewData.h"

// FUNCTION: LEMBALL 0x004108b0
void CPlayerLemming::GetViewData(CViewData& p_viewData)
{
	CGameObject::GetViewData(p_viewData);
	int flags = (m_isGroupLeader != 0 ? LEMMING_VIEW_STATUS_GROUP_LEADER : 0) |
				(m_groupIndex != 0 ? LEMMING_VIEW_STATUS_IN_GROUP : 0);
	p_viewData.m_statusFlags = flags;
	p_viewData.m_playerIndex = m_playerIndex;
}
