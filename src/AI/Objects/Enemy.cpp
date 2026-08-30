#include "Enemy.h"

#include "../../Control/Game/Game.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../Navigation/Ai.h"

struct EnemyFacingOffset {
	int m_dx;
	int m_dy;
};

// GLOBAL: LEMBALL 0x004950c0
EnemyFacingOffset g_enemyFacingOffsets[8] = {{0, 3}, {-4, 1}, {-5, 0}, {-4, -3}, {0, -4}, {6, -3}, {5, 0}, {4, 1}};

// 68K 0x10607884 __ct__6CEnemyFP3CAIiiii
// STUB: LEMBALL 0x0041fba0
Enemy::Enemy(Ai* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4)
{
}

// 68K 0x106079d6 Restart__6CEnemyFv
// STUB: LEMBALL 0x0041fcd0
void Enemy::Restart()
{
}

// 68K 0x10607b5e
// SetEnemyType__6CEnemyF18eEnemyStateActions16eEnemyStateRules18eEnemyStateActions16eEnemyStateRules18eEnemyStateActions16eEnemyStateRules
// STUB: LEMBALL 0x0041fe30
void Enemy::SetEnemyType(eEnemyStateActions p_action0,
						 eEnemyStateRules p_rule0,
						 eEnemyStateActions p_action1,
						 eEnemyStateRules p_rule1,
						 eEnemyStateActions p_action2,
						 eEnemyStateRules p_rule2)
{
}

// 68K 0x10607c1a Process__6CEnemyFv
// STUB: LEMBALL 0x0041fec0
bool Enemy::Process()
{
	return 0;
}

// 68K 0x10607cd2 ProcessAction__6CEnemyF16eEnemyStateRules18eEnemyStateActionsP18tEnemyLemmingUnion
// STUB: LEMBALL 0x0041ff60
void Enemy::ProcessAction(eEnemyStateRules p_rule, eEnemyStateActions p_action, EnemyLemmingUnion* p_data)
{
}

// 68K 0x10607dce ProcessRule__6CEnemyF16eEnemyStateRules
// STUB: LEMBALL 0x00420000
bool Enemy::ProcessRule(eEnemyStateRules p_rule)
{
	return 0;
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
	return inRadius & LineOfSight(m_targetPosition);
}

// 68K 0x10607f6a EnemyAction_PATROL__6CEnemyFP18tEnemyLemmingUnion
// STUB: LEMBALL 0x004200f0
void Enemy::EnemyActionPatrol(EnemyLemmingUnion* p_data)
{
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
// STUB: LEMBALL 0x00420200
bool Enemy::CheckRadius(int p_radius)
{
	return 0;
}

// 68K 0x1060827a LineOfSight__6CEnemyF7AICOORD
// STUB: LEMBALL 0x004202a0
bool Enemy::LineOfSight(AiCoord p_target)
{
	return 0;
}

// 68K 0x10608396 TurnToFaceTarget__6CEnemyFv
// STUB: LEMBALL 0x00420350
void Enemy::TurnToFaceTarget()
{
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
// STUB: LEMBALL 0x00420430
bool Enemy::Fire()
{
	return 0;
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
// STUB: LEMBALL 0x00420600
void Enemy::HitBullet(Bullet* p_bullet)
{
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
// STUB: LEMBALL 0x004206a0
void Enemy::HitMine()
{
}

// 68K 0x10608820 HitBall__6CEnemyFv
// STUB: LEMBALL 0x004206f0
void Enemy::HitBall()
{
}

// 68K 0x10608864 GetHit__6CEnemyFv
// STUB: LEMBALL 0x00420720
void Enemy::GetHit()
{
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
