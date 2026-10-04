#include "CNetworkOptionsDrawer.h"

#include "Frontend/CBaseFrontendDrawer.h"
#include "Visos/Math/CVSRect.h"

// FUNCTION: LEMBALL 0x004536b0
void CNetworkOptionsDrawer::DrawFrame(int p_position)
{
	NetworkOptionsFramePos* pos = &m_layoutTable->m_framePos[p_position];
	CBaseFrontendDrawer::DrawFrame(CVSRect(pos[0].m_x, pos[0].m_y, pos[1].m_x, pos[1].m_y));
}
