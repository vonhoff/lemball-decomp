#include "CBall.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../../Visos/Foundation/CVector.h"
#include "../Base/CPt3.h"
#include "../Managers/CBallManager.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x00421660
CBall::CBall() : CGameObject(OBJECT_BALL, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00421690
void CBall::Restart()
{
	CGameObject::Restart();
	m_action = ACTION_0x25;
	m_speed = g_anTurnDelayCursor[m_objectType];
}

// FUNCTION: LEMBALL 0x004216c0
void CBall::Set(AiCoord p_start, AiCoord p_destination, int p_speed)
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
	m_action = ACTION_0x25;
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

// FUNCTION: LEMBALL 0x00421770
void CBall::StartMovement(unsigned int p_direction)
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

// FUNCTION: LEMBALL 0x00421870
bool CBall::Move()
{
	int elapsed = (int) (g_dwGameTick - m_lastMovementTick);
	int duration = m_moveDurationTicks;
	int blockX;
	int blockY;
	CMap* map;
	unsigned int z;
	int x;
	int y;
	{
		CVector movement = *(const CVector*) &m_moveDeltaXFixed * elapsed;
		movement.m_xFixed /= duration;
		movement.m_yFixed /= duration;
		x = (m_moveStartXFixed + movement.m_xFixed) >> 12;
		y = (m_moveStartYFixed + movement.m_yFixed) >> 12;
	}

	map = g_pMap;
	blockX = x >> 4;
	blockY = y >> 4;
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

	CPt3 point;
	point.m_x = x;
	point.m_y = y;
	CAI* ai = g_pAI;
	point.m_z = z;
	ai->m_collisionExclude = this;
	ai->m_collisionPoint = point;
	ai->m_collisionIndex = 0;
	CGameObject* hit;
	if (ai->m_objectCount > 0) {
		do {
			CGameObject* object = ai->m_objects[ai->m_collisionIndex];
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
		m_action = ACTION_0x26;
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

// FUNCTION: LEMBALL 0x00421aa0
void CBall::HitBullet(CBullet* p_bullet)
{
	Delete();
}

// FUNCTION: LEMBALL 0x00421ab0
void CBall::Delete()
{
	int* objectCount;
	int i = 0;
	objectCount = &g_pAI->m_objectCount;
	for (; i < *objectCount; i++) {
		CGameObject**& objects = g_pAI->m_objects;
		if (objects[i] == this) {
			(*objectCount)--;
			for (; i < *objectCount; i++) {
				objects[i] = objects[i + 1];
			}
			objects[*objectCount] = 0;
			break;
		}
	}
	g_pBallManager->Delete(this);
	SetId(0xffff);
}

// FUNCTION: LEMBALL 0x00421b40
void CBall::SetHeightCorrect()
{
	int x = m_position.m_xFixed >> 12;
	int y = m_position.m_yFixed >> 12;
	CMap* map = g_pMap;
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

// FUNCTION: LEMBALL 0x00421bc0
bool CBall::Process()
{
	switch (m_action) {
	case ACTION_0x25:
		switch ((unsigned short) m_actionArgument) {
		case 0:
			m_actionDeadline = g_dwGameTick;
			m_position.m_xFixed = m_spawnPosition.m_xFixed;
			m_position.m_yFixed = m_spawnPosition.m_yFixed;
			m_position.m_zFixed = m_spawnPosition.m_zFixed;
			SetHeightCorrect();
			m_actionArgument = 1;
			break;
		case 1:
			if (m_actionDeadline < g_dwGameTick) {
				m_actionArgument = 2;
				StartMovement(1);
			}
			break;
		case 2:
			if (m_actionDeadline < g_dwGameTick) {
				m_position.m_xFixed = m_destination.m_xFixed;
				m_position.m_yFixed = m_destination.m_yFixed;
				m_position.m_zFixed = m_destination.m_zFixed;
				SetHeightCorrect();
				m_actionArgument = 3;
			}
			else {
				Move();
			}
			break;
		case 3:
			m_actionDeadline = g_dwGameTick;
			m_position.m_xFixed = m_destination.m_xFixed;
			m_position.m_yFixed = m_destination.m_yFixed;
			m_position.m_zFixed = m_destination.m_zFixed;
			SetHeightCorrect();
			m_actionArgument = 4;
			break;
		case 4:
			if (m_actionDeadline < g_dwGameTick) {
				m_actionArgument = 5;
				StartMovement(0);
			}
			break;
		case 5:
			if (m_actionDeadline < g_dwGameTick) {
				m_position.m_xFixed = m_spawnPosition.m_xFixed;
				m_position.m_yFixed = m_spawnPosition.m_yFixed;
				m_position.m_zFixed = m_spawnPosition.m_zFixed;
				SetHeightCorrect();
				m_actionArgument = 0;
			}
			else {
				Move();
			}
			break;
		}
		UpdateCollision();
		return 1;
	case ACTION_0x26:
		if (m_actionDeadline < g_dwGameTick) {
			g_pBallManager->Delete(this);
			return 0;
		}
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00421da0
void CBall::LoadLevel(unsigned char*& p_data)
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
