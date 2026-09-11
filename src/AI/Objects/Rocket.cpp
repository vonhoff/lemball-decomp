#include "Rocket.h"

#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/VsMath.h"

// 68K 0x1061d8ce __ct__7CRocketFv
// FUNCTION: LEMBALL 0x004267d0
Rocket::Rocket() : GlobalGameObject(0x20, 0, 0)
{
}

// 68K 0x1061d910 Initialise__7CRocketFv
// FUNCTION: LEMBALL 0x004267f0
void Rocket::Initialise()
{
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// 68K 0x1061d942 Restart__7CRocketFv
// FUNCTION: LEMBALL 0x00426810
void Rocket::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x1061d9cc Set__7CRocketFUsRC7AICOORD
// FUNCTION: LEMBALL 0x00426840
void Rocket::Set(unsigned short p_id, const AiCoord& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	int z = p_position.m_zFixed;
	m_active = 1;
	m_action = 0x18;
	m_position.m_zFixed = z;
	int x = p_position.m_xFixed >> 12;
	int y = p_position.m_yFixed >> 12;
	int blockX = x / 16;
	if (blockX >= 0) {
		int blockY = y / 16;
		if (blockY < 0) {
			return;
		}
		int width = g_pMap->m_ground.m_width;
		if (blockX < width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= 0x8000;
		}
	}
}

#include "../../Control/Game/GameTime.h"

// 68K 0x1061da76 Process__7CRocketFv
// FUNCTION: LEMBALL 0x004268e0
bool Rocket::Process()
{
	unsigned int remoteObject = m_isRemoteObject;
	unsigned long tick;
	if (remoteObject != 0) {
		tick = g_dwRemoteGameTick;
	}
	else {
		tick = g_dwGameTick;
	}
	eAction action = m_action;
	if (action == (eAction) 4) {
		m_position.m_zFixed = ((tick - m_lastMovementTick) * 10 + m_launchBaseZ) << 12;
	}

	if (remoteObject != 0) {
		if (m_pendingAction != action) {
			if (action == (eAction) 27) {
				m_lastMovementTick = tick + 48;
				m_launchBaseZ = m_position.m_zFixed >> 12;
				SetSndEffect((eSoundEffect) 18);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}

	switch (action) {
	case (eAction) 4:
		if ((m_position.m_zFixed & -4096) > 0xc8000) {
			Action((eAction) 24);
			return 1;
		}
		break;
	case (eAction) 27:
		if (m_lastMovementTick < g_dwGameTick) {
			Action((eAction) 4);
		}
		break;
	default:
		return 1;
	}
	return 1;
}

// 68K 0x1061db8c StepOn__7CRocketFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x004269d0
int Rocket::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	if ((int) Distance(m_position.m_xFixed >> 12,
					   m_position.m_yFixed >> 12,
					   p_position.m_xFixed >> 12,
					   p_position.m_yFixed >> 12) < 32) {
		m_position.m_xFixed = p_position.m_xFixed + 0x4000;
		m_position.m_yFixed = p_position.m_yFixed + 0x4000;
		m_position.m_zFixed = p_position.m_zFixed;
		m_launchBaseZ = m_position.m_zFixed >> 12;
		m_activator = p_object;
		m_lastMovementTick = 0x30;
		RequestAction((eAction) 0x1b);
		return 1;
	}
	return 0;
}

#include "../../Visos/Network/Connect.h"
#include "../Messages/ObjectPosMess.h"

// 68K 0x1061dc6a DoActivate__7CRocketFv
// FUNCTION: LEMBALL 0x00426a60
void Rocket::DoActivate()
{
	m_lastMovementTick += g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
	m_activator->Action((eAction) 21);
	m_activator->m_actionDeadline = g_dwGameTick + 60;
	SetSndEffect((eSoundEffect) 18);
	if (g_pActiveConnection != 0) {
		g_pObjectPosMessage->Send(this);
	}
}

// 68K 0x1011b220 GetViewData__7CRocketFR9CViewData
// STUB: LEMBALL 0x004273f0
void Rocket::GetViewData(ViewData& p_viewData)
{
}

// 68K 0x1061d972 __dt__7CRocketFv
Rocket::~Rocket()
{
}
