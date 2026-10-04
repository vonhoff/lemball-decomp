#include "CBaseGlobalObject.h"

#include "Multiplayer/Transport/CConnect.h"
#include "Gameplay/Messages/CObjectPosMess.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "CGlobalGameObject.h"
#include "ObjectActions.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0041c670
void CBaseGlobalObject::OldRestart()
{
	m_position.m_xFixed = m_initialPosition.m_xFixed;
	m_position.m_yFixed = m_initialPosition.m_yFixed;
	m_objectActive = 1;
	m_position.m_zFixed = m_initialPosition.m_zFixed;
	if (g_pActiveConnection != NULL) {
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
	m_action = ACTION_READY;
}
