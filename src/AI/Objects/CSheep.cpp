#include "CSheep.h"

#include "../Base/StateMachine.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectTypes.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0041f990
CSheep::CSheep(CAI* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4) : CGameObject(OBJECT_SHEEP, 0x108, 0x14)
{
	g_pAI = p_arg0;
	m_spawnPosition.m_xFixed = p_arg1 << 12;
	m_initialFacingDirection = (short) p_arg4;
	m_spawnPosition.m_yFixed = p_arg2 << 12;
	m_spawnPosition.m_zFixed = p_arg3 << 12;
	SetId(NextLoadingId());
}

// FUNCTION: LEMBALL 0x0041f9f0
void CSheep::Restart()
{
	CGameObject::Restart();
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
	memcpy(&m_collisionBounds, collision, sizeof(collision));
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
	return 0;
}

// FUNCTION: LEMBALL 0x0041fab0
void CSheep::HitBall()
{
	g_pAI->Score(0x96);
}

// FUNCTION: LEMBALL 0x0041fad0
void CSheep::HitMine()
{
	g_pAI->Score(0x96);
	C3DVector velocity;
	velocity.m_xFixed = 0;
	velocity.m_yFixed = 0;
	velocity.m_zFixed = 0xa000;
	StartFly(velocity, 0);
}
