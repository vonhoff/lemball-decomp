#include "CHand.h"

#include "Application/SoundEffects.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"

#define HAND_PLAYER_TRIGGER_HALF_WIDTH 16
#define HAND_PLAYER_TRIGGER_DOWNWARD_RANGE 48
#define HAND_ACTIVATION_WINDUP_TICKS 6
#define HAND_ACTIVATION_SEQUENCE_TICKS 16
#define HAND_PLAYER_DEATH_DELAY_TICKS 40
#define HAND_RECOVERY_DURATION_TICKS 20
#define HAND_PLAYER_ACTION_LOCKOUT_TICKS 1000

// FUNCTION: LEMBALL 0x00427ad0
CHand::CHand() : CGlobalGameObject(OBJECT_HAND, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00427af0
void CHand::Initialise()
{
	m_stateTimer = 0;
	m_activated = false;
	m_enabled = false;
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
	m_enabled = true;
	m_action = ACTION_READY;
	m_actionArgument = REMOTE_PALETTE_REMAP_DISABLED;
	m_activated = false;

	int blockX = (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;

	if (blockX >= 0) {
		int collisionY = blockY + 1;
		if (collisionY >= 0) {
			int width = g_pMap->m_ground.m_width;
			if (width > blockX && g_pMap->m_ground.m_height > collisionY) {
				g_pMap->m_ground.m_ground[width * collisionY + blockX].m_collision |=
					GROUND_COLLISION_OBJECT_INTERACTION;
			}
		}

		if (blockX >= 0) {
			collisionY = blockY + 2;
			if (collisionY >= 0) {
				int width = g_pMap->m_ground.m_width;
				if (width > blockX && g_pMap->m_ground.m_height > collisionY) {
					g_pMap->m_ground.m_ground[width * collisionY + blockX].m_collision |=
						GROUND_COLLISION_OBJECT_INTERACTION;
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00427c40
bool CHand::Process()
{
	if (m_isRemoteObject != 0) {
		m_actionArgument = REMOTE_PALETTE_REMAP_ENABLED;
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				SetSndEffect(SFX_EEEEH);
			}
			m_pendingAction = m_action;
		}
		return true;
	}

	m_actionArgument = REMOTE_PALETTE_REMAP_DISABLED;
	if (m_activated != 0) {
		switch (m_action) {
		case ACTION_RECOVERY:
			if (m_actionDeadline < g_dwGameTick) {
				m_enabled = true;
				m_activated = false;
				Action(ACTION_READY);
				return true;
			}
			break;
		case ACTION_ACTIVATING:
			if (m_actionPhase1Deadline < g_dwGameTick) {
				m_target->Action(ACTION_WAITING_TO_DIE);
				m_target->m_actionDeadline = g_dwGameTick + HAND_PLAYER_DEATH_DELAY_TICKS;
				Action(ACTION_ACTIVATED);
				SetSndEffect(SFX_EEEEH);
				return true;
			}
			break;
		case ACTION_ACTIVATED:
			if (m_actionDeadline < g_dwGameTick) {
				m_enabled = true;
				m_actionDeadline = g_dwGameTick + HAND_RECOVERY_DURATION_TICKS;
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
		int distanceY =
			(p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
		int distanceX =
			(p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
		if ((distanceX < 0 ? -distanceX : distanceX) < HAND_PLAYER_TRIGGER_HALF_WIDTH && distanceY >= 0 &&
			distanceY < HAND_PLAYER_TRIGGER_DOWNWARD_RANGE) {
			m_actionPhase1Deadline = HAND_ACTIVATION_WINDUP_TICKS;
			m_actionDeadline = HAND_ACTIVATION_SEQUENCE_TICKS;
			m_activator = p_object;
			p_object->ResetInstructions();
			m_activator->Action(ACTION_NONE);
			m_activator->m_actionDeadline = g_dwGameTick + HAND_PLAYER_ACTION_LOCKOUT_TICKS;
			RequestAction(ACTION_ACTIVATING);
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x00427e10
void CHand::DoActivate()
{
	m_activated = true;
	m_target = m_activator;
	m_lastMovementTick = g_dwGameTick;
	m_actionPhase1Deadline += g_dwGameTick;
	m_actionDeadline += g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
}
