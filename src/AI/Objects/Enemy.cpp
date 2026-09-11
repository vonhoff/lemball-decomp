#include "Enemy.h"

#include "../../Control/Game/Game.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../Base/StateMachine.h"
#include "../Base/WaypointInformation.h"
#include "../Navigation/Ai.h"
#include "Bullet.h"

struct EnemyFacingOffset {
	int m_dx;
	int m_dy;
};

// GLOBAL: LEMBALL 0x004950c0
EnemyFacingOffset g_enemyFacingOffsets[8] = {{0, 3}, {-4, 1}, {-5, 0}, {-4, -3}, {0, -4}, {6, -3}, {5, 0}, {4, 1}};

// 68K 0x10607884 __ct__6CEnemyFP3CAIiiii
// FUNCTION: LEMBALL 0x0041fba0
Enemy::Enemy(Ai* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4)
	: GameObject((eObjectType) 1, 0x118, 10), m_targetPosition(), m_fireTarget()
{
	unsigned short z;
	int width;
	int blockX;
	int blockY;
	Map* map;

	g_pAI = p_arg0;
	m_spawnPosition.m_xFixed = p_arg1 << 12;
	m_spawnPosition.m_yFixed = p_arg2 << 12;
	m_spawnPosition.m_zFixed = p_arg3 << 12;
	map = g_pMap;
	blockX = p_arg1 >> 4;
	blockY = p_arg2 >> 4;
	if (p_arg1 < 0 || p_arg2 < 0 || g_pMap->m_ground.m_width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		width = map->m_ground.m_width;
		z = map->m_ground.m_ground[blockY * width + blockX].GetZ(p_arg1 & 0xf, p_arg2 & 0xf);
	}
	m_initialFacingDirection = (short) p_arg4;
	m_spawnPosition.m_zFixed = (int) z << 12;
	SetId((unsigned short) NextLoadingId());
	m_state2Action = (eEnemyStateActions) 0;
	m_state1Action = (eEnemyStateActions) 0;
	m_state0Action = (eEnemyStateActions) 0;
	m_state2Rule = (eEnemyStateRules) 0;
	m_state1Rule = (eEnemyStateRules) 0;
	m_state0Rule = (eEnemyStateRules) 0;
	m_state0Data.m_waypointInformation = 0;
	m_state1Data.m_waypointInformation = 0;
	m_state2Data.m_waypointInformation = 0;
}

// 68K 0x106079d6 Restart__6CEnemyFv
// FUNCTION: LEMBALL 0x0041fcd0
void Enemy::Restart()
{
	GameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_facingDirection = m_initialFacingDirection;
	m_stateIndex = 0;
	m_fireState = 0;
	m_hit = 0;
	m_unk0x2c = 0;

	int* objectCount = &g_pAI->m_objectCount;
	g_pAI->m_objects[*objectCount] = this;
	(*objectCount)++;

	if (m_state0Data.m_waypointInformation != 0) {
		m_state0Data.m_waypointInformation->m_value = 0;
		m_state0Data.m_waypointInformation->m_signedValue = 1;
	}
	if (m_state1Data.m_waypointInformation != 0) {
		m_state1Data.m_waypointInformation->m_value = 0;
		m_state1Data.m_waypointInformation->m_signedValue = 1;
	}
	if (m_state2Data.m_waypointInformation != 0) {
		m_state2Data.m_waypointInformation->m_value = 0;
		m_state2Data.m_waypointInformation->m_signedValue = 1;
	}
}

// clang-format off
// 68K 0x10607b5e SetEnemyType__6CEnemyF18eEnemyStateActions16eEnemyStateRules18eEnemyStateActions16eEnemyStateRules18eEnemyStateActions16eEnemyStateRules
// clang-format on
// FUNCTION: LEMBALL 0x0041fe30
void Enemy::SetEnemyType(eEnemyStateActions p_action0,
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

// 68K 0x10607c1a Process__6CEnemyFv
// FUNCTION: LEMBALL 0x0041fec0
bool Enemy::Process()
{
	if (m_action != (eAction) 8) {
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

// 68K 0x10607cd2 ProcessAction__6CEnemyF16eEnemyStateRules18eEnemyStateActionsP18tEnemyLemmingUnion
// FUNCTION: LEMBALL 0x0041ff60
void Enemy::ProcessAction(eEnemyStateRules p_rule, eEnemyStateActions p_action, EnemyLemmingUnion* p_data)
{
	if (ProcessRule(p_rule) == 1) {
		m_stateIndex++;
		if (m_stateIndex > 2) {
			m_stateIndex = 0;
		}
	}
	else {
		switch (p_action) {
		case 1:
			EnemyActionPatrol(p_data);
			break;
		case 2:
			EnemyActionTurnAndFireRapid(p_data);
			break;
		case 3:
			EnemyActionTurnAndFireSlow(p_data);
			break;
		case 4:
			EnemyActionTurnAndFireRandom(p_data);
			break;
		default:
			StopMoving();
			break;
		}
	}
}

// 68K 0x10607dce ProcessRule__6CEnemyF16eEnemyStateRules
// FUNCTION: LEMBALL 0x00420000
bool Enemy::ProcessRule(eEnemyStateRules p_rule)
{
	switch (p_rule) {
	case 0:
		return 1;
	case 2:
		return EnemyRuleRadius50();
	case 3:
		return EnemyRuleRadius50() == 0;
	case 4:
		return EnemyRuleRadius50AndLineOfSight();
	case 5:
		return EnemyRuleRadius50AndLineOfSight() == 0;
	default:
		return 0;
	}
}

// 68K 0x10607e80 EnemyRule_RADIUS50__6CEnemyFv
// FUNCTION: LEMBALL 0x00420070
bool Enemy::EnemyRuleRadius50()
{
	if (g_pAI->m_playerGroups == 0) {
		return 0;
	}
	return CheckRadius(50);
}

// 68K 0x10607ed4 EnemyRule_RADIUS50ANDLINEOFSIGHT__6CEnemyFv
// FUNCTION: LEMBALL 0x00420090
bool Enemy::EnemyRuleRadius50AndLineOfSight()
{
	if (g_pAI->m_playerGroups == 0) {
		return 0;
	}
	int inRadius = CheckRadius(50);
	if (inRadius == 0) {
		return 0;
	}
	inRadius &= LineOfSight(m_targetPosition);
	return inRadius;
}

// 68K 0x10607f6a EnemyAction_PATROL__6CEnemyFP18tEnemyLemmingUnion
// FUNCTION: LEMBALL 0x004200f0
void Enemy::EnemyActionPatrol(EnemyLemmingUnion* p_data)
{
	AiCoord destination;
	if (DestinationExists() != 1) {
		const Pt3& position =
			g_pAI->GetNodePosition(p_data->m_waypointInformation->m_waypoints[p_data->m_waypointInformation->m_value]);
		destination.m_xFixed = position.m_x;
		destination.m_yFixed = position.m_y;
		destination.m_zFixed = position.m_z;

		p_data->m_waypointInformation->m_value += p_data->m_waypointInformation->m_signedValue;
		if ((int) p_data->m_waypointInformation->m_waypointCount <= (int) p_data->m_waypointInformation->m_value ||
			(int) p_data->m_waypointInformation->m_value < 0) {
			switch (p_data->m_waypointInformation->m_action) {
			case 0:
				p_data->m_waypointInformation->m_signedValue = -p_data->m_waypointInformation->m_signedValue;
				p_data->m_waypointInformation->m_value += p_data->m_waypointInformation->m_signedValue;
				break;
			case 1:
				p_data->m_waypointInformation->m_value = 0;
				break;
			}
		}
		AddDestination(destination);
	}
}

// 68K 0x1060807e EnemyAction_TURNANDFIRERAPID__6CEnemyFP18tEnemyLemmingUnion
// FUNCTION: LEMBALL 0x004201a0
void Enemy::EnemyActionTurnAndFireRapid(EnemyLemmingUnion* p_data)
{
	RequestFire(0x64);
}

// 68K 0x106080d4 EnemyAction_TURNANDFIRESLOW__6CEnemyFP18tEnemyLemmingUnion
// FUNCTION: LEMBALL 0x004201b0
void Enemy::EnemyActionTurnAndFireSlow(EnemyLemmingUnion* p_data)
{
	RequestFire(0x320);
}

// 68K 0x10608128 EnemyAction_TURNANDFIRERANDOM__6CEnemyFP18tEnemyLemmingUnion
// FUNCTION: LEMBALL 0x004201c0
void Enemy::EnemyActionTurnAndFireRandom(EnemyLemmingUnion* p_data)
{
	int seed = *g_pSentinel;
	seed = seed * 41 + 0x1f;
	seed = seed & 0x7fffff;
	*g_pSentinel = seed;
	RequestFire(seed % 1000 + 0x96);
}

// 68K 0x106081b0 CheckRadius__6CEnemyFi
// FUNCTION: LEMBALL 0x00420200
bool Enemy::CheckRadius(int p_radius)
{
	VsRect rect;
	rect.m_x = (m_position.m_xFixed >> 12) - p_radius;
	rect.m_y = (m_position.m_yFixed >> 12) - p_radius;
	rect.m_width = rect.m_height = p_radius * 2;

	if (g_pAI->PlayerCheckGroupIntersection(&rect, &m_targetPosition) == 1) {
		return 1;
	}
	return g_pAI->SheepCheckGroupIntersection(&rect, &m_targetPosition) == 1;
}

// 68K 0x1060827a LineOfSight__6CEnemyF7AICOORD
// FUNCTION: LEMBALL 0x004202a0
bool Enemy::LineOfSight(AiCoord p_target)
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
			return 1;
		}
	}
	return 0;
}

// 68K 0x10608396 TurnToFaceTarget__6CEnemyFv
// FUNCTION: LEMBALL 0x00420350
void Enemy::TurnToFaceTarget()
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

// 68K 0x1060846a IsRequestingFire__6CEnemyFv
// FUNCTION: LEMBALL 0x004203d0
bool Enemy::IsRequestingFire()
{
	return m_fireState == 1;
}

// 68K 0x106084a0 RequestFire__6CEnemyFi
// FUNCTION: LEMBALL 0x004203e0
void Enemy::RequestFire(int p_interval)
{
	if (g_pAI->m_playerGroups != 0 && m_fireState == 0) {
		m_fireTarget.m_xFixed = m_targetPosition.m_xFixed;
		m_fireTarget.m_yFixed = m_targetPosition.m_yFixed;
		m_fireTarget.m_zFixed = m_targetPosition.m_zFixed;
		m_fireInterval = p_interval;
		m_fireState = 1;
	}
}

// 68K 0x10608502 Fire__6CEnemyFv
// FUNCTION: LEMBALL 0x00420430
void Enemy::Fire()
{
	AiCoord start;
	start.m_xFixed = m_position.m_xFixed;
	int facing = m_facingDirection;
	start.m_yFixed = m_position.m_yFixed;
	start.m_zFixed = m_position.m_zFixed + 0xc000;

	g_pAI->FireBullet(m_linkedObjectId, (eBulletType) 0, (eOwner) 1, facing, start, m_fireTarget);
	m_fireState = 2;
	m_actionDeadline = g_dwGameTick + m_fireInterval / 50;
}

// 68K 0x106085c0 StartFiring__6CEnemyFv
// FUNCTION: LEMBALL 0x004204d0
void Enemy::StartFiring()
{
	m_actionDeadline = g_dwGameTick + 10;
}

// 68K 0x106085f2 EndFiring__6CEnemyFv
// FUNCTION: LEMBALL 0x004204e0
void Enemy::EndFiring()
{
	m_fireState = 0;
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

// 68K 0x106086d0 HitBullet__6CEnemyFP7CBullet
// FUNCTION: LEMBALL 0x00420600
void Enemy::HitBullet(Bullet* p_bullet)
{
	if (p_bullet->m_owner != 1) {
		m_hit = 1;
		m_actionDeadline = g_dwGameTick + 60;
		m_facingDirection = (p_bullet->m_facingDirection + 4) & 7;
		m_unk0x2c = 1;
	}
}

// 68K 0x10608732 FacingTarget__6CEnemyFv
// FUNCTION: LEMBALL 0x00420650
bool Enemy::FacingTarget()
{
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> 12,
												m_position.m_yFixed >> 12,
												m_fireTarget.m_xFixed >> 12,
												m_fireTarget.m_yFixed >> 12);
	return (unsigned int) m_facingDirection == facing;
}

// 68K 0x106087b4 HitMine__6CEnemyFv
// FUNCTION: LEMBALL 0x004206a0
void Enemy::HitMine()
{
	m_unk0x10c = 1;
	g_pAI->Score(300);
	C3DVector velocity;
	velocity.m_xFixed = 0;
	velocity.m_yFixed = 0;
	velocity.m_zFixed = 0xa000;
	StartFly(velocity, 0);
	m_unk0x2c = 1;
}

// 68K 0x10608820 HitBall__6CEnemyFv
// FUNCTION: LEMBALL 0x004206f0
void Enemy::HitBall()
{
	m_hit = 1;
	m_actionDeadline = g_dwGameTick + 60;
	g_pAI->Score(300);
}

// 68K 0x10608864 GetHit__6CEnemyFv
// FUNCTION: LEMBALL 0x00420720
void Enemy::GetHit()
{
	int* count = &g_pAI->m_objectCount;
	int i;
	for (i = 0; i < *count; i++) {
		GameObject**& objects = g_pAI->m_objects;
		if (objects[i] == (GameObject*) this) {
			(*count)--;
			for (; i < *count; i++) {
				objects[i] = objects[i + 1];
			}
			objects[*count] = 0;
			break;
		}
	}
	g_pAI->Score(500);
}

// 68K 0x10119cdc IsHit__6CEnemyFv
// FUNCTION: LEMBALL 0x00420aa0
int Enemy::IsHit()
{
	return m_hit;
}

// 68K 0x10607a9e __dt__6CEnemyFv
Enemy::~Enemy()
{
}
