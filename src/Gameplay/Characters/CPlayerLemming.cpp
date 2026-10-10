#include "CPlayerLemming.h"

#include "Application/CDemo.h"
#include "Application/CGameStatus.h"
#include "Application/GameMain.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/RandomConstants.h"
#include "Gameplay/Behavior/StateMachine.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Geometry/CRect3.h"
#include "Gameplay/Geometry/Facing.h"
#include "Gameplay/Groups/CPlayerLemmingGroup.h"
#include "Gameplay/Groups/CPlayerLemmingGroupManager.h"
#include "Gameplay/Mechanisms/CBalloonPost.h"
#include "Gameplay/Mechanisms/CIce.h"
#include "Gameplay/Mechanisms/CMover.h"
#include "Gameplay/Messages/CObjectHitMess.h"
#include "Gameplay/Messages/GameMessageIds.h"
#include "Gameplay/Navigation/CAiDestinationList.h"
#include "Gameplay/Objects/CBaseGlobalObject.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Projectiles/CBullet.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Simulation/CAICursor.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"
#include "Multiplayer/Transport/CConnect.h"

#include <stddef.h>

#define PLAYER_LEMMING_ACTION_ARGUMENT_MASK 0x07
#define PLAYER_LEMMING_ACTION_ARGUMENT_FIELD_MASK 0x38
#define PLAYER_LEMMING_ACTION_ARGUMENT_SHIFT 3
#define PLAYER_LEMMING_ACTION_BYTE_MASK 0xff
#define PLAYER_LEMMING_SOUND_BYTE_SHIFT 8

enum {
	PLAYER_LEMMING_DESTINATION_CAPACITY = 20,
	PLAYER_LEMMING_NETWORK_STATE_SIZE_BYTES = 17,
	PLAYER_LEMMING_VERTICAL_CORRECTION_STEP_FIXED = 2 * FIXED_POINT_ONE,
	PLAYER_LEMMING_BULLET_HIT_STUN_TICKS = 40,
	PLAYER_LEMMING_BALL_HIT_STUN_TICKS = 60,
	PLAYER_LEMMING_IDLE_JIG_DURATION_TICKS = 56,
	PLAYER_LEMMING_IDLE_TOSS_DURATION_TICKS = 33,
	PLAYER_LEMMING_IDLE_LOOK_DURATION_TICKS = 38,
	PLAYER_LEMMING_INITIAL_FALL_HORIZONTAL_VELOCITY_FIXED = 3 * FIXED_POINT_ONE,
	PLAYER_LEMMING_INITIAL_FALL_VERTICAL_VELOCITY_FIXED = 10 * FIXED_POINT_ONE,
	PLAYER_LEMMING_BULLET_MUZZLE_HEIGHT_FIXED = 10 * FIXED_POINT_ONE,
	PLAYER_LEMMING_MINE_LAUNCH_VERTICAL_VELOCITY_FIXED = 10 * FIXED_POINT_ONE,
	PLAYER_LEMMING_SPAWN_HEIGHT_OFFSET_FIXED = 68 * FIXED_POINT_ONE,
	PLAYER_LEMMING_BOREDOM_DEADLINE_QUANTUM_MS = 66
};

// FUNCTION: LEMBALL 0x0040ecb0
CPlayerLemming::CPlayerLemming(int p_x,
							   int p_y,
							   int p_z,
							   int p_facing,
							   unsigned int p_alternatePlayer,
							   unsigned long p_spawnDelay)
	: CGlobalGameObject(p_alternatePlayer ? OBJECT_PLAYER_1 : OBJECT_PLAYER_2,
						GAME_OBJECT_COLLISION_PLAYER_LEMMING,
						PLAYER_LEMMING_DESTINATION_CAPACITY)
{
	m_alternatePlayer = p_alternatePlayer;
	m_spawnPosition.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
	m_spawnDelay = p_spawnDelay;
	m_initialFacingDirection = (short) p_facing;
	if (p_alternatePlayer != 0) {
		SetId(NextLoadingId());
	}
	else {
		m_payloadCapacity += PLAYER_LEMMING_NETWORK_STATE_SIZE_BYTES;
	}
}

// FUNCTION: LEMBALL 0x0040ed90
void CPlayerLemming::Restart()
{
	enum {
		NETWORK_LEMMING_SLOTS_PER_PLAYER = 4
	};
	CGlobalGameObject::Restart();
	if (m_alternatePlayer == 0) {
		m_playerIndex = g_wNetworkLemmingIndex;
		g_wNetworkLemmingIndex++;
		g_pAI->m_networkLemmings[m_playerIndex] = this;
		g_wLemmingCount++;
		m_action = ACTION_WAITING_TO_SPAWN;
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionDeadline = m_spawnDelay + g_dwGameTick;
		int tileX = m_spawnPosition.m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int tileY = m_spawnPosition.m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int tileZ = m_spawnPosition.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		CRect3 collision;
		collision.m_x1 = tileX - GAME_OBJECT_COLLISION_XY_MIN_INSET;
		collision.m_y1 = tileY - GAME_OBJECT_COLLISION_XY_MIN_INSET;
		collision.m_z1 = tileZ;
		collision.m_x2 = tileX + GAME_OBJECT_COLLISION_XY_MAX_OFFSET;
		collision.m_y2 = tileY + GAME_OBJECT_COLLISION_XY_MAX_OFFSET;
		collision.m_z2 = tileZ + GAME_OBJECT_COLLISION_BOX_LAST_PIXEL_OFFSET;
		m_collisionBounds = collision;
		int& objectCount = g_pAI->m_objectCount;
		g_pAI->m_objects[objectCount] = this;
		objectCount++;
		m_flightVelocity.m_xFixed = PLAYER_LEMMING_INITIAL_FALL_HORIZONTAL_VELOCITY_FIXED;
		m_flightVelocity.m_yFixed = 0;
		m_flightVelocity.m_zFixed = PLAYER_LEMMING_INITIAL_FALL_VERTICAL_VELOCITY_FIXED;
		m_isGroupLeader = 0;
		m_wasHitByBullet = false;
		m_hasDestination = 0;
		m_fireRequestState = FIRE_REQUEST_NONE;
		m_desiredFacingDirection = m_initialFacingDirection;
		SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
		AICOORD dest;
		dest.m_xFixed = ((NETWORK_LEMMING_SLOTS_PER_PLAYER - m_playerIndex) * GROUND_BLOCK_PIXEL_SIZE + tileX)
						<< FIXED_POINT_FRACTION_BITS;
		dest.m_yFixed = tileY << FIXED_POINT_FRACTION_BITS;
		dest.m_zFixed = tileZ << FIXED_POINT_FRACTION_BITS;
		AddDestination(dest);
		m_position.m_xFixed = m_spawnPosition.m_xFixed;
		m_position.m_yFixed = m_spawnPosition.m_yFixed;
		m_position.m_zFixed = m_spawnPosition.m_zFixed;
		m_deathRequested = false;
		m_sfxChanged = 1;
		m_group = NULL;
		m_ice = NULL;
		m_onConveyor = 0;
		m_ammoCount = PLAYER_START_AMMO;
	}
	else {
		m_playerIndex = g_wLocalLemmingIndex;
		g_wLocalLemmingIndex++;
		g_pAI->m_networkLemmings[m_playerIndex + NETWORK_LEMMING_SLOTS_PER_PLAYER] = this;
		m_isRemoteObject = 1;
		m_action = ACTION_DEAD;
	}
	m_position.m_zFixed += PLAYER_LEMMING_SPAWN_HEIGHT_OFFSET_FIXED;
	m_facingDirection = m_initialFacingDirection;
	m_inventoryCount = 0;
}

// FUNCTION: LEMBALL 0x0040efd0
CPlayerLemming::~CPlayerLemming()
{
}

// FUNCTION: LEMBALL 0x0040f000
void CPlayerLemming::HitBullet(CBullet* p_bullet)
{
	if (!g_pGameStatus->m_bulletHitHandlingDisabled) {
		if ((int) m_action < ACTION_FLYING || ((int) m_action > ACTION_HIDDEN && m_action != ACTION_ON_BALLOON)) {
			switch (p_bullet->m_owner) {
			case OWNER_ENEMY: {
				int randVal = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
				*g_pRandomSeed = randVal;
				if (randVal % 2) {
					return;
				}
				break;
			}
			case OWNER_REMOTE_PLAYER:
				if (g_pActiveConnection != NULL) {
					g_pObjectHitMessage->Send(p_bullet);
				}
				break;
			default:
				return;
			}
			m_hidden = 0;
			m_wasHitByBullet = true;
			m_actionDeadline = g_dwGameTick + PLAYER_LEMMING_BULLET_HIT_STUN_TICKS;
			m_facingDirection =
				(p_bullet->m_facingDirection + FACING_DIRECTION_OPPOSITE_OFFSET) & FACING_DIRECTION_MASK;
		}
	}
}

// FUNCTION: LEMBALL 0x0040f0d0
void CPlayerLemming::SetGroup(CPlayerLemmingGroup* p_group)
{
	m_group = p_group;
}

// FUNCTION: LEMBALL 0x0040f0e0
CPlayerLemmingGroup* CPlayerLemming::GetGroup()
{
	return m_group;
}

// FUNCTION: LEMBALL 0x0040f0f0
void CPlayerLemming::SetGroupLeader(unsigned int p_isLeader)
{
	m_isGroupLeader = p_isLeader;
}

// FUNCTION: LEMBALL 0x0040f100
void CPlayerLemming::SetGroup(unsigned int p_groupIndex)
{
	m_groupIndex = p_groupIndex;
}

// FUNCTION: LEMBALL 0x0040f110
unsigned int CPlayerLemming::IsGroupLeader()
{
	return m_isGroupLeader;
}

// FUNCTION: LEMBALL 0x0040f120
bool CPlayerLemming::Process()
{
	if (m_isRemoteObject == 0) {
		if (g_pAI->IsLemmingPlayerControlled(this)) {
			UserLemming(g_pAI, this);
			return false;
		}
		AIPlayerLemming(g_pAI, this);
	}
	return false;
}

// FUNCTION: LEMBALL 0x0040f160
void CPlayerLemming::TurnToFaceCursor()
{
	if (g_pDemo->m_demoMode == 0) {
		int cursorX;
		int cursorY;
		g_pAI->m_cursor->GetCursorSurfaceCoordinates(cursorX, cursorY);
		unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
													cursorX,
													cursorY);
		if (facing != (unsigned int) m_facingDirection) {
			if (g_anRotationDirections[(facing - m_facingDirection) & FACING_DIRECTION_MASK] < 0) {
				RotateAnticlockwise();
			}
			else {
				RotateClockwise();
			}
			SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
		}
		m_actionDeadline = g_dwGameTick + g_anTurnDelayCursor[m_objectType] / GAME_TICK_MILLISECONDS;
	}
}

// FUNCTION: LEMBALL 0x0040f220
void CPlayerLemming::TurnToFaceTarget()
{
	int facing = ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									   m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
									   m_fireTarget.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									   m_fireTarget.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	if (facing != m_facingDirection) {
		if (g_anRotationDirections[(facing - m_facingDirection) & FACING_DIRECTION_MASK] < 0) {
			RotateAnticlockwise();
		}
		else {
			RotateClockwise();
		}
		SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
	}
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / GAME_TICK_MILLISECONDS;
}

// FUNCTION: LEMBALL 0x0040f2b0
bool CPlayerLemming::IsRequestingFire()
{
	return m_fireRequestState == FIRE_REQUEST_PENDING;
}

// FUNCTION: LEMBALL 0x0040f2c0
void CPlayerLemming::RequestFire(int p_x, int p_y)
{
	if (m_fireRequestState == FIRE_REQUEST_NONE &&
		(m_action == ACTION_NONE || m_action == ACTION_WALKING || m_action == ACTION_IDLE_ANIMATION)) {
		m_fireRequestState = FIRE_REQUEST_PENDING;
		m_fireTarget.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
		m_fireTarget.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	}
}

// FUNCTION: LEMBALL 0x0040f310
void CPlayerLemming::Fire()
{
	AICOORD start;
	start.m_xFixed = m_position.m_xFixed;
	int facing = m_facingDirection;
	start.m_yFixed = m_position.m_yFixed;
	start.m_zFixed = m_position.m_zFixed + PLAYER_LEMMING_BULLET_MUZZLE_HEIGHT_FIXED;
	switch (m_action) {
	case ACTION_FLYING:
	case ACTION_HIDDEN:
	case ACTION_HIT:
	case ACTION_DEAD:
	case ACTION_FINDING_ROUTE:
	case ACTION_JUMPING:
	case ACTION_FALLING:
	case ACTION_WAITING_TO_SPAWN:
	case ACTION_SOMMERSAULT:
	case ACTION_EXTERNAL_CONTROL:
		break;
	default:
		if (g_pGameStatus->m_unlimitedAmmo || m_ammoCount != 0) {
			SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
			g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_PLAYER, facing, start, m_fireTarget);
			m_soundEffect = SFX_GUN;
			if (!g_pGameStatus->m_unlimitedAmmo) {
				m_ammoCount--;
			}
		}
	}
	m_fireRequestState = FIRE_REQUEST_NONE;
}

// FUNCTION: LEMBALL 0x0040f410
void CPlayerLemming::StartFiring()
{
	m_actionDeadline = g_dwGameTick + GAME_OBJECT_FIRE_WINDUP_TICKS;
}

// FUNCTION: LEMBALL 0x0040f420
void CPlayerLemming::EndFiring()
{
	m_fireRequestState = FIRE_REQUEST_NONE;
}

// FUNCTION: LEMBALL 0x0040f430
bool CPlayerLemming::FacingCursor()
{
	if (g_pDemo->m_demoMode != 0) {
		return true;
	}
	int cursorX;
	int cursorY;
	g_pAI->m_cursor->GetCursorSurfaceCoordinates(cursorX, cursorY);
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
												cursorX,
												cursorY);
	return (int) m_facingDirection == (int) facing;
}

// FUNCTION: LEMBALL 0x0040f4b0
bool CPlayerLemming::FacingTarget()
{
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
												m_fireTarget.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												m_fireTarget.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	return (int) m_facingDirection == (int) facing;
}

// FUNCTION: LEMBALL 0x0040f500
void CPlayerLemming::Die()
{
	int inventoryIndex = 0;
	if ((int) m_inventoryCount > 0) {
		CGameObject** inventoryObject = m_inventoryObjects;
		do {
			switch ((*inventoryObject)->m_objectType) {
			case OBJECT_KEY_1:
			case OBJECT_KEY_2:
			case OBJECT_KEY_3:
			case OBJECT_BALLOON_0:
			case OBJECT_BALLOON_2:
			case OBJECT_BALLOON_4:
			case OBJECT_BALLOON_6:
				((CBaseGlobalObject*) *inventoryObject)->OldRestart();
				break;
			}
			inventoryObject++;
			inventoryIndex++;
		} while (inventoryIndex < (int) m_inventoryCount);
	}
	int objectIndex = 0;
	int& objectCount = g_pAI->m_objectCount;
	if (objectIndex < objectCount) {
		CGameObject**& objectArray = g_pAI->m_objects;
		do {
			if (objectArray[objectIndex] == this) {
				objectCount--;
				while (objectIndex < objectCount) {
					objectArray[objectIndex] = objectArray[objectIndex + 1];
					objectIndex++;
				}
				objectArray[objectCount] = NULL;
				break;
			}
			objectIndex++;
		} while (objectIndex < objectCount);
	}
	g_wLemmingCount--;
	if (g_wLemmingCount == 0) {
		g_pAI->GameState(GAME_STATUS_FAILURE);
	}
}

// FUNCTION: LEMBALL 0x0040f600
void CPlayerLemming::HitMine()
{
	C3DVector vel;
	vel.m_xFixed = 0;
	vel.m_yFixed = 0;
	m_wasHitByMine = true;
	vel.m_zFixed = PLAYER_LEMMING_MINE_LAUNCH_VERTICAL_VELOCITY_FIXED;
	StartFly(vel, NULL);
	m_deathRequested = true;
}

// FUNCTION: LEMBALL 0x0040f640
void CPlayerLemming::GetData()
{
	unsigned short packedState[8];
	m_position.m_xFixed = (int) (unsigned int) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_position.m_yFixed = (int) (unsigned int) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_position.m_zFixed = (int) (unsigned int) GetWORD() << FIXED_POINT_FRACTION_BITS;
	Get(packedState[1]);
	m_facingDirection = packedState[1] & FACING_DIRECTION_MASK;
	m_actionArgument =
		(packedState[1] & PLAYER_LEMMING_ACTION_ARGUMENT_FIELD_MASK) >> PLAYER_LEMMING_ACTION_ARGUMENT_SHIFT;
	Get(packedState[1]);
	m_action = (eAction) (packedState[1] & PLAYER_LEMMING_ACTION_BYTE_MASK);
	m_soundEffect = (eSoundEffect) (packedState[1] >> PLAYER_LEMMING_SOUND_BYTE_SHIFT);
	m_stateTimer = GetDWORD();
}

// FUNCTION: LEMBALL 0x0040f6f0
void CPlayerLemming::AddData()
{
	Add((unsigned short) MESSAGE_PLAYER_LEMMING_STATE);
	Add((unsigned char) m_playerIndex);
	Add((unsigned short) (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (((m_actionArgument & PLAYER_LEMMING_ACTION_ARGUMENT_MASK)
						   << PLAYER_LEMMING_ACTION_ARGUMENT_SHIFT) |
						  (m_facingDirection & FACING_DIRECTION_MASK)));
	Add((unsigned short) ((m_soundEffect << PLAYER_LEMMING_SOUND_BYTE_SHIFT) |
						  (m_action & PLAYER_LEMMING_ACTION_BYTE_MASK)));
	if (g_dwSimulationTimestamp < m_stateTimer) {
		m_stateTimer = g_dwSimulationTimestamp;
	}
	Add((unsigned long) m_stateTimer);
	m_sfxChanged = 0;
}

// FUNCTION: LEMBALL 0x0040f7a0
bool CPlayerLemming::CheckSFX()
{
	unsigned int* pSfx = &m_sfxChanged;
	eSoundEffect sfx = m_soundEffect;
	if (m_cachedSoundEffect == sfx && *pSfx == 0) {
		*pSfx = 0;
		m_cachedSoundEffect = sfx;
		return *pSfx;
	}
	m_cachedSoundEffect = sfx;
	*pSfx = 1;
	return *pSfx;
}

// FUNCTION: LEMBALL 0x0040f7e0
unsigned int CPlayerLemming::CheckNetworkStateChanged()
{
	int x = m_position.m_xFixed;
	m_sfxChanged = ((m_networkPositionCache.m_xFixed ^ x) & FIXED_POINT_INTEGER_MASK) != 0 || m_sfxChanged;
	int y = m_position.m_yFixed;
	m_sfxChanged = ((m_networkPositionCache.m_yFixed ^ y) & FIXED_POINT_INTEGER_MASK) != 0 || m_sfxChanged;
	int z = m_position.m_zFixed;
	m_sfxChanged = ((m_networkPositionCache.m_zFixed ^ z) & FIXED_POINT_INTEGER_MASK) != 0 || m_sfxChanged;
	eAction action = m_action;
	switch (action) {
	case ACTION_NONE:
		m_sfxChanged = m_cachedAction != action || m_sfxChanged;
		m_cachedAction = action;
		break;
	case ACTION_TURNING:
		break;
	default:
		m_sfxChanged = m_cachedAction != action || m_sfxChanged;
		m_sfxChanged = m_cachedActionArgument != m_actionArgument || m_sfxChanged;
		m_sfxChanged = m_cachedStateTimer != m_stateTimer || m_sfxChanged;
		m_cachedAction = action;
		break;
	}
	eSoundEffect sound = m_soundEffect;
	m_sfxChanged = m_cachedSoundEffect != sound || m_sfxChanged;
	m_cachedFacingDirection = m_facingDirection;
	m_networkPositionCache.m_xFixed = x;
	m_networkPositionCache.m_yFixed = y;
	m_networkPositionCache.m_zFixed = z;
	m_cachedSoundEffect = sound;
	m_cachedActionArgument = m_actionArgument;
	m_cachedStateTimer = m_stateTimer;
	return m_sfxChanged;
}

// FUNCTION: LEMBALL 0x0040f960
bool CPlayerLemming::HasObject(eObjectType p_objectType)
{
	if (p_objectType != OBJECT_AMMO) {
		int count = m_inventoryCount;
		if (count != PLAYER_INVENTORY_CAPACITY) {
			for (int i = 0; i < count; i++) {
				if (m_inventoryTypes[i] == p_objectType) {
					return true;
				}
			}
		}
	}
	else if (m_ammoCount == PLAYER_MAX_AMMO) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0040f9b0
bool CPlayerLemming::AddObject(eObjectType p_objectType, CGameObject* p_object)
{
	if (m_inventoryCount == PLAYER_INVENTORY_CAPACITY) {
		return false;
	}
	if (HasObject(p_objectType)) {
		return false;
	}
	m_inventoryTypes[m_inventoryCount] = p_objectType;
	m_inventoryObjects[m_inventoryCount] = p_object;
	m_inventoryCount++;
	return true;
}

// FUNCTION: LEMBALL 0x0040fa10
void CPlayerLemming::RandomAction()
{
	int randVal = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
	*g_pRandomSeed = randVal;
	int idleAnim = randVal % LEMMING_IDLE_ANIMATION_COUNT;
	m_actionArgument = (short) idleAnim;
	switch (idleAnim) {
	case LEMMING_IDLE_JIG:
		m_actionDeadline = g_dwGameTick + PLAYER_LEMMING_IDLE_JIG_DURATION_TICKS;
		break;
	case LEMMING_IDLE_TOSS:
		m_actionDeadline = g_dwGameTick + PLAYER_LEMMING_IDLE_TOSS_DURATION_TICKS;
		break;
	case LEMMING_IDLE_LOOK:
		m_actionDeadline = g_dwGameTick + PLAYER_LEMMING_IDLE_LOOK_DURATION_TICKS;
		break;
	}
}

// FUNCTION: LEMBALL 0x0040fa80
void CPlayerLemming::Resurrect(const AICOORD& p_position)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_deathRequested = false;
	g_wLemmingCount++;
	m_facingDirection = 0;
	m_inventoryCount = 0;
	m_action = ACTION_NONE;
	m_isGroupLeader = 0;
	m_wasHitByBullet = false;
	m_ice = NULL;
	m_onConveyor = 0;
	m_hasDestination = 0;
	short& resetFlags = m_unk0xc4;
	resetFlags = 0;
	m_fireRequestState = FIRE_REQUEST_NONE;
	m_isFlying = false;
	m_hidden = 0;
	m_activationReserved = 0;
	m_routeSearchFailed = false;
	m_routeSearchActive = false;
	m_isJumping = false;
	m_isFalling = false;
	m_wasHitByMine = false;
	m_liftId = INVALID_OBJECT_ID;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	m_flightVelocity.m_xFixed = 0;
	m_flightVelocity.m_yFixed = 0;
	unsigned short& switchId = m_invisibleSwitchId;
	switchId = INVALID_OBJECT_ID;
	m_flightVelocity.m_zFixed = 0;
	short& facing = m_desiredFacingDirection;
	facing = 0;
	m_unk0x58 = 0;
	m_onMover = false;
	m_ammoCount = PLAYER_START_AMMO;
	SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
	int tileX = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int tileY = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int tileZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	CRect3 collision;
	collision.m_x1 = tileX - GAME_OBJECT_COLLISION_XY_MIN_INSET;
	collision.m_y1 = tileY - GAME_OBJECT_COLLISION_XY_MIN_INSET;
	collision.m_z1 = tileZ;
	collision.m_x2 = tileX + GAME_OBJECT_COLLISION_XY_MAX_OFFSET;
	collision.m_y2 = tileY + GAME_OBJECT_COLLISION_XY_MAX_OFFSET;
	collision.m_z2 = tileZ + GAME_OBJECT_COLLISION_BOX_LAST_PIXEL_OFFSET;
	m_collisionBounds = collision;
	int& objectCount = g_pAI->m_objectCount;
	g_pAI->m_objects[objectCount] = this;
	objectCount++;
	m_destinationList->m_count = 0;
}

// FUNCTION: LEMBALL 0x0040fbe0
int CPlayerLemming::GetLastBalloon()
{
	if (m_inventoryCount == 0) {
		return OBJECT_INVALID;
	}
	for (int i = m_inventoryCount - 1; i >= 0; i--) {
		switch (m_inventoryTypes[i]) {
		case OBJECT_BALLOON_0:
			return OBJECT_BALLOON_0;
		case OBJECT_BALLOON_2:
			return OBJECT_BALLOON_2;
		case OBJECT_BALLOON_4:
			return OBJECT_BALLOON_4;
		case OBJECT_BALLOON_6:
			return OBJECT_BALLOON_6;
		}
	}
	return OBJECT_INVALID;
}

// FUNCTION: LEMBALL 0x0040fc50
void CPlayerLemming::RemoveObject(eObjectType p_objectType)
{
	for (int i = 0; i < (int) m_inventoryCount; i++) {
		if (m_inventoryTypes[i] == p_objectType) {
			for (int j = i + 1; j < (int) m_inventoryCount; j++) {
				m_inventoryTypes[j - 1] = m_inventoryTypes[j];
			}
			m_inventoryCount--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0040fcb0
int CPlayerLemming::GetObject(int p_index)
{
	if ((int) m_inventoryCount <= p_index) {
		return OBJECT_INVALID;
	}
	return m_inventoryTypes[p_index];
}

// FUNCTION: LEMBALL 0x0040fcd0
void CPlayerLemming::ExternalControlEnd()
{
	int actionArgument = (unsigned short) m_actionArgument;
	switch (actionArgument) {
	case EXTERNAL_CONTROL_ELECTROCUTED:
	case EXTERNAL_CONTROL_ON_FIRE:
		Die();
		Action(ACTION_DEAD);
		break;
	default:
		Action(ACTION_NONE);
		break;
	}
}

// FUNCTION: LEMBALL 0x0040fd10
void CPlayerLemming::OnBalloon()
{
	AICOORD postPos;
	postPos.m_xFixed = DEBUG_SENTINEL;
	postPos.m_yFixed = DEBUG_SENTINEL;
	postPos.m_zFixed = DEBUG_SENTINEL;
	g_pAI->m_balloonPost->FindPost(m_balloonObjectType, postPos);
	int dist = Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
						m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
						postPos.m_xFixed >> FIXED_POINT_FRACTION_BITS,
						postPos.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	if (dist < 16) {
		m_balloonPostActive = 0;
		SetSndEffect(SFX_BALLOON_EXPLODE);
		m_isFalling = true;
		m_lastMovementTick = g_dwGameTick;
		m_actionArgument = 0;
		m_action = ACTION_FALLING;
		m_flightZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		ResetInstructions();
		int posX = m_position.m_xFixed;
		int posY = m_position.m_yFixed;
		m_groundPosition.m_xFixed = posX;
		m_groundPosition.m_yFixed = posY;
		int tileY = posY >> FIXED_POINT_FRACTION_BITS;
		CMap* map = g_pMap;
		int tileX = posX >> FIXED_POINT_FRACTION_BITS;
		int blockY = tileY >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockX = tileX >> GROUND_BLOCK_PIXEL_SHIFT;
		unsigned short groundZ;
		if (tileX < 0 || tileY < 0 || blockX >= map->m_ground.m_width || blockY >= g_pMap->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			int cellX = tileX & GROUND_BLOCK_PIXEL_MASK;
			int cellY = tileY & GROUND_BLOCK_PIXEL_MASK;
			groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
		}
		m_groundPosition.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
		return;
	}
	int posY = m_position.m_yFixed;
	int posX = m_position.m_xFixed;
	int tileY = posY >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int tileX = posX >> FIXED_POINT_FRACTION_BITS;
	int blockY = tileY >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockX = tileX >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short groundZ;
	if (tileX < 0 || tileY < 0 || blockX >= map->m_ground.m_width || blockY >= g_pMap->m_ground.m_height) {
		groundZ = 0;
	}
	else {
		int cellX = tileX & GROUND_BLOCK_PIXEL_MASK;
		int cellY = tileY & GROUND_BLOCK_PIXEL_MASK;
		groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	int baseZ = groundZ + 32;
	int curZ = m_position.m_zFixed;
	int tileZ = curZ >> FIXED_POINT_FRACTION_BITS;
	int lowerZ = baseZ - 6;
	int upperZ = baseZ + 6;
	if (dist != 0) {
		int factor = (g_dwGameTick - m_lastMovementTick) * 2;
		m_position.m_xFixed = (unsigned int) m_position.m_xFixed +
							  (int) (((unsigned int) postPos.m_xFixed - m_position.m_xFixed) * factor) / dist;
		m_position.m_yFixed = (unsigned int) m_position.m_yFixed +
							  (int) (((unsigned int) postPos.m_yFixed - m_position.m_yFixed) * factor) / dist;
	}
	int aboveLowerZ = tileZ >= lowerZ;
	int belowUpperZ = tileZ <= upperZ;
	if ((aboveLowerZ & belowUpperZ) == 0) {
		if (tileZ > baseZ) {
			curZ -= PLAYER_LEMMING_VERTICAL_CORRECTION_STEP_FIXED;
		}
		else {
			curZ += PLAYER_LEMMING_VERTICAL_CORRECTION_STEP_FIXED;
		}
		m_position.m_zFixed = curZ;
	}
	m_lastMovementTick = g_dwGameTick;
}

// FUNCTION: LEMBALL 0x0040ff60
void CPlayerLemming::StartBalloon()
{
}

// FUNCTION: LEMBALL 0x0040ff70
void CPlayerLemming::RequestBalloon()
{
	AICOORD postPos;
	postPos.m_xFixed = DEBUG_SENTINEL;
	postPos.m_yFixed = DEBUG_SENTINEL;
	postPos.m_zFixed = DEBUG_SENTINEL;
	int lastBalloon = GetLastBalloon();
	switch (lastBalloon) {
	case OBJECT_INVALID:
		m_balloonPostActive = 0;
		return;
	case OBJECT_BALLOON_0:
		m_balloonObjectType = OBJECT_BALLOON_1;
		RemoveObject(OBJECT_BALLOON_0);
		m_actionArgument = 3;
		break;
	case OBJECT_BALLOON_2:
		m_balloonObjectType = OBJECT_BALLOON_3;
		RemoveObject(OBJECT_BALLOON_2);
		m_actionArgument = 1;
		break;
	case OBJECT_BALLOON_4:
		m_balloonObjectType = OBJECT_BALLOON_5;
		RemoveObject(OBJECT_BALLOON_4);
		m_actionArgument = 4;
		break;
	case OBJECT_BALLOON_6:
		m_balloonObjectType = OBJECT_BALLOON_7;
		RemoveObject(OBJECT_BALLOON_6);
		m_actionArgument = 0;
		break;
	}
	m_balloonPostActive = g_pAI->m_balloonPost->FindPost(m_balloonObjectType, postPos);
	m_lastMovementTick = g_dwGameTick;
	g_pAI->Score(AI_SCORE_BALLOON_POST_ACTIVATION_POINTS);
}

// FUNCTION: LEMBALL 0x00410090
void CPlayerLemming::SetBored(unsigned long p_minimumDelay)
{
	int random = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
	*g_pRandomSeed = random;
	m_boredDeadline = p_minimumDelay + random % GAME_OBJECT_BOREDOM_RANDOM_DELAY_RANGE_MS;
	m_boredDeadline = m_boredDeadline - m_boredDeadline % PLAYER_LEMMING_BOREDOM_DEADLINE_QUANTUM_MS;
	m_boredDeadline = m_boredDeadline / GAME_TICK_MILLISECONDS;
	m_boredDeadline = m_actionDeadline + m_boredDeadline;
}

// FUNCTION: LEMBALL 0x00410100
void CPlayerLemming::StartStanding()
{
	CMover* mover = NULL;
	unsigned int groundZ = g_pMap->GetZ(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
										m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
										&mover);
	int tileZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	if (m_onMover == 0 && mover != NULL) {
		mover->GetOn(this);
	}
	if (tileZ <= (int) groundZ + 2) {
		if (mover == NULL) {
			m_position.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
		}
		g_pAI->StepOn(m_position, this, m_collisionFlags);
		return;
	}
	m_actionDeadline = g_dwGameTick;
	if ((m_collisionFlags & GAME_OBJECT_COLLISION_ALLOW_FALL) != 0) {
		m_flightVelocity.m_yFixed = 0;
		m_isFalling = true;
		m_flightVelocity.m_xFixed = PLAYER_LEMMING_INITIAL_FALL_HORIZONTAL_VELOCITY_FIXED;
		m_flightVelocity.m_zFixed = (((tileZ - (int) groundZ) / 8) + 1) * FIXED_POINT_ONE;
		unsigned int now = g_dwGameTick;
		int posY = m_position.m_yFixed;
		m_actionArgument = 0;
		m_lastMovementTick = now;
		int posX = m_position.m_xFixed;
		m_flightZ = tileZ;
		m_groundPosition.m_xFixed = posX;
		m_groundPosition.m_yFixed = posY;
		m_groundPosition.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
	}
}

// FUNCTION: LEMBALL 0x00410220
void CPlayerLemming::Action(eAction p_action)
{
	m_stateTimer = g_dwSimulationTimestamp;
	if (p_action == ACTION_DEAD) {
		CGlobalGameObject::Action(p_action);
		return;
	}
	m_action = p_action;
}

// FUNCTION: LEMBALL 0x00410250
void CPlayerLemming::OnConveyor(unsigned int p_onConveyor, CIce* p_ice, unsigned int p_leave)
{
	if (p_onConveyor == 0 && m_onConveyor != 0 && p_leave != 0) {
		((CIce*) m_ice)->Leave(this);
	}
	m_onConveyor = p_onConveyor;
	m_ice = p_ice;
	if (p_onConveyor != 0) {
		if (m_group->GetElementsInGroup() > 1) {
			g_pAI->m_playerGroupManager->CreateNewGroup(1, &m_objectId);
		}
		m_group->ClearExistingWaypoints();
	}
}

// FUNCTION: LEMBALL 0x004102d0
bool CPlayerLemming::IsSelectable()
{
	if (!CGameObject::IsSelectable()) {
		return false;
	}
	if (m_action == ACTION_EXTERNAL_CONTROL) {
		int actionArgument = (unsigned short) m_actionArgument;
		if (actionArgument >= EXTERNAL_CONTROL_ELECTROCUTED && actionArgument <= EXTERNAL_CONTROL_ON_FIRE) {
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x004108b0
void CPlayerLemming::GetViewData(CViewData& p_viewData)
{
	CGameObject::GetViewData(p_viewData);
	int flags = (m_isGroupLeader != 0 ? LEMMING_VIEW_STATUS_GROUP_LEADER : 0) |
				(m_groupIndex != 0 ? LEMMING_VIEW_STATUS_IN_GROUP : 0);
	p_viewData.m_statusFlags = flags;
	p_viewData.m_playerIndex = m_playerIndex;
}

// FUNCTION: LEMBALL 0x004109f0
int CPlayerLemming::Bored()
{
	return m_boredDeadline < g_dwGameTick;
}

// FUNCTION: LEMBALL 0x00410a10
bool CPlayerLemming::IsHit()
{
	return m_wasHitByBullet;
}

// FUNCTION: LEMBALL 0x00410a20
void CPlayerLemming::GetHit()
{
	int& count = g_pAI->m_objectCount;
	int index = 0;
	int objectCount = count;
	if (index < objectCount) {
		CGameObject**& objects = g_pAI->m_objects;
		do {
			if (objects[index] == this) {
				count = objectCount - 1;
				while (index < count) {
					objects[index] = objects[index + 1];
					index++;
				}
				objects[count] = NULL;
				return;
			}
			index++;
		} while (index < objectCount);
	}
}

// FUNCTION: LEMBALL 0x00410aa0
void CPlayerLemming::HitBall()
{
	m_wasHitByBullet = true;
	m_actionDeadline = g_dwGameTick + PLAYER_LEMMING_BALL_HIT_STUN_TICKS;
}

// FUNCTION: LEMBALL 0x00410ac0
void CPlayerLemming::PickUpAmmo(unsigned short p_amount)
{
	m_ammoCount += p_amount;
	if (m_ammoCount > PLAYER_MAX_AMMO) {
		m_ammoCount = PLAYER_MAX_AMMO;
	}
}

// FUNCTION: LEMBALL 0x00410af0
int CPlayerLemming::OnConveyor()
{
	return m_onConveyor;
}

// FUNCTION: LEMBALL 0x00410b00
CIce* CPlayerLemming::Conveyor()
{
	return m_ice;
}

// FUNCTION: LEMBALL 0x00410b10
int CPlayerLemming::QOnBalloon()
{
	return m_balloonPostActive;
}
