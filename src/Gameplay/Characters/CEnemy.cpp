#include "CEnemy.h"

#include "Application/GameMain.h"

#include "Gameplay/Simulation/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/Facing.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Behavior/StateMachine.h"
#include "Gameplay/Characters/EnemyBehavior.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Projectiles/CBullet.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Math/RandomConstants.h"

enum {
	ENEMY_HIT_RESPONSE_DELAY_TICKS = 60
};

#include <stddef.h>
enum eEnemyBehaviorStage {
	ENEMY_BEHAVIOR_STAGE_FIRST = 0,
	ENEMY_BEHAVIOR_STAGE_SECOND = 1,
	ENEMY_BEHAVIOR_STAGE_THIRD = 2
};

#define ENEMY_FIRE_RAPID_INTERVAL_MS 100
#define ENEMY_FIRE_SLOW_INTERVAL_MS 800
#define ENEMY_FIRE_RANDOM_MIN_INTERVAL_MS 150
#define ENEMY_FIRE_RANDOM_INTERVAL_RANGE_MS 1000
#define ENEMY_MUZZLE_HEIGHT_FIXED 0xc000
#define ENEMY_FIRE_IDLE 0
#define ENEMY_FIRE_REQUESTED 1
#define ENEMY_FIRE_FIRED 2

struct EnemyFacingOffset {
	int m_dx;
	int m_dy;
};

// GLOBAL: LEMBALL 0x004950c0
EnemyFacingOffset g_enemyFacingOffsets[8] = {{0, 3}, {-4, 1}, {-5, 0}, {-4, -3}, {0, -4}, {6, -3}, {5, 0}, {4, 1}};

enum {
	ENEMY_DESTINATION_CAPACITY = 10,
	ENEMY_LOS_MIN_RATIO_FIXED = 0x6a0,
	ENEMY_LOS_MAX_RATIO_HALF_FIXED = 0x1350,
	ENEMY_MINE_LAUNCH_VERTICAL_VELOCITY_FIXED = 10 * FIXED_POINT_ONE
};

// FUNCTION: LEMBALL 0x0041fba0
CEnemy::CEnemy(CAI* p_ai, int p_x, int p_y, int p_z, int p_facingDirection)
	: CGameObject(OBJECT_PLAYER_1, GAME_OBJECT_COLLISION_ENEMY, ENEMY_DESTINATION_CAPACITY), m_targetPosition(),
	  m_fireTarget()
{
	unsigned short z;
	int width;
	int blockX;
	int blockY;
	CMap* map;

	g_pAI = p_ai;
	m_spawnPosition.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
	map = g_pMap;
	blockX = p_x >> GROUND_BLOCK_PIXEL_SHIFT;
	blockY = p_y >> GROUND_BLOCK_PIXEL_SHIFT;
	if (p_x < 0 || p_y < 0 || g_pMap->m_ground.m_width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		width = map->m_ground.m_width;
		int pixelX = p_x & GROUND_BLOCK_PIXEL_MASK;
		int pixelY = p_y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * width + blockX].GetZ(pixelX, pixelY);
	}
	m_initialFacingDirection = (short) p_facingDirection;
	m_spawnPosition.m_zFixed = (int) z << FIXED_POINT_FRACTION_BITS;
	SetId((unsigned short) NextLoadingId());
	m_state2Action = ENEMY_ACTION_STOP;
	m_state1Action = ENEMY_ACTION_STOP;
	m_state0Action = ENEMY_ACTION_STOP;
	m_state2Rule = ENEMY_RULE_NONE;
	m_state1Rule = ENEMY_RULE_NONE;
	m_state0Rule = ENEMY_RULE_NONE;
	m_state0Data.m_waypointInformation = NULL;
	m_state1Data.m_waypointInformation = NULL;
	m_state2Data.m_waypointInformation = NULL;
}

// FUNCTION: LEMBALL 0x0041fcd0
void CEnemy::Restart()
{
	CGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_facingDirection = m_initialFacingDirection;
	m_stateIndex = ENEMY_BEHAVIOR_STAGE_FIRST;
	m_fireState = ENEMY_FIRE_IDLE;
	m_hit = 0;
	m_deathRequested = 0;

	CAI* ai = g_pAI;
	ai->m_objects[ai->m_objectCount] = this;
	ai->m_objectCount++;

	if (m_state0Data.m_waypointInformation != NULL) {
		m_state0Data.m_waypointInformation->m_waypointIndex = 0;
		m_state0Data.m_waypointInformation->m_waypointStep = 1;
	}
	if (m_state1Data.m_waypointInformation != NULL) {
		m_state1Data.m_waypointInformation->m_waypointIndex = 0;
		m_state1Data.m_waypointInformation->m_waypointStep = 1;
	}
	if (m_state2Data.m_waypointInformation != NULL) {
		m_state2Data.m_waypointInformation->m_waypointIndex = 0;
		m_state2Data.m_waypointInformation->m_waypointStep = 1;
	}
}

// FUNCTION: LEMBALL 0x0041fda0
CEnemy::~CEnemy()
{
	if (m_state0Action == ENEMY_ACTION_PATROL) {
		delete[] m_state0Data.m_waypointInformation->m_waypoints;
		delete m_state0Data.m_waypointInformation;
	}
	if (m_state1Action == ENEMY_ACTION_PATROL) {
		delete[] m_state1Data.m_waypointInformation->m_waypoints;
		delete m_state1Data.m_waypointInformation;
	}
	if (m_state2Action == ENEMY_ACTION_PATROL) {
		delete[] m_state2Data.m_waypointInformation->m_waypoints;
		delete m_state2Data.m_waypointInformation;
	}
}

// FUNCTION: LEMBALL 0x0041fe30
void CEnemy::SetEnemyType(eEnemyStateActions p_action0,
						  eEnemyStateRules p_rule0,
						  eEnemyStateActions p_action1,
						  eEnemyStateRules p_rule1,
						  eEnemyStateActions p_action2,
						  eEnemyStateRules p_rule2)
{
	m_state0Action = p_action0;
	m_state0Rule = p_rule0;
	m_state1Action = p_action1;
	m_state1Rule = p_rule1;
	m_state2Action = p_action2;
	m_state2Rule = p_rule2;
}

// FUNCTION: LEMBALL 0x0041fe70
void CEnemy::GetEnemyType(eEnemyStateActions& p_action0,
						  eEnemyStateRules& p_rule0,
						  eEnemyStateActions& p_action1,
						  eEnemyStateRules& p_rule1,
						  eEnemyStateActions& p_action2,
						  eEnemyStateRules& p_rule2)
{
	p_action0 = m_state0Action;
	p_rule0 = m_state0Rule;
	p_action1 = m_state1Action;
	p_rule1 = m_state1Rule;
	p_action2 = m_state2Action;
	p_rule2 = m_state2Rule;
}

// FUNCTION: LEMBALL 0x0041fec0
bool CEnemy::Process()
{
	if (m_action != ACTION_DEAD) {
		switch (m_stateIndex) {
		case ENEMY_BEHAVIOR_STAGE_FIRST:
			ProcessAction(m_state0Rule, m_state0Action, &m_state0Data);
			break;
		case ENEMY_BEHAVIOR_STAGE_SECOND:
			ProcessAction(m_state1Rule, m_state1Action, &m_state1Data);
			break;
		case ENEMY_BEHAVIOR_STAGE_THIRD:
			ProcessAction(m_state2Rule, m_state2Action, &m_state2Data);
			break;
		}

		EnemyState(g_pAI, this);
		g_pAI->StepOn(m_position, this, m_collisionFlags);
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041ff60
void CEnemy::ProcessAction(eEnemyStateRules p_rule, eEnemyStateActions p_action, tEnemyLemmingUnion* p_data)
{
	if (ProcessRule(p_rule) == true) {
		m_stateIndex++;
		if (m_stateIndex > ENEMY_BEHAVIOR_STAGE_THIRD) {
			m_stateIndex = ENEMY_BEHAVIOR_STAGE_FIRST;
		}
	}
	else {
		switch (p_action) {
		case ENEMY_ACTION_PATROL:
			EnemyAction_PATROL(p_data);
			break;
		case ENEMY_ACTION_TURN_AND_FIRE_RAPID:
			EnemyAction_TURNANDFIRERAPID(p_data);
			break;
		case ENEMY_ACTION_TURN_AND_FIRE_SLOW:
			EnemyAction_TURNANDFIRESLOW(p_data);
			break;
		case ENEMY_ACTION_TURN_AND_FIRE_RANDOM:
			EnemyAction_TURNANDFIRERANDOM(p_data);
			break;
		default:
			StopMoving();
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00420000
bool CEnemy::ProcessRule(eEnemyStateRules p_rule)
{
	switch (p_rule) {
	case ENEMY_RULE_NONE:
		return true;
	case ENEMY_RULE_RADIUS50:
		return EnemyRule_RADIUS50();
	case ENEMY_RULE_NOT_RADIUS50:
		return EnemyRule_RADIUS50() == 0;
	case ENEMY_RULE_RADIUS50_AND_LOS:
		return EnemyRule_RADIUS50ANDLINEOFSIGHT();
	case ENEMY_RULE_NOT_RADIUS50_AND_LOS:
		return EnemyRule_RADIUS50ANDLINEOFSIGHT() == 0;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x00420070
bool CEnemy::EnemyRule_RADIUS50()
{
	if (g_pAI->m_gameplayEnabled == 0) {
		return false;
	}
	return CheckRadius(50);
}

// FUNCTION: LEMBALL 0x00420090
bool CEnemy::EnemyRule_RADIUS50ANDLINEOFSIGHT()
{
	if (g_pAI->m_gameplayEnabled == 0) {
		return false;
	}
	int inRadius = CheckRadius(50);
	if (inRadius == 0) {
		return false;
	}
	inRadius &= LineOfSight(m_targetPosition);
	return inRadius;
}

// FUNCTION: LEMBALL 0x004200f0
void CEnemy::EnemyAction_PATROL(tEnemyLemmingUnion* p_data)
{
	AICOORD destination;
	if (DestinationExists() != true) {
		const CPt3& position = g_pAI->GetNodePosition(
			p_data->m_waypointInformation->m_waypoints[p_data->m_waypointInformation->m_waypointIndex]);
		destination.m_xFixed = position.m_x;
		destination.m_yFixed = position.m_y;
		destination.m_zFixed = position.m_z;

		p_data->m_waypointInformation->m_waypointIndex += p_data->m_waypointInformation->m_waypointStep;
		if ((int) p_data->m_waypointInformation->m_waypointCount <=
				(int) p_data->m_waypointInformation->m_waypointIndex ||
			(int) p_data->m_waypointInformation->m_waypointIndex < 0) {
			switch (p_data->m_waypointInformation->m_patrolMode) {
			case WAYPOINT_PATROL_REVERSE:
				p_data->m_waypointInformation->m_waypointStep = -p_data->m_waypointInformation->m_waypointStep;
				p_data->m_waypointInformation->m_waypointIndex += p_data->m_waypointInformation->m_waypointStep;
				break;
			case WAYPOINT_PATROL_LOOP:
				p_data->m_waypointInformation->m_waypointIndex = 0;
				break;
			}
		}
		AddDestination(destination);
	}
}

// FUNCTION: LEMBALL 0x004201a0
void CEnemy::EnemyAction_TURNANDFIRERAPID(tEnemyLemmingUnion* p_data)
{
	RequestFire(ENEMY_FIRE_RAPID_INTERVAL_MS);
}

// FUNCTION: LEMBALL 0x004201b0
void CEnemy::EnemyAction_TURNANDFIRESLOW(tEnemyLemmingUnion* p_data)
{
	RequestFire(ENEMY_FIRE_SLOW_INTERVAL_MS);
}

// FUNCTION: LEMBALL 0x004201c0
void CEnemy::EnemyAction_TURNANDFIRERANDOM(tEnemyLemmingUnion* p_data)
{
	int seed = *g_pRandomSeed;
	seed = seed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT;
	seed = seed & RANDOM_SEED_MASK;
	*g_pRandomSeed = seed;
	RequestFire(seed % ENEMY_FIRE_RANDOM_INTERVAL_RANGE_MS + ENEMY_FIRE_RANDOM_MIN_INTERVAL_MS);
}

// FUNCTION: LEMBALL 0x00420200
bool CEnemy::CheckRadius(int p_radius)
{
	CVSRect rect;
	CVSPoint* position = &rect;
	position->m_x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - p_radius;
	position->m_y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - p_radius;
	CVSSize* size = &rect;
	size->m_width = size->m_height = p_radius * 2;

	if (g_pAI->PlayerCheckGroupIntersection(&rect, &m_targetPosition) == true) {
		return true;
	}
	return g_pAI->SheepCheckGroupIntersection(&rect, &m_targetPosition) == true;
}

// FUNCTION: LEMBALL 0x004202a0
bool CEnemy::LineOfSight(AICOORD p_target)
{
	int deltaX = p_target.m_xFixed - m_position.m_xFixed;
	int deltaY = p_target.m_yFixed - m_position.m_yFixed;
	int absX = VsAbs(deltaX);
	int absY = VsAbs(deltaY);
	int low = absY & FIXED_POINT_FRACTION_MASK;
	int fraction = (low * ENEMY_LOS_MIN_RATIO_FIXED) >> FIXED_POINT_FRACTION_BITS;
	int high = absY >> FIXED_POINT_FRACTION_BITS;
	if (high * ENEMY_LOS_MIN_RATIO_FIXED + fraction < absX) {
		if ((high * ENEMY_LOS_MAX_RATIO_HALF_FIXED + low) * 2 + fraction > absX) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x00420350
void CEnemy::TurnToFaceTarget()
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
	}
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / GAME_TICK_MILLISECONDS;
}

// FUNCTION: LEMBALL 0x004203d0
bool CEnemy::IsRequestingFire()
{
	return m_fireState == ENEMY_FIRE_REQUESTED;
}

// FUNCTION: LEMBALL 0x004203e0
void CEnemy::RequestFire(int p_interval)
{
	if (g_pAI->m_gameplayEnabled != 0 && m_fireState == ENEMY_FIRE_IDLE) {
		m_fireTarget.m_xFixed = m_targetPosition.m_xFixed;
		m_fireTarget.m_yFixed = m_targetPosition.m_yFixed;
		m_fireTarget.m_zFixed = m_targetPosition.m_zFixed;
		m_fireInterval = p_interval;
		m_fireState = ENEMY_FIRE_REQUESTED;
	}
}

// FUNCTION: LEMBALL 0x00420430
void CEnemy::Fire()
{
	AICOORD start;
	start.m_xFixed = m_position.m_xFixed;
	int facing = m_facingDirection;
	start.m_yFixed = m_position.m_yFixed;
	start.m_zFixed = m_position.m_zFixed + ENEMY_MUZZLE_HEIGHT_FIXED;

	g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_ENEMY, facing, start, m_fireTarget);
	m_fireState = ENEMY_FIRE_FIRED;
	m_actionDeadline = g_dwGameTick + m_fireInterval / GAME_TICK_MILLISECONDS;
}

// FUNCTION: LEMBALL 0x004204d0
void CEnemy::StartFiring()
{
	m_actionDeadline = g_dwGameTick + GAME_OBJECT_FIRE_WINDUP_TICKS;
}

// FUNCTION: LEMBALL 0x004204e0
void CEnemy::EndFiring()
{
	m_fireState = ENEMY_FIRE_IDLE;
	int width = g_pMap->m_ground.m_width;
	int height = g_pMap->m_ground.m_height;
	int pixelWidth = width << GROUND_BLOCK_PIXEL_SHIFT;
	int pixelHeight = height << GROUND_BLOCK_PIXEL_SHIFT;
	int x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) + g_enemyFacingOffsets[m_facingDirection].m_dx;
	int y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) + g_enemyFacingOffsets[m_facingDirection].m_dy;
	if (x >= 0 && y >= 0 && x < pixelWidth && y < pixelHeight) {
		int z = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		CMap* map = g_pMap;
		int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		unsigned short groundZ;
		if (x < 0 || y < 0 || blockX >= width || blockY >= height) {
			groundZ = 0;
		}
		else {
			groundZ = map->m_ground.m_ground[blockY * width + blockX].GetZ(x & GROUND_BLOCK_PIXEL_MASK,
																		   y & GROUND_BLOCK_PIXEL_MASK);
		}
		if (groundZ == z) {
			if ((MapCheck(x, y) & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				m_position.m_xFixed = x << FIXED_POINT_FRACTION_BITS;
				m_position.m_yFixed = y << FIXED_POINT_FRACTION_BITS;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00420600
void CEnemy::HitBullet(CBullet* p_bullet)
{
	if (p_bullet->m_owner != OWNER_ENEMY) {
		m_hit = 1;
		m_actionDeadline = g_dwGameTick + ENEMY_HIT_RESPONSE_DELAY_TICKS;
		m_facingDirection = (p_bullet->m_facingDirection + FACING_DIRECTION_OPPOSITE_OFFSET) & FACING_DIRECTION_MASK;
		m_deathRequested = 1;
	}
}

// FUNCTION: LEMBALL 0x00420650
bool CEnemy::FacingTarget()
{
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
												m_fireTarget.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												m_fireTarget.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	return (unsigned int) m_facingDirection == facing;
}

// FUNCTION: LEMBALL 0x004206a0
void CEnemy::HitMine()
{
	m_wasHitByMine = 1;
	g_pAI->Score(AI_SCORE_ENEMY_HIT_POINTS);
	C3DVector velocity;
	velocity.m_xFixed = 0;
	velocity.m_yFixed = 0;
	velocity.m_zFixed = ENEMY_MINE_LAUNCH_VERTICAL_VELOCITY_FIXED;
	StartFly(velocity, NULL);
	m_deathRequested = 1;
}

// FUNCTION: LEMBALL 0x004206f0
void CEnemy::HitBall()
{
	m_hit = 1;
	m_actionDeadline = g_dwGameTick + ENEMY_HIT_RESPONSE_DELAY_TICKS;
	g_pAI->Score(AI_SCORE_ENEMY_HIT_POINTS);
}

// FUNCTION: LEMBALL 0x00420720
void CEnemy::GetHit()
{
	CAI* ai = g_pAI;
	int i;
	for (i = 0; i < ai->m_objectCount; i++) {
		if (ai->m_objects[i] == this) {
			(ai->m_objectCount)--;
			for (; i < ai->m_objectCount; i++) {
				ai->m_objects[i] = ai->m_objects[i + 1];
			}
			ai->m_objects[ai->m_objectCount] = NULL;
			break;
		}
	}
	g_pAI->Score(AI_SCORE_ENEMY_KILL_POINTS);
}

// FUNCTION: LEMBALL 0x00420aa0
int CEnemy::IsHit()
{
	return m_hit;
}
