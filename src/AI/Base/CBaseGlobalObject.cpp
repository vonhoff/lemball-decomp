#include "CBaseGlobalObject.h"

#include "../../Visos/Network/CConnect.h"
#include "../Messages/CObjectPosMess.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"

// FUNCTION: LEMBALL 0x0041c670
void CBaseGlobalObject::OldRestart()
{
	m_position.m_xFixed = m_initialPosition.m_xFixed;
	m_position.m_yFixed = m_initialPosition.m_yFixed;
	m_objectActive = 1;
	m_position.m_zFixed = m_initialPosition.m_zFixed;
	if (g_pActiveConnection != 0) {
		g_pObjectPosMessage->Send(this);
	}
}

// FUNCTION: LEMBALL 0x0041c6c0
void CBaseGlobalObject::Restart()
{
	CGlobalGameObject::Restart();
	m_position.m_xFixed = m_initialPosition.m_xFixed;
	m_position.m_yFixed = m_initialPosition.m_yFixed;
	m_position.m_zFixed = m_initialPosition.m_zFixed;
	m_action = ACTION_0x18;
}
