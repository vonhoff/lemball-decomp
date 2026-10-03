#include "CHand.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x00427ad0
CHand::CHand() : CGlobalGameObject(OBJECT_HAND, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00427af0
void CHand::Initialise()
{
	m_stateTimer = 0;
	m_activated = 0;
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x00427b10
void CHand::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
}

// FUNCTION: LEMBALL 0x00427b40
CHand::~CHand()
{
}

// FUNCTION: LEMBALL 0x00427b50
void CHand::Set(unsigned short p_id, const AICOORD& p_position)
{
	SetId(p_id);
	m_spawnPosition.m_xFixed = p_position.m_xFixed;
	m_spawnPosition.m_yFixed = p_position.m_yFixed;
	m_spawnPosition.m_zFixed = p_position.m_zFixed;
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_enabled = 1;
	m_action = ACTION_READY;
	m_actionArgument = 0;
	m_activated = 0;

	int blockX = (p_position.m_xFixed >> 12) / 16;
	int blockY = (p_position.m_yFixed >> 12) / 16;

	if (blockX >= 0) {
		int collisionY = blockY + 1;
		if (collisionY >= 0) {
			int width = g_pMap->m_ground.m_width;
			if (width > blockX && g_pMap->m_ground.m_height > collisionY) {
				g_pMap->m_ground.m_ground[width * collisionY + blockX].m_collision |= 0x8000;
			}
		}

		if (blockX >= 0) {
			collisionY = blockY + 2;
			if (collisionY >= 0) {
				int width = g_pMap->m_ground.m_width;
				if (width > blockX && g_pMap->m_ground.m_height > collisionY) {
					g_pMap->m_ground.m_ground[width * collisionY + blockX].m_collision |= 0x8000;
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00427c40
bool CHand::Process()
{
	if (m_isRemoteObject != 0) {
		m_actionArgument = 1;
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				SetSndEffect(SFX_EEEEH);
			}
			m_pendingAction = m_action;
		}
		return true;
	}

	m_actionArgument = 0;
	if (m_activated != 0) {
		switch (m_action) {
		case ACTION_RECOVERY:
			if (m_actionDeadline < g_dwGameTick) {
				m_enabled = 1;
				m_activated = 0;
				Action(ACTION_READY);
				return true;
			}
			break;
		case ACTION_ACTIVATING:
			if (m_actionPhase1Deadline < g_dwGameTick) {
				m_target->Action(ACTION_WAITING_TO_DIE);
				m_target->m_actionDeadline = g_dwGameTick + 40;
				Action(ACTION_ACTIVATED);
				SetSndEffect(SFX_EEEEH);
				return true;
			}
			break;
		case ACTION_ACTIVATED:
			if (m_actionDeadline < g_dwGameTick) {
				m_enabled = 1;
				m_actionDeadline = g_dwGameTick + 20;
				Action(ACTION_RECOVERY);
			}
			break;
		default:
			return true;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x00427d70
bool CHand::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	if (p_object->m_objectType == OBJECT_PLAYER_2) {
		int distanceY = (p_position.m_yFixed >> 12) - (m_position.m_yFixed >> 12);
		int distanceX = (p_position.m_xFixed >> 12) - (m_position.m_xFixed >> 12);
		if ((distanceX < 0 ? -distanceX : distanceX) < 16 && distanceY >= 0 && distanceY < 48) {
			m_actionPhase1Deadline = 6;
			m_actionDeadline = 16;
			m_activator = p_object;
			p_object->ResetInstructions();
			m_activator->Action(ACTION_NONE);
			m_activator->m_actionDeadline = g_dwGameTick + 1000;
			RequestAction(ACTION_ACTIVATING);
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x00427e10
void CHand::DoActivate()
{
	m_activated = 1;
	m_target = m_activator;
	m_lastMovementTick = g_dwGameTick;
	m_actionPhase1Deadline += g_dwGameTick;
	m_actionDeadline += g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
}
