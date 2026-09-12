#include "Slinky.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"

// 68K 0x1061ec2c __ct__7CSlinkyFv
// FUNCTION: LEMBALL 0x0040b480
Slinky::Slinky() : GameObject((eObjectType) 0x35, 0, 0), m_unk0x138(0, 0, 0)
{
}

// 68K 0x1061ecc4 Set__7CSlinkyFiiii
// FUNCTION: LEMBALL 0x0040b4d0
void Slinky::Set(int p_minX, int p_maxX, int p_minY, int p_maxY)
{
	m_minX = p_minX;
	m_minY = p_minY;
	m_maxX = p_maxX;
	m_maxY = p_maxY;
	unsigned short z;
	Map* map = g_pMap;
	int bx = p_minX >> 4;
	int by = p_minY >> 4;
	if (p_minX >= 0 && p_minY >= 0 && bx < map->m_ground.m_width && by < map->m_ground.m_height) {
		z = map->m_ground.m_ground[map->m_ground.m_width * by + bx].GetZ(p_minX & 15, p_minY & 15);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << 12;
	m_position.m_xFixed = m_minX << 12;
	m_position.m_yFixed = m_minY << 12;
	m_actionDeadline = g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
	Action(0x18);
}

// 68K 0x1061eda0 GoodEndPt__7CSlinkyFRC7AICOORD
// FUNCTION: LEMBALL 0x0040b630
bool Slinky::GoodEndPt(const AiCoord& p_coordinate)
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

// 68K 0x1061ee14 Move__7CSlinkyFv
// FUNCTION: LEMBALL 0x0040b670
bool Slinky::Move()
{
	int count = 0;
	int dx;
	int dy;
	do {
		int random = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
		*g_pSentinel = random;
		m_actionArgument = random % 4;
		switch ((unsigned short) m_actionArgument) {
		case 0:
			dx = 16;
			dy = 0;
			break;
		case 1:
			dx = -16;
			dy = 0;
			break;
		case 2:
			dx = 0;
			dy = 16;
			break;
		case 3:
			dx = 0;
			dy = -16;
			break;
		}
		int x = m_position.m_xFixed >> 12;
		int y = m_position.m_yFixed >> 12;
		int z = m_position.m_zFixed >> 12;
		count++;
		m_destination.m_xFixed = (x + dx) << 12;
		m_destination.m_yFixed = (y + dy) << 12;
		m_destination.m_zFixed = z << 12;
	} while (count < 8 && !GoodEndPt(m_destination));
	return true;
}

// 68K 0x1061eef4 Process__7CSlinkyFv
// FUNCTION: LEMBALL 0x0040b760
bool Slinky::Process()
{
	Ai* ai;
	switch (m_action) {
	case 0x18:
		if (g_dwGameTick >= m_actionDeadline) {
			Move();
			m_stateTimer = g_dwSimulationTimestamp;
			Action(0x1b);
			m_actionDeadline = g_dwGameTick + 0x10;
		}
		break;
	case 0x1b:
		if (g_dwGameTick >= m_actionDeadline) {
			int x = m_destination.m_xFixed;
			int y = m_destination.m_yFixed;
			int z = m_destination.m_zFixed;
			m_position.m_xFixed = x;
			m_position.m_yFixed = y;
			m_position.m_zFixed = z;
			m_stateTimer = g_dwSimulationTimestamp;
			Action(0x18);
			m_actionDeadline = g_dwGameTick + 0x14;
		}
		break;
	}
	Rect3 rect;
	int z = (m_position.m_zFixed >> 12) - 4;
	int y = (m_position.m_yFixed >> 12) - 4;
	int x = (m_position.m_xFixed >> 12) - 4;
	rect.m_x1 = x;
	rect.m_y1 = y;
	rect.m_z1 = z;
	rect.m_x2 = x + 7;
	rect.m_y2 = y + 7;
	rect.m_z2 = z + 7;
	ai = g_pAI;
	ai->m_collisionExclude = this;
	ai->m_collisionRect = rect;
	ai->m_rectCollisionIndex = 0;
	GameObject* hit;
	while (ai->m_rectCollisionIndex < ai->m_objectCount) {
		GameObject* object = ai->m_objects[ai->m_rectCollisionIndex];
		if (object != ai->m_collisionExclude && object->Collision(ai->m_collisionRect)) {
			ai->m_rectCollisionIndex++;
			hit = object;
			goto hitObject;
		}
		ai->m_rectCollisionIndex++;
	}
	hit = 0;
hitObject:
	if (hit) {
		hit->HitBall();
	}
	return true;
}

// 68K 0x1011b484 __dt__7CSlinkyFv
Slinky::~Slinky()
{
}
