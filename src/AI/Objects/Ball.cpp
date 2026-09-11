#include "Ball.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/Vector.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../Base/Pt3.h"
#include "../Managers/BallManager.h"
#include "../Navigation/Ai.h"

// 68K 0x1060364a __ct__5CBallFv
// FUNCTION: LEMBALL 0x00421660
Ball::Ball() : GameObject(9, 0, 0)
{
}

// 68K 0x106036a8 Restart__5CBallFv
// FUNCTION: LEMBALL 0x00421690
void Ball::Restart()
{
	GameObject::Restart();
	m_action = (eAction) 0x25;
	m_speed = g_anTurnDelayCursor[m_objectType];
}

// 68K 0x106036f0 Set__5CBallF7AICOORD7AICOORDi
// FUNCTION: LEMBALL 0x004216c0
void Ball::Set(AiCoord p_start, AiCoord p_destination, int p_speed)
{
	m_position.m_xFixed = p_start.m_xFixed;
	m_position.m_yFixed = p_start.m_yFixed;
	m_position.m_zFixed = p_start.m_zFixed;
	m_spawnPosition.m_xFixed = p_start.m_xFixed;
	m_spawnPosition.m_yFixed = p_start.m_yFixed;
	m_spawnPosition.m_zFixed = p_start.m_zFixed;
	m_destination.m_xFixed = p_destination.m_xFixed;
	m_destination.m_yFixed = p_destination.m_yFixed;
	m_destination.m_zFixed = p_destination.m_zFixed;
	m_action = (eAction) 0x25;
	m_actionArgument = 0;
	unsigned short* speed = &m_speed;
	if (g_pAI->m_levelVersion < 7) {
		*speed = (unsigned short) g_anTurnDelayCursor[m_objectType];
	}
	else {
		*speed = (unsigned short) p_speed;
	}
	if (*speed <= 1) {
		*speed = (unsigned short) g_anTurnDelayCursor[m_objectType];
	}
	m_enabled = 1;
}

// 68K 0x106037c0 StartMovement__5CBallFUc
// FUNCTION: LEMBALL 0x00421770
void Ball::StartMovement(unsigned char p_direction)
{
	m_direction = p_direction;

	int targetX;
	int targetY;
	if (p_direction != 0) {
		targetX = m_destination.m_xFixed;
		targetY = m_destination.m_yFixed;
	}
	else {
		targetX = m_spawnPosition.m_xFixed;
		targetY = m_spawnPosition.m_yFixed;
	}

	int distance = Distance(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, targetX >> 12, targetY >> 12);
	m_lastMovementTick = g_dwGameTick;
	m_moveDurationTicks = (m_speed * distance) / 50;
	if (m_moveDurationTicks == 0) {
		m_moveDurationTicks = 1;
	}
	m_actionDeadline = m_moveDurationTicks + g_dwGameTick;
	m_moveStartXFixed = m_position.m_xFixed;
	m_moveStartYFixed = m_position.m_yFixed;
	m_moveDeltaXFixed = targetX - m_moveStartXFixed;
	m_moveDeltaYFixed = targetY - m_moveStartYFixed;
}

// 68K 0x10603926 Move__5CBallFv
// FUNCTION: LEMBALL 0x00421870
bool Ball::Move()
{
	int elapsed = (int) (g_dwGameTick - m_lastMovementTick);
	int duration = m_moveDurationTicks;
	unsigned int z;
	int x;
	int y;
	{
		Vector delta(m_moveDeltaXFixed, m_moveDeltaYFixed);
		Vector movement = delta * elapsed;
		movement.m_xFixed /= duration;
		movement.m_yFixed /= duration;
		x = (m_moveStartXFixed + movement.m_xFixed) >> 12;
		y = (m_moveStartYFixed + movement.m_yFixed) >> 12;
	}

	Map* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short groundZ;
	if (x < 0 || y < 0 || map->m_ground.m_width <= blockX || map->m_ground.m_height <= blockY) {
		groundZ = 0;
	}
	else {
		int groundX = x & 0xf;
		int groundY = y & 0xf;
		groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(groundX, groundY);
	}
	z = groundZ;

	Ai* ai = g_pAI;
	Pt3 point;
	point.m_x = x;
	point.m_y = y;
	point.m_z = z;
	ai->m_collisionExclude = this;
	ai->m_collisionPoint = point;
	ai->m_collisionIndex = 0;
	GameObject* hit;
	if (ai->m_objectCount > 0) {
		do {
			GameObject* object = ai->m_objects[ai->m_collisionIndex];
			if (ai->m_collisionExclude != object && object->Collision(ai->m_collisionPoint)) {
				hit = ai->m_objects[ai->m_collisionIndex];
				ai->m_collisionIndex++;
				goto found;
			}
			ai->m_collisionIndex++;
		} while (ai->m_collisionIndex < ai->m_objectCount);
	}
	hit = 0;
found:
	if (hit != 0) {
		hit->HitBall();
		m_action = (eAction) 0x26;
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionDeadline = g_dwGameTick + 0x16;
		return 1;
	}

	if ((m_position.m_zFixed >> 12) + 12 < (int) z) {
		switch ((unsigned short) m_actionArgument) {
		case 2:
			m_actionArgument = 5;
			StartMovement(0);
			return 1;
		case 5:
			m_actionArgument = 2;
			StartMovement(1);
			return 1;
		}
		return 1;
	}

	m_position.m_zFixed = (unsigned int) z << 12;
	m_position.m_xFixed = x << 12;
	m_position.m_yFixed = y << 12;
	return 1;
}

// 68K 0x10603a98 HitBullet__5CBallFP7CBullet
// FUNCTION: LEMBALL 0x00421aa0
void Ball::HitBullet(Bullet* p_bullet)
{
	Delete();
}

// 68K 0x10603ace Delete__5CBallFv
// FUNCTION: LEMBALL 0x00421ab0
void Ball::Delete()
{
	int& objectCount = g_pAI->m_objectCount;
	for (int i = 0; i < objectCount; i++) {
		GameObject**& objects = g_pAI->m_objects;
		if (objects[i] == this) {
			objectCount--;
			for (; i < objectCount; i++) {
				objects[i] = objects[i + 1];
			}
			objects[objectCount] = 0;
			break;
		}
	}
	g_pBallManager->Delete(this);
	SetId(0xffff);
}

// 68K 0x10603b26 SetHeightCorrect__5CBallFv
// FUNCTION: LEMBALL 0x00421b40
void Ball::SetHeightCorrect()
{
	int x = m_position.m_xFixed >> 12;
	int y = m_position.m_yFixed >> 12;
	Map* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (unsigned int) z << 12;
}

// 68K 0x10603b92 Process__5CBallFv
// STUB: LEMBALL 0x00421bc0
bool Ball::Process()
{
	return 0;
}

// 68K 0x10603d56 LoadLevel__5CBallFRPUc
// FUNCTION: LEMBALL 0x00421da0
void Ball::LoadLevel(unsigned char*& p_data)
{
	AiCoord start;
	AiCoord destination;

	if (g_pAI->m_levelVersion > 1) {
		unsigned short id = *(unsigned short*) p_data;
		p_data += 2;
		SetId(id);
	}

	start.m_xFixed = *(unsigned short*) p_data << 12;
	p_data += 2;
	start.m_yFixed = *(unsigned short*) p_data << 12;
	p_data += 2;
	start.m_zFixed = *(unsigned short*) p_data << 12;
	p_data += 2;

	destination.m_xFixed = *(unsigned short*) p_data << 12;
	p_data += 2;
	destination.m_yFixed = *(unsigned short*) p_data << 12;
	p_data += 2;
	destination.m_zFixed = *(unsigned short*) p_data << 12;
	p_data += 2;

	unsigned short speed = *(unsigned short*) p_data;
	p_data += 2;

	Set(start, destination, speed);
}

// 68K 0x101196dc __dt__5CBallFv
Ball::~Ball()
{
}
