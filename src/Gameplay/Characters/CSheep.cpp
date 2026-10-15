#include "CSheep.h"

#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Behavior/StateMachine.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Geometry/CRect3.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"

#include <stddef.h>

enum {
	SHEEP_DESTINATION_CAPACITY = 20,
	SHEEP_MINE_LAUNCH_VERTICAL_VELOCITY_FIXED = 10 * FIXED_POINT_ONE
};

// FUNCTION: LEMBALL 0x0041f990
CSheep::CSheep(CAI* p_ai, int p_x, int p_y, int p_z, int p_facingDirection)
	: CGameObject(OBJECT_SHEEP, GAME_OBJECT_COLLISION_SHEEP, SHEEP_DESTINATION_CAPACITY)
{
	g_pAI = p_ai;
	m_spawnPosition.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
	m_initialFacingDirection = (short) p_facingDirection;
	SetId(NextLoadingId());
}

// FUNCTION: LEMBALL 0x0041f9f0
void CSheep::Restart()
{
	CGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
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
	m_facingDirection = m_initialFacingDirection;
	CAI* objectAi = g_pAI;
	CAI* countAi = g_pAI;
	int* objectCount = &countAi->m_objectCount;
	objectAi->m_objects[*objectCount] = this;
	(*objectCount)++;
}

// FUNCTION: LEMBALL 0x0041fa90
bool CSheep::Process()
{
	SheepState(g_pAI, this);
	return false;
}

// FUNCTION: LEMBALL 0x0041fab0
void CSheep::HitBall()
{
	g_pAI->Score(AI_SCORE_SHEEP_HIT_POINTS);
}

// FUNCTION: LEMBALL 0x0041fad0
void CSheep::HitMine()
{
	g_pAI->Score(AI_SCORE_SHEEP_HIT_POINTS);
	C3DVector velocity;
	velocity.m_xFixed.m_value = 0;
	velocity.m_yFixed.m_value = 0;
	velocity.m_zFixed.m_value = SHEEP_MINE_LAUNCH_VERTICAL_VELOCITY_FIXED;
	StartFly(velocity, NULL);
}
