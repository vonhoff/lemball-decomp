#include "AI/Objects/CPlayerLemming.h"

#include "AI/Objects/CViewData.h"

// FUNCTION: LEMBALL 0x004108b0
void CPlayerLemming::GetViewData(CViewData& p_viewData)
{
	CGameObject::GetViewData(p_viewData);
	int flags = (m_isGroupLeader != 0 ? 1 : 0) | (m_groupIndex != 0 ? 2 : 0);
	p_viewData.m_statusFlags = flags;
	p_viewData.m_playerIndex = m_playerIndex;
}
