#include "CRocket.h"

#include "Game/CGame.h"
#include "Map/CMap.h"
#include "Map/CGround.h"
#include "Gameplay/Geometry/CVSMath.h"

#include <stddef.h>

enum {
	ROCKET_ACTIVATION_RADIUS_PIXELS = 32,
	ROCKET_ACTIVATION_POSITION_XY_OFFSET_FIXED = 4 * FIXED_POINT_ONE
};

enum {
	ROCKET_LAUNCH_MOVEMENT_DELAY_TICKS = 48,
	ROCKET_ACTIVATOR_DEATH_DELAY_TICKS = 60
};

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
void CRocket::Set(unsigned short p_id, const AICOORD& p_position)
{
	SetId(p_id);
	m_position = p_position;
	m_active = 1;
	m_action = ACTION_READY;
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int blockX = x / GROUND_BLOCK_PIXEL_SIZE;
	if (blockX >= 0) {
		int blockY = y / GROUND_BLOCK_PIXEL_SIZE;
		if (blockY < 0) {
			return;
		}
		int width = g_pMap->m_ground.m_width;
		if (blockX < width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
		}
	}
}

#include "Game/GameTime.h"

// FUNCTION: LEMBALL 0x004268e0
bool CRocket::Process()
{
	unsigned long tick;
	unsigned int remoteObject = m_isRemoteObject;
	if (remoteObject != 0) {
		tick = g_dwRemoteGameTick;
	}
	else {
		tick = g_dwGameTick;
	}
	eAction action = m_action;
	if (action == ACTION_FLYING) {
		const unsigned long& height = ((tick - m_lastMovementTick) * 10 + m_launchBaseZ) << FIXED_POINT_FRACTION_BITS;
		m_position.m_zFixed = height;
	}

	if (remoteObject != 0) {
		if (m_pendingAction != action) {
			if (action == ACTION_RUNNING) {
				m_lastMovementTick = tick + ROCKET_LAUNCH_MOVEMENT_DELAY_TICKS;
				m_launchBaseZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
				SetSndEffect(SFX_ROCKET);
			}
			m_pendingAction = m_action;
		}
		return true;
	}

	switch (action) {
	case ACTION_FLYING:
		if ((m_position.m_zFixed & FIXED_POINT_INTEGER_MASK) > 200 * FIXED_POINT_ONE) {
			Action(ACTION_READY);
			return true;
		}
		break;
	case ACTION_RUNNING:
		if (m_lastMovementTick < g_dwGameTick) {
			Action(ACTION_FLYING);
		}
		break;
	default:
		return true;
	}
	return true;
}

// FUNCTION: LEMBALL 0x004269d0
int CRocket::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	if ((int) Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) < ROCKET_ACTIVATION_RADIUS_PIXELS) {
		m_position.m_xFixed = p_position.m_xFixed + ROCKET_ACTIVATION_POSITION_XY_OFFSET_FIXED;
		m_position.m_yFixed = p_position.m_yFixed + ROCKET_ACTIVATION_POSITION_XY_OFFSET_FIXED;
		m_position.m_zFixed = p_position.m_zFixed;
		m_launchBaseZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		m_activator = p_object;
		m_lastMovementTick = ROCKET_LAUNCH_MOVEMENT_DELAY_TICKS;
		RequestAction(ACTION_RUNNING);
		return 1;
	}
	return 0;
}

#include "Visos/Network/CConnect.h"
#include "Gameplay/Messages/CObjectPosMess.h"

// FUNCTION: LEMBALL 0x00426a60
void CRocket::DoActivate()
{
	m_lastMovementTick += g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
	m_activator->Action(ACTION_WAITING_TO_DIE);
	m_activator->m_actionDeadline = g_dwGameTick + ROCKET_ACTIVATOR_DEATH_DELAY_TICKS;
	SetSndEffect(SFX_ROCKET);
	if (g_pActiveConnection != NULL) {
		g_pObjectPosMessage->Send(this);
	}
}

#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Objects/CViewData.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x004273f0
void CRocket::GetViewData(CViewData& p_viewData)
{
	p_viewData.m_objectId = m_objectId;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_positionX = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_positionY = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_positionZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int argument = (unsigned short) m_actionArgument;
	unsigned int timer = m_stateTimer;
	eAction action = m_action;
	p_viewData.m_actionArgument = argument;
	p_viewData.m_action = action;
	p_viewData.m_stateTimer = timer;
	p_viewData.m_statusFlags = 0;
	p_viewData.m_hidden = m_hidden;
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
