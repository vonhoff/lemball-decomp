#include "CEnemy.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../Base/StateMachine.h"
#include "../Base/tagWaypointInformation.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CPt3.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "CBullet.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/CVSSize.h"

#include <stddef.h>

#define ENEMY_FIRE_RAPID_INTERVAL 100
#define ENEMY_FIRE_SLOW_INTERVAL 800
#define ENEMY_FIRE_RANDOM_MIN_INTERVAL 150
#define ENEMY_FIRE_RANDOM_INTERVAL_RANGE 1000
#define ENEMY_RANDOM_MULTIPLIER 41
#define ENEMY_RANDOM_INCREMENT 31
#define ENEMY_RANDOM_MASK ((1 << 23) - 1)
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

// FUNCTION: LEMBALL 0x0041fba0
CEnemy::CEnemy(CAI* p_ai, int p_x, int p_y, int p_z, int p_facingDirection)
	: CGameObject(OBJECT_PLAYER_1, 0x118, 10), m_targetPosition(), m_fireTarget()
{
	unsigned short z;
	int width;
	int blockX;
	int blockY;
	CMap* map;

	g_pAI = p_ai;
	m_spawnPosition.m_xFixed = p_x << 12;
	m_spawnPosition.m_yFixed = p_y << 12;
	m_spawnPosition.m_zFixed = p_z << 12;
	map = g_pMap;
	blockX = p_x >> 4;
	blockY = p_y >> 4;
	if (p_x < 0 || p_y < 0 || g_pMap->m_ground.m_width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		width = map->m_ground.m_width;
		z = map->m_ground.m_ground[blockY * width + blockX].GetZ(p_x & 0xf, p_y & 0xf);
	}
	m_initialFacingDirection = (short) p_facingDirection;
	m_spawnPosition.m_zFixed = (int) z << 12;
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
	m_stateIndex = 0;
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
		case 0:
			ProcessAction(m_state0Rule, m_state0Action, &m_state0Data);
			break;
		case 1:
			ProcessAction(m_state1Rule, m_state1Action, &m_state1Data);
			break;
		case 2:
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
	if (ProcessRule(p_rule) == 1) {
		m_stateIndex++;
		if (m_stateIndex > 2) {
			m_stateIndex = 0;
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
	if (DestinationExists() != 1) {
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
	RequestFire(ENEMY_FIRE_RAPID_INTERVAL);
}

// FUNCTION: LEMBALL 0x004201b0
void CEnemy::EnemyAction_TURNANDFIRESLOW(tEnemyLemmingUnion* p_data)
{
	RequestFire(ENEMY_FIRE_SLOW_INTERVAL);
}

// FUNCTION: LEMBALL 0x004201c0
void CEnemy::EnemyAction_TURNANDFIRERANDOM(tEnemyLemmingUnion* p_data)
{
	int seed = *g_pRandomSeed;
	seed = seed * ENEMY_RANDOM_MULTIPLIER + ENEMY_RANDOM_INCREMENT;
	seed = seed & ENEMY_RANDOM_MASK;
	*g_pRandomSeed = seed;
	RequestFire(seed % ENEMY_FIRE_RANDOM_INTERVAL_RANGE + ENEMY_FIRE_RANDOM_MIN_INTERVAL);
}

// FUNCTION: LEMBALL 0x00420200
bool CEnemy::CheckRadius(int p_radius)
{
	CVSRect rect;
	CVSPoint* position = &rect;
	position->m_x = (m_position.m_xFixed >> 12) - p_radius;
	position->m_y = (m_position.m_yFixed >> 12) - p_radius;
	CVSSize* size = &rect;
	size->m_width = size->m_height = p_radius * 2;

	if (g_pAI->PlayerCheckGroupIntersection(&rect, &m_targetPosition) == 1) {
		return true;
	}
	return g_pAI->SheepCheckGroupIntersection(&rect, &m_targetPosition) == 1;
}

// FUNCTION: LEMBALL 0x004202a0
bool CEnemy::LineOfSight(AICOORD p_target)
{
	int deltaX = p_target.m_xFixed - m_position.m_xFixed;
	int deltaY = p_target.m_yFixed - m_position.m_yFixed;
	int absX = VsAbs(deltaX);
	int absY = VsAbs(deltaY);
	int low = absY & 0xfff;
	int fraction = (low * 0x6a0) >> 12;
	int high = absY >> 12;
	if (high * 0x6a0 + fraction < absX) {
		if ((high * 0x1350 + low) * 2 + fraction > absX) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x00420350
void CEnemy::TurnToFaceTarget()
{
	int facing = ReturnFacingDirection(m_position.m_xFixed >> 12,
									   m_position.m_yFixed >> 12,
									   m_fireTarget.m_xFixed >> 12,
									   m_fireTarget.m_yFixed >> 12);
	if (facing != m_facingDirection) {
		if (g_anRotationDirections[(facing - m_facingDirection) & 7] < 0) {
			RotateAnticlockwise();
		}
		else {
			RotateClockwise();
		}
	}
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / 50;
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
	m_actionDeadline = g_dwGameTick + 10;
}

// FUNCTION: LEMBALL 0x004204e0
void CEnemy::EndFiring()
{
	m_fireState = ENEMY_FIRE_IDLE;
	int width = g_pMap->m_ground.m_width;
	int height = g_pMap->m_ground.m_height;
	int x = (m_position.m_xFixed >> 12) + g_enemyFacingOffsets[m_facingDirection].m_dx;
	int y = (m_position.m_yFixed >> 12) + g_enemyFacingOffsets[m_facingDirection].m_dy;
	if (x >= 0 && y >= 0 && x < (width << 4) && y < (height << 4)) {
		if (g_pMap->m_ground.GetZ(x, y) == (m_position.m_zFixed >> 12)) {
			if ((MapCheck(x, y) & 1) == 0) {
				m_position.m_xFixed = x << 12;
				m_position.m_yFixed = y << 12;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00420600
void CEnemy::HitBullet(CBullet* p_bullet)
{
	if (p_bullet->m_owner != OWNER_ENEMY) {
		m_hit = 1;
		m_actionDeadline = g_dwGameTick + 60;
		m_facingDirection = (p_bullet->m_facingDirection + 4) & 7;
		m_deathRequested = 1;
	}
}

// FUNCTION: LEMBALL 0x00420650
bool CEnemy::FacingTarget()
{
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> 12,
												m_position.m_yFixed >> 12,
												m_fireTarget.m_xFixed >> 12,
												m_fireTarget.m_yFixed >> 12);
	return (unsigned int) m_facingDirection == facing;
}

// FUNCTION: LEMBALL 0x004206a0
void CEnemy::HitMine()
{
	m_wasHitByMine = 1;
	g_pAI->Score(300);
	C3DVector velocity;
	velocity.m_xFixed = 0;
	velocity.m_yFixed = 0;
	velocity.m_zFixed = 0xa000;
	StartFly(velocity, NULL);
	m_deathRequested = 1;
}

// FUNCTION: LEMBALL 0x004206f0
void CEnemy::HitBall()
{
	m_hit = 1;
	m_actionDeadline = g_dwGameTick + 60;
	g_pAI->Score(300);
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
	g_pAI->Score(500);
}

// FUNCTION: LEMBALL 0x00420aa0
int CEnemy::IsHit()
{
	return m_hit;
}
