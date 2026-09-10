#include "Sheep.h"

#include "../Base/StateMachine.h"
#include "../Navigation/Ai.h"

#include <string.h>

// 68K 0x1061e282 __ct__6CSheepFP3CAIiiii
// FUNCTION: LEMBALL 0x0041f990
Sheep::Sheep(Ai* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4) : GameObject((eObjectType) 7, 0x108, 0x14)
{
	g_pAI = p_arg0;
	m_spawnPosition.m_xFixed = p_arg1 << 12;
	m_initialFacingDirection = (short) p_arg4;
	m_spawnPosition.m_yFixed = p_arg2 << 12;
	m_spawnPosition.m_zFixed = p_arg3 << 12;
	SetId(NextLoadingId());
}

// 68K 0x1061e344 Restart__6CSheepFv
// FUNCTION: LEMBALL 0x0041f9f0
void Sheep::Restart()
{
	GameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	int tileX = m_spawnPosition.m_xFixed >> 12;
	int tileY = m_spawnPosition.m_yFixed >> 12;
	int tileZ = m_spawnPosition.m_zFixed >> 12;
	int collision[6];
	collision[0] = tileX - 8;
	collision[1] = tileY - 8;
	collision[2] = tileZ;
	collision[3] = tileX + 7;
	collision[4] = tileY + 7;
	collision[5] = tileZ + 15;
	memcpy(&m_collisionMinX, collision, sizeof(collision));
	m_facingDirection = m_initialFacingDirection;
	Ai* objectAi = g_pAI;
	Ai* countAi = g_pAI;
	int* objectCount = &countAi->m_objectCount;
	objectAi->m_objects[*objectCount] = this;
	(*objectCount)++;
}

// 68K 0x1061e424 Process__6CSheepFv
// FUNCTION: LEMBALL 0x0041fa90
bool Sheep::Process()
{
	SheepState(g_pAI, this);
	return 0;
}

// 68K 0x1061e454 HitBall__6CSheepFv
// FUNCTION: LEMBALL 0x0041fab0
void Sheep::HitBall()
{
	g_pAI->Score(0x96);
}

// 68K 0x1061e482 HitMine__6CSheepFv
// FUNCTION: LEMBALL 0x0041fad0
void Sheep::HitMine()
{
	g_pAI->Score(0x96);
	C3DVector velocity;
	velocity.m_xFixed = 0;
	velocity.m_yFixed = 0;
	velocity.m_zFixed = 0xa000;
	StartFly(velocity, 0);
}

// 68K 0x1011b36a __dt__6CSheepFv
Sheep::~Sheep()
{
}
