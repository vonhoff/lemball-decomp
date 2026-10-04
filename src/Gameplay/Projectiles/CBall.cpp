#include "CBall.h"

#include "Game/CGame.h"
#include "Game/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/Facing.h"
#include "Visos/Math/CVector.h"
#include "Gameplay/Movement/CMovementInterpolation.h"
#include "Gameplay/Geometry/CPt3.h"
#include "CBallManager.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Level/LevelVersions.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"

#include <stddef.h>

enum {
	BALL_EXPLOSION_DURATION_TICKS = 22
};

enum eBallMovementPhase {
	BALL_PHASE_SPAWNED = 0,
	BALL_PHASE_WAITING_TO_TRAVEL = 1,
	BALL_PHASE_TRAVELING_TO_DESTINATION = 2,
	BALL_PHASE_AT_DESTINATION = 3,
	BALL_PHASE_WAITING_TO_RETURN = 4,
	BALL_PHASE_RETURNING_TO_SPAWN = 5
};

// FUNCTION: LEMBALL 0x00421660
CBall::CBall() : CGameObject(OBJECT_BALL, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00421690
void CBall::Restart()
{
	CGameObject::Restart();
	m_action = ACTION_BALL_MOVING;
	m_speed = g_anTurnDelayCursor[m_objectType];
}

// FUNCTION: LEMBALL 0x004216c0
void CBall::Set(AICOORD p_start, AICOORD p_destination, int p_speed)
{
	enum {
		FIRST_VERSION_USING_BALL_SPEED = 7,
		MIN_CUSTOM_SPEED = 2
	};

	m_position = p_start;
	m_spawnPosition = p_start;
	m_destination = p_destination;
	m_action = ACTION_BALL_MOVING;
	m_actionArgument = BALL_PHASE_SPAWNED;
	if (g_pAI->m_levelVersion < FIRST_VERSION_USING_BALL_SPEED) {
		m_speed = (unsigned short) g_anTurnDelayCursor[m_objectType];
	}
	else {
		m_speed = (unsigned short) p_speed;
	}
	if (m_speed < MIN_CUSTOM_SPEED) {
		m_speed = (unsigned short) g_anTurnDelayCursor[m_objectType];
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

	int distance = Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
							m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
							targetX >> FIXED_POINT_FRACTION_BITS,
							targetY >> FIXED_POINT_FRACTION_BITS);
	m_lastMovementTick = g_dwGameTick;
	m_moveDurationTicks = (m_speed * distance) / GAME_TICK_MILLISECONDS;
	if (m_moveDurationTicks == 0) {
		m_moveDurationTicks = 1;
	}
	m_actionDeadline = m_moveDurationTicks + g_dwGameTick;
	CVector start(m_position.m_xFixed, m_position.m_yFixed);
	CVector end(start);
	end.m_xFixed = targetX;
	end.m_yFixed = targetY;
	m_movement.SetEndpoints(start, end);
}

// FUNCTION: LEMBALL 0x00421870
bool CBall::Move()
{
	unsigned short groundZ;
	int groundX;
	int groundY;
	CAI* ai;
	CGameObject* hit;
	CGameObject* object;
	int elapsed = (int) (g_dwGameTick - m_lastMovementTick);
	int duration = m_moveDurationTicks;
	int blockX;
	int blockY;
	CMap* map;
	unsigned int z;
	int x;
	int y;
	{
		CVector movement = m_movement.m_delta * elapsed;
		movement.m_xFixed /= duration;
		movement.m_yFixed /= duration;
		x = (m_movement.m_start.m_xFixed + movement.m_xFixed) >> FIXED_POINT_FRACTION_BITS;
		y = (m_movement.m_start.m_yFixed + movement.m_yFixed) >> FIXED_POINT_FRACTION_BITS;
	}

	map = g_pMap;
	blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	if (x < 0 || y < 0 || map->m_ground.m_width <= blockX || map->m_ground.m_height <= blockY) {
		groundZ = 0;
	}
	else {
		groundX = x & GROUND_BLOCK_PIXEL_MASK;
		groundY = y & GROUND_BLOCK_PIXEL_MASK;
		groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(groundX, groundY);
	}
	z = groundZ;

	CPt3 point;
	point.m_x = x;
	point.m_y = y;
	ai = g_pAI;
	point.m_z = z;
	ai->m_collisionExclude = this;
	ai->m_collisionPoint = point;
	ai->m_collisionIndex = 0;
	if (ai->m_objectCount > 0) {
		do {
			object = ai->m_objects[ai->m_collisionIndex];
			if (ai->m_collisionExclude != object && object->Collision(ai->m_collisionPoint)) {
				hit = ai->m_objects[ai->m_collisionIndex];
				ai->m_collisionIndex++;
				goto found;
			}
			ai->m_collisionIndex++;
		} while (ai->m_collisionIndex < ai->m_objectCount);
	}
	hit = NULL;
found:
	if (hit != NULL) {
		hit->HitBall();
		m_action = ACTION_BALL_EXPLODING;
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionDeadline = g_dwGameTick + BALL_EXPLOSION_DURATION_TICKS;
		return true;
	}

	if ((m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS) + 12 < (int) z) {
		switch ((unsigned short) m_actionArgument) {
		case BALL_PHASE_TRAVELING_TO_DESTINATION:
			m_actionArgument = BALL_PHASE_RETURNING_TO_SPAWN;
			StartMovement(0);
			return true;
		case BALL_PHASE_RETURNING_TO_SPAWN:
			m_actionArgument = BALL_PHASE_TRAVELING_TO_DESTINATION;
			StartMovement(1);
			return true;
		}
		return true;
	}

	m_position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
	m_position.m_xFixed = x << FIXED_POINT_FRACTION_BITS;
	m_position.m_yFixed = y << FIXED_POINT_FRACTION_BITS;
	return true;
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
			objects[*objectCount] = NULL;
			break;
		}
	}
	g_pBallManager->Delete(this);
	SetId(INVALID_OBJECT_ID);
}

// FUNCTION: LEMBALL 0x00421b40
void CBall::SetHeightCorrect()
{
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & GROUND_BLOCK_PIXEL_MASK;
		int cellY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (unsigned int) z << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x00421bc0
bool CBall::Process()
{
	switch (m_action) {
	case ACTION_BALL_MOVING:
		switch ((unsigned short) m_actionArgument) {
		case BALL_PHASE_SPAWNED:
			m_actionDeadline = g_dwGameTick;
			m_position.m_xFixed = m_spawnPosition.m_xFixed;
			m_position.m_yFixed = m_spawnPosition.m_yFixed;
			m_position.m_zFixed = m_spawnPosition.m_zFixed;
			SetHeightCorrect();
			m_actionArgument = BALL_PHASE_WAITING_TO_TRAVEL;
			break;
		case BALL_PHASE_WAITING_TO_TRAVEL:
			if (m_actionDeadline < g_dwGameTick) {
				m_actionArgument = BALL_PHASE_TRAVELING_TO_DESTINATION;
				StartMovement(1);
			}
			break;
		case BALL_PHASE_TRAVELING_TO_DESTINATION:
			if (m_actionDeadline < g_dwGameTick) {
				m_position.m_xFixed = m_destination.m_xFixed;
				m_position.m_yFixed = m_destination.m_yFixed;
				m_position.m_zFixed = m_destination.m_zFixed;
				SetHeightCorrect();
				m_actionArgument = BALL_PHASE_AT_DESTINATION;
			}
			else {
				Move();
			}
			break;
		case BALL_PHASE_AT_DESTINATION:
			m_actionDeadline = g_dwGameTick;
			m_position.m_xFixed = m_destination.m_xFixed;
			m_position.m_yFixed = m_destination.m_yFixed;
			m_position.m_zFixed = m_destination.m_zFixed;
			SetHeightCorrect();
			m_actionArgument = BALL_PHASE_WAITING_TO_RETURN;
			break;
		case BALL_PHASE_WAITING_TO_RETURN:
			if (m_actionDeadline < g_dwGameTick) {
				m_actionArgument = BALL_PHASE_RETURNING_TO_SPAWN;
				StartMovement(0);
			}
			break;
		case BALL_PHASE_RETURNING_TO_SPAWN:
			if (m_actionDeadline < g_dwGameTick) {
				m_position.m_xFixed = m_spawnPosition.m_xFixed;
				m_position.m_yFixed = m_spawnPosition.m_yFixed;
				m_position.m_zFixed = m_spawnPosition.m_zFixed;
				SetHeightCorrect();
				m_actionArgument = BALL_PHASE_SPAWNED;
			}
			else {
				Move();
			}
			break;
		}
		UpdateCollision();
		return true;
	case ACTION_BALL_EXPLODING:
		if (m_actionDeadline < g_dwGameTick) {
			g_pBallManager->Delete(this);
			return false;
		}
		return true;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00421da0
void CBall::LoadLevel(unsigned char*& p_data)
{
	enum {
		LEVEL_WORD_BYTES = 2
	};

	AICOORD start;
	AICOORD destination;

	if (g_pAI->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
		unsigned short id = *(unsigned short*) p_data;
		p_data += LEVEL_WORD_BYTES;
		SetId(id);
	}

	const int startX = *(unsigned short*) p_data << FIXED_POINT_FRACTION_BITS;
	start.m_xFixed = startX;
	p_data += LEVEL_WORD_BYTES;
	const int startY = *(unsigned short*) p_data << FIXED_POINT_FRACTION_BITS;
	start.m_yFixed = startY;
	p_data += LEVEL_WORD_BYTES;
	const int startZ = *(unsigned short*) p_data << FIXED_POINT_FRACTION_BITS;
	start.m_zFixed = startZ;
	p_data += LEVEL_WORD_BYTES;

	const int destinationX = *(unsigned short*) p_data << FIXED_POINT_FRACTION_BITS;
	destination.m_xFixed = destinationX;
	p_data += LEVEL_WORD_BYTES;
	const int destinationY = *(unsigned short*) p_data << FIXED_POINT_FRACTION_BITS;
	destination.m_yFixed = destinationY;
	p_data += LEVEL_WORD_BYTES;
	const int destinationZ = *(unsigned short*) p_data << FIXED_POINT_FRACTION_BITS;
	destination.m_zFixed = destinationZ;
	p_data += LEVEL_WORD_BYTES;

	unsigned short speed = *(unsigned short*) p_data;
	p_data += LEVEL_WORD_BYTES;

	Set(start, destination, speed);
}
