#include "CRocket.h"

#include "../../Control/Game/CGame.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVSMath.h"

// FUNCTION: LEMBALL 0x004267d0
CRocket::CRocket() : CGlobalGameObject(OBJECT_ROCKET, 0, 0)
{
}

// FUNCTION: LEMBALL 0x004267f0
void CRocket::Initialise()
{
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// FUNCTION: LEMBALL 0x00426810
void CRocket::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x00426830
CRocket::~CRocket()
{
}

// FUNCTION: LEMBALL 0x00426840
void CRocket::Set(unsigned short p_id, const AiCoord& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	int z = p_position.m_zFixed;
	m_active = 1;
	m_action = ACTION_0x18;
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

// FUNCTION: LEMBALL 0x004268e0
bool CRocket::Process()
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
	if (action == ACTION_4) {
		m_position.m_zFixed = ((tick - m_lastMovementTick) * 10 + m_launchBaseZ) << 12;
	}

	if (remoteObject != 0) {
		if (m_pendingAction != action) {
			if (action == ACTION_0x1b) {
				m_lastMovementTick = tick + 48;
				m_launchBaseZ = m_position.m_zFixed >> 12;
				SetSndEffect(SFX_ROCKET);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}

	switch (action) {
	case ACTION_4:
		if ((m_position.m_zFixed & -4096) > 0xc8000) {
			Action(ACTION_0x18);
			return 1;
		}
		break;
	case ACTION_0x1b:
		if (m_lastMovementTick < g_dwGameTick) {
			Action(ACTION_4);
		}
		break;
	default:
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004269d0
int CRocket::StepOn(const AiCoord& p_position, CGameObject* p_object)
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
		RequestAction(ACTION_0x1b);
		return 1;
	}
	return 0;
}

#include "../../Visos/Network/CConnect.h"
#include "../Messages/CObjectPosMess.h"

// FUNCTION: LEMBALL 0x00426a60
void CRocket::DoActivate()
{
	m_lastMovementTick += g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
	m_activator->Action(ACTION_0x15);
	m_activator->m_actionDeadline = g_dwGameTick + 60;
	SetSndEffect(SFX_ROCKET);
	if (g_pActiveConnection != 0) {
		g_pObjectPosMessage->Send(this);
	}
}

#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "CViewData.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x004273f0
void CRocket::GetViewData(CViewData& p_viewData)
{
	p_viewData.m_objectId = m_objectId;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_positionX = m_position.m_xFixed >> 12;
	p_viewData.m_positionY = m_position.m_yFixed >> 12;
	p_viewData.m_positionZ = m_position.m_zFixed >> 12;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int argument = (unsigned short) m_actionArgument;
	unsigned int timer = m_stateTimer;
	eAction action = m_action;
	p_viewData.m_actionArgument = argument;
	p_viewData.m_action = action;
	p_viewData.m_stateTimer = timer;
	p_viewData.m_statusFlags = 0;
	p_viewData.m_unk0x30 = m_unk0xc0;
	p_viewData.m_auxiliaryPosition.m_xFixed = m_auxiliaryPosition.m_xFixed;
	p_viewData.m_auxiliaryPosition.m_yFixed = m_auxiliaryPosition.m_yFixed;
	p_viewData.m_auxiliaryPosition.m_zFixed = m_auxiliaryPosition.m_zFixed;
	p_viewData.m_soundEffect = m_soundEffect;
	unsigned long timestamp;
	if (m_isRemoteObject != 0) {
		timestamp = g_dwNetworkSimulationTimestamp;
	}
	else {
		timestamp = g_dwSimulationTimestamp;
	}
	p_viewData.m_animationTime = timestamp;
	SetSndEffect(SFX_NONE);
	p_viewData.m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
}
