#include "BaseGlobalObject.h"

#include "../../Visos/Network/Connect.h"
#include "../Messages/ObjectPosMess.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/GlobalGameObject.h"
#include "AI/Base/ObjectActions.h"

// 68K 0x10618974 OldRestart__17CBaseGlobalObjectFv
// FUNCTION: LEMBALL 0x0041c670
void BaseGlobalObject::OldRestart()
{
	m_position.m_xFixed = m_initialPosition.m_xFixed;
	m_position.m_yFixed = m_initialPosition.m_yFixed;
	m_heading = 1;
	m_position.m_zFixed = m_initialPosition.m_zFixed;
	if (g_pActiveConnection != 0) {
		g_pObjectPosMessage->Send(this);
	}
}

// 68K 0x106189ea Restart__17CBaseGlobalObjectFv
// FUNCTION: LEMBALL 0x0041c6c0
void BaseGlobalObject::Restart()
{
	GlobalGameObject::Restart();
	m_position.m_xFixed = m_initialPosition.m_xFixed;
	m_position.m_yFixed = m_initialPosition.m_yFixed;
	m_position.m_zFixed = m_initialPosition.m_zFixed;
	m_action = (eAction) 0x18;
}
