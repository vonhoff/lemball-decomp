#include "CSlinky.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CRect3.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0040b480
CSlinky::CSlinky() : CGameObject(OBJECT_SLINKY, 0, 0), m_unk0x138(0, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0040b4d0
void CSlinky::Set(int p_minX, int p_maxX, int p_minY, int p_maxY)
{
	m_minX = p_minX;
	m_minY = p_minY;
	m_maxX = p_maxX;
	m_maxY = p_maxY;
	unsigned short z;
	CMap* map = g_pMap;
	int bx = p_minX >> 4;
	int by = p_minY >> 4;
	if (p_minX < 0 || p_minY < 0) {
		z = 0;
	}
	else {
		CMap* boundsMap = g_pMap;
		int width = boundsMap->m_ground.m_width;
		if (bx >= width || boundsMap->m_ground.m_height <= by) {
			z = 0;
		}
		else {
			p_minX &= 15;
			p_minY &= 15;
			z = map->m_ground.m_ground[width * by + bx].GetZ(p_minX, p_minY);
		}
	}
	int positionZ = z << 12;
	m_position.m_xFixed = m_minX << 12;
	m_position.m_yFixed = m_minY << 12;
	m_position.m_zFixed = positionZ;
	m_actionDeadline = g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
	Action(ACTION_READY);
}

// FUNCTION: LEMBALL 0x0040b5b0
void CSlinky::GetBounds(int* p_minX, int* p_maxX, int* p_minY, int* p_maxY)
{
	*p_minX = m_minX;
	*p_minY = m_minY;
	*p_maxX = m_maxX;
	*p_maxY = m_maxY;
}

// FUNCTION: LEMBALL 0x0040b5f0
bool CSlinky::ContainsIntegerPoint(const int* p_xy)
{
	return m_minX <= p_xy[0] && m_minY <= p_xy[1] && p_xy[0] <= m_maxX && p_xy[1] <= m_maxY;
}

// FUNCTION: LEMBALL 0x0040b630
bool CSlinky::GoodEndPt(const AICOORD& p_coordinate)
{
	int x = p_coordinate.m_xFixed >> 12;
	if (m_minX <= x) {
		int y = p_coordinate.m_yFixed >> 12;
		if (m_minY <= y && x <= m_maxX && y <= m_maxY) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0040b670
bool CSlinky::Move()
{
	enum {
		DIRECTION_POSITIVE_X = 0,
		DIRECTION_NEGATIVE_X = 1,
		DIRECTION_POSITIVE_Y = 2,
		DIRECTION_NEGATIVE_Y = 3,
		DIRECTION_COUNT = 4,
		STEP_PIXELS = 16,
		MAX_ATTEMPTS = 8,
		RANDOM_MULTIPLIER = 41,
		RANDOM_INCREMENT = 31,
		RANDOM_MASK = (1 << 23) - 1
	};
	int dx;
	int count = 0;
	int dy;
	do {
		int random = (*g_pRandomSeed * RANDOM_MULTIPLIER + RANDOM_INCREMENT) & RANDOM_MASK;
		*g_pRandomSeed = random;
		m_actionArgument = random % DIRECTION_COUNT;
		switch ((unsigned short) m_actionArgument) {
		case DIRECTION_POSITIVE_X:
			dx = STEP_PIXELS;
			dy = 0;
			break;
		case DIRECTION_NEGATIVE_X:
			dx = -STEP_PIXELS;
			dy = 0;
			break;
		case DIRECTION_POSITIVE_Y:
			dx = 0;
			dy = STEP_PIXELS;
			break;
		case DIRECTION_NEGATIVE_Y:
			dx = 0;
			dy = -STEP_PIXELS;
			break;
		}
		{
			const int x = m_position.m_xFixed >> 12;
			count++;
			m_destination.m_xFixed = (x + dx) << 12;
		}
		{
			const int y = m_position.m_yFixed >> 12;
			m_destination.m_yFixed = (y + dy) << 12;
		}
		{
			const int z = m_position.m_zFixed >> 12;
			m_destination.m_zFixed = z << 12;
		}
	} while (count < MAX_ATTEMPTS && !GoodEndPt(m_destination));
	return true;
}

// FUNCTION: LEMBALL 0x0040b760
bool CSlinky::Process()
{
	switch (m_action) {
	case ACTION_READY:
		if (g_dwGameTick >= m_actionDeadline) {
			Move();
			m_stateTimer = g_dwSimulationTimestamp;
			Action(ACTION_RUNNING);
			m_actionDeadline = g_dwGameTick + 0x10;
		}
		break;
	case ACTION_RUNNING:
		if (g_dwGameTick >= m_actionDeadline) {
			int x = m_destination.m_xFixed;
			int y = m_destination.m_yFixed;
			int z = m_destination.m_zFixed;
			m_position.m_xFixed = x;
			m_position.m_yFixed = y;
			m_position.m_zFixed = z;
			m_stateTimer = g_dwSimulationTimestamp;
			Action(ACTION_READY);
			m_actionDeadline = g_dwGameTick + 0x14;
		}
		break;
	}
	CRect3 rect;
	int z = (m_position.m_zFixed >> 12) - 4;
	int y = (m_position.m_yFixed >> 12) - 4;
	int x = (m_position.m_xFixed >> 12) - 4;
	rect.m_x1 = x;
	rect.m_y1 = y;
	rect.m_z1 = z;
	rect.m_x2 = x + 7;
	rect.m_y2 = y + 7;
	rect.m_z2 = z + 7;
	CGameObject* hit;
	CAI* ai = g_pAI;
	ai->m_collisionExclude = this;
	ai->m_collisionRect = rect;
	ai->m_rectCollisionIndex = 0;
	while (ai->m_rectCollisionIndex < ai->m_objectCount) {
		CGameObject* object = ai->m_objects[ai->m_rectCollisionIndex];
		if (object != ai->m_collisionExclude && object->Collision(ai->m_collisionRect)) {
			ai->m_rectCollisionIndex++;
			hit = object;
			goto hitObject;
		}
		ai->m_rectCollisionIndex++;
	}
	hit = NULL;
hitObject:
	if (hit) {
		hit->HitBall();
	}
	return true;
}
