#include "CIce.h"

#include "Gameplay/Simulation/GameTime.h"

#include "Map/CMap.h"
#include "Map/CGround.h"
#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Simulation/CAI.h"

#include <stdlib.h>

enum {
	ICE_EXTERNAL_CONTROL_TIMEOUT_TICKS = 1000
};

// FUNCTION: LEMBALL 0x0042ca70
CIce::CIce() : CGlobalGameObject(OBJECT_ICE, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0042ca90
void CIce::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x0042cab0
void CIce::Initialise()
{
	m_stateTimer = 0;
	m_switched = 0;
	m_enabled = 0;
	m_objectCount = 0;
	m_action = ACTION_READY;
	m_lastMovementTick = g_dwGameTick;
}

// FUNCTION: LEMBALL 0x0042caf0
CIce::~CIce()
{
}

// FUNCTION: LEMBALL 0x0042cb00
void CIce::Set(unsigned short p_id,
			   const tCoord3d& p_cornerA,
			   const tCoord3d& p_cornerB,
			   int p_velocityX,
			   int p_velocityY,
			   unsigned int p_initialSwitched)
{
	SetId(p_id);
	m_enabled = 1;
	m_objectCount = 0;
	m_lastMovementTick = g_dwGameTick;
	m_velocityX = p_velocityX;
	m_velocityY = p_velocityY;
	m_initialSwitched = p_initialSwitched;
	m_switched = p_initialSwitched;

	int minX = p_cornerA.m_x;
	int maxX = p_cornerB.m_x;
	int minY = p_cornerA.m_y;
	int maxY = p_cornerB.m_y;
	if (maxX < minX) {
		int originalMinX = minX;
		minX = maxX;
		maxX = originalMinX;
	}
	if (maxY < minY) {
		int originalMinY = minY;
		minY = maxY;
		maxY = originalMinY;
	}
	m_min.m_x = (short) minX;
	m_min.m_y = (short) minY;
	m_max.m_x = (short) maxX;
	m_max.m_y = (short) maxY;

	int minGroundY = (short) minY;
	int minGroundX = (short) minX;
	unsigned short minZ;
	{
		CMap* map = g_pMap;
		int width = map->m_ground.m_width;
		if (minGroundX < 0 || minGroundY < 0 || (minGroundX >> GROUND_BLOCK_PIXEL_SHIFT) >= width ||
			(minGroundY >> GROUND_BLOCK_PIXEL_SHIFT) >= g_pMap->m_ground.m_height) {
			minZ = 0;
		}
		else {
			minZ = map->m_ground
					   .m_ground[(minGroundY >> GROUND_BLOCK_PIXEL_SHIFT) * width +
								 (minGroundX >> GROUND_BLOCK_PIXEL_SHIFT)]
					   .GetZ(minGroundX & GROUND_BLOCK_PIXEL_MASK, minGroundY & GROUND_BLOCK_PIXEL_MASK);
		}
	}
	m_min.m_z = (short) minZ;

	int maxGroundY = m_max.m_y;
	int maxGroundX = m_max.m_x;
	unsigned short maxZ;
	{
		CMap* map = g_pMap;
		int width = map->m_ground.m_width;
		if (maxGroundX < 0 || maxGroundY < 0 || (maxGroundX >> GROUND_BLOCK_PIXEL_SHIFT) >= width ||
			(maxGroundY >> GROUND_BLOCK_PIXEL_SHIFT) >= g_pMap->m_ground.m_height) {
			maxZ = 0;
		}
		else {
			maxZ = map->m_ground
					   .m_ground[(maxGroundY >> GROUND_BLOCK_PIXEL_SHIFT) * width +
								 (maxGroundX >> GROUND_BLOCK_PIXEL_SHIFT)]
					   .GetZ(maxGroundX & GROUND_BLOCK_PIXEL_MASK, maxGroundY & GROUND_BLOCK_PIXEL_MASK);
		}
	}
	m_max.m_z = (short) maxZ;

	m_position.m_xFixed = ((int) p_cornerA.m_x) << FIXED_POINT_FRACTION_BITS;
	m_position.m_yFixed = ((int) p_cornerA.m_y) << FIXED_POINT_FRACTION_BITS;
	m_position.m_zFixed = ((int) p_cornerA.m_z) << FIXED_POINT_FRACTION_BITS;
	for (int y = minY; y <= maxY; y += GROUND_BLOCK_PIXEL_SIZE) {
		for (int x = minX; x <= maxX; x += GROUND_BLOCK_PIXEL_SIZE) {
			int blockX = x / GROUND_BLOCK_PIXEL_SIZE;
			if (blockX >= 0) {
				int blockY = y / GROUND_BLOCK_PIXEL_SIZE;
				int width;
				CMap* map;
				if (blockY >= 0 && (width = g_pMap->m_ground.m_width) > blockX &&
					blockY < (map = g_pMap)->m_ground.m_height) {
					CGround* ground = map->m_ground.m_ground + width * blockY + blockX;
					ground->m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
				}
			}
		}
	}

	if (m_velocityX == 0 && m_velocityY == 0) {
		m_velocityX = 1;
		m_velocityY = 1;
	}
}

// FUNCTION: LEMBALL 0x0042cd70
bool CIce::Process()
{
	if (m_isRemoteObject) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				m_switched = !m_switched;
				Switched();
			}
			m_pendingAction = m_action;
		}
	}
	else if (m_action == ACTION_ACTIVATED) {
		m_switched = !m_switched;
		Switched();
		Action(ACTION_READY);
	}
	if (!m_switched) {
		return true;
	}
	int elapsed = g_dwGameTick - m_lastMovementTick;
	if (!elapsed) {
		return true;
	}
	m_lastMovementTick = g_dwGameTick;
	int minX = m_min.m_x - GAME_OBJECT_COLLISION_XY_MIN_INSET;
	int minY = m_min.m_y - GAME_OBJECT_COLLISION_XY_MIN_INSET;
	int maxX = m_max.m_x + GAME_OBJECT_COLLISION_XY_MAX_OFFSET;
	int maxY = m_max.m_y + GAME_OBJECT_COLLISION_XY_MAX_OFFSET;
	int i;
	for (i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		AICOORD position(object->m_position.m_xFixed, object->m_position.m_yFixed, object->m_position.m_zFixed);
		int dx = (m_velocityX * elapsed * FIXED_POINT_ONE) / 8;
		int dy = (m_velocityY * elapsed * FIXED_POINT_ONE) / 8;
		int ax = abs(dx >> FIXED_POINT_FRACTION_BITS);
		int ay = abs(dy >> FIXED_POINT_FRACTION_BITS);
		while (ax > GROUND_BLOCK_PIXEL_MASK || ay > GROUND_BLOCK_PIXEL_MASK) {
			ax /= 2;
			ay /= 2;
			dx /= 2;
			dy /= 2;
		}
		position.m_xFixed += dx;
		position.m_yFixed += dy;
		unsigned short terrainZ;
		{
			CMap* map = g_pMap;
			int width;
			int y = (position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			int x = (position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
			int by = y >> GROUND_BLOCK_PIXEL_SHIFT;
			int bx = x >> GROUND_BLOCK_PIXEL_SHIFT;
			if (x < 0 || y < 0 || bx >= (width = map->m_ground.m_width) || by >= map->m_ground.m_height) {
				terrainZ = 0;
			}
			else {
				terrainZ = map->m_ground.m_ground[by * width + bx].GetZ(x & GROUND_BLOCK_PIXEL_MASK,
																		y & GROUND_BLOCK_PIXEL_MASK);
			}
		}
		int groundZ = terrainZ;
		int z = position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		if (z < groundZ) {
			position.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
		}
		else if (groundZ < z) {
			z -= elapsed * (GROUND_BLOCK_PIXEL_SIZE / 4);
			if (z < groundZ) {
				const int& floorZ = groundZ;
				int& fallingZ = z;
				fallingZ = floorZ;
			}
			position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
		}
		if (m_velocityX != 0) {
			if (m_velocityY == 0) {
				int fraction = (position.m_yFixed & (GROUND_BLOCK_PIXEL_MASK << FIXED_POINT_FRACTION_BITS)) >>
							   FIXED_POINT_FRACTION_BITS;
				if (fraction > GROUND_BLOCK_PIXEL_SIZE / 2) {
					position.m_yFixed -= FIXED_POINT_ONE;
				}
				else if (fraction < GROUND_BLOCK_PIXEL_SIZE / 2) {
					position.m_yFixed += FIXED_POINT_ONE;
				}
			}
		}
		else {
			int fraction = (position.m_xFixed & (GROUND_BLOCK_PIXEL_MASK << FIXED_POINT_FRACTION_BITS)) >>
						   FIXED_POINT_FRACTION_BITS;
			if (fraction > GROUND_BLOCK_PIXEL_SIZE / 2) {
				position.m_xFixed -= FIXED_POINT_ONE;
			}
			else if (fraction < GROUND_BLOCK_PIXEL_SIZE / 2) {
				position.m_xFixed += FIXED_POINT_ONE;
			}
		}
		const AICOORD& movedPosition = position;
		object->m_position = movedPosition;
	}
	for (i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		AICOORD current(object->m_position.m_xFixed, object->m_position.m_yFixed, object->m_position.m_zFixed);
		int x = current.m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int y = current.m_yFixed >> FIXED_POINT_FRACTION_BITS;
		if (x < minX || x > maxX || y < minY || y > maxY) {
			object->m_hidden = 0;
			object->m_action = ACTION_NONE;
			object->m_actionDeadline = g_dwGameTick;
			unsigned short groundZ;
			if (object->m_objectType == OBJECT_PLAYER_2) {
				object->SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
				object->OnConveyor(0, NULL, 0);
				{
					int y = (current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
					int x = (current.m_xFixed >> FIXED_POINT_FRACTION_BITS);
					CMap* map = g_pMap;
					int width;
					int by = y >> GROUND_BLOCK_PIXEL_SHIFT;
					int bx = x >> GROUND_BLOCK_PIXEL_SHIFT;
					if (x < 0 || y < 0 || bx >= (width = map->m_ground.m_width) || by >= g_pMap->m_ground.m_height) {
						groundZ = 0;
					}
					else {
						groundZ = map->m_ground.m_ground[by * width + bx].GetZ(x & GROUND_BLOCK_PIXEL_MASK,
																			   y & GROUND_BLOCK_PIXEL_MASK);
					}
				}
				if (groundZ < (current.m_zFixed >> FIXED_POINT_FRACTION_BITS)) {
					C3DVector velocity;
					velocity.m_xFixed = (m_velocityX << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_yFixed = (m_velocityY << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_zFixed = 0;
					object->StartFly(velocity, NULL);
				}
			}
			for (int j = i + 1; j < m_objectCount; j++) {
				m_objects[j - 1] = m_objects[j];
			}
			i--;
			m_objectCount--;
			{
				CMap* map = g_pMap;
				int width;
				int y = (current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
				int x = (current.m_xFixed >> FIXED_POINT_FRACTION_BITS);
				int by = y >> GROUND_BLOCK_PIXEL_SHIFT;
				int bx = x >> GROUND_BLOCK_PIXEL_SHIFT;
				if (x < 0 || y < 0 || bx >= (width = map->m_ground.m_width) || by >= g_pMap->m_ground.m_height) {
					groundZ = 0;
				}
				else {
					groundZ = map->m_ground.m_ground[by * width + bx].GetZ(x & GROUND_BLOCK_PIXEL_MASK,
																		   y & GROUND_BLOCK_PIXEL_MASK);
				}
			}
			if ((current.m_zFixed >> FIXED_POINT_FRACTION_BITS) <= groundZ) {
				g_pAI->StepOn(current, object, object->m_collisionFlags);
			}
		}
	}
	for (i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		AICOORD position(object->m_position.m_xFixed, object->m_position.m_yFixed, object->m_position.m_zFixed);
		unsigned short groundZ;
		{
			CMap* map = g_pMap;
			int y = (position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			int x = (position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
			int by = y >> GROUND_BLOCK_PIXEL_SHIFT;
			int bx = x >> GROUND_BLOCK_PIXEL_SHIFT;
			int width;
			if (x < 0 || y < 0 || bx >= (width = g_pMap->m_ground.m_width) || by >= g_pMap->m_ground.m_height) {
				groundZ = 0;
			}
			else {
				x &= GROUND_BLOCK_PIXEL_MASK;
				y &= GROUND_BLOCK_PIXEL_MASK;
				groundZ = map->m_ground.m_ground[by * width + bx].GetZ(x, y);
			}
		}
		if ((position.m_zFixed >> FIXED_POINT_FRACTION_BITS) <= groundZ) {
			g_pAI->StepOn(position, object, object->m_collisionFlags);
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0042d380
bool CIce::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	if (!m_switched) {
		return false;
	}
	int alreadyOn;
	if (p_object->m_objectType == OBJECT_PLAYER_2) {
		alreadyOn = p_object->OnConveyor();
	}
	else {
		alreadyOn =
			p_object->m_action == ACTION_EXTERNAL_CONTROL && p_object->m_actionArgument == EXTERNAL_CONTROL_ON_ICE;
	}
	if (alreadyOn) {
		return false;
	}
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	if (m_min.m_x - GAME_OBJECT_COLLISION_XY_MIN_INSET <= x && x <= m_max.m_x + GAME_OBJECT_COLLISION_XY_MAX_OFFSET &&
		m_min.m_y - GAME_OBJECT_COLLISION_XY_MIN_INSET <= y && y <= m_max.m_y + GAME_OBJECT_COLLISION_XY_MAX_OFFSET) {
		if (m_objectCount < 10) {
			m_objects[m_objectCount++] = p_object;
			p_object->ResetInstructions();
			p_object->m_stateTimer = g_dwGameTick * GAME_TICK_MILLISECONDS;
			p_object->m_actionDeadline = g_dwGameTick + ICE_EXTERNAL_CONTROL_TIMEOUT_TICKS;
			if (p_object->m_objectType == OBJECT_PLAYER_2) {
				p_object->Action(ACTION_ON_CONVEYOR);
				p_object->SetSndEffect(SFX_WHEEE);
				p_object->OnConveyor(1, this, 0);
				return true;
			}
			p_object->Action(ACTION_EXTERNAL_CONTROL, EXTERNAL_CONTROL_ON_ICE);
			p_object->SetSndEffect(SFX_WHEEE);
		}
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042d4d0
void CIce::Leave(CPlayerLemming* p_lemming)
{
	short lemmingId = ((CGameObject*) p_lemming)->GetId();
	int index = 0;
	if (m_objectCount > 0) {
		CGameObject** object = m_objects;
		while ((*object)->GetId() != lemmingId) {
			object++;
			index++;
			if (index >= m_objectCount) {
				return;
			}
		}

		index++;
		if (index < m_objectCount) {
			do {
				m_objects[index - 1] = m_objects[index];
				index++;
			} while (index < m_objectCount);
		}
		m_objectCount--;
	}
}

// FUNCTION: LEMBALL 0x0042d550
void CIce::Switch()
{
	RequestAction(ACTION_ACTIVATED);
}

// FUNCTION: LEMBALL 0x0042d560
void CIce::Switched()
{
	if (m_switched != 0) {
		m_lastMovementTick = g_dwGameTick;
		return;
	}
	{
		for (int i = 0; i < m_objectCount; i++) {
			CGameObject* object = m_objects[i];
			AICOORD current(object->m_position.m_xFixed, object->m_position.m_yFixed, object->m_position.m_zFixed);
			object->m_hidden = 0;
			object->m_action = ACTION_NONE;
			object->m_actionDeadline = g_dwGameTick;
			unsigned short groundZ;
			if (object->m_objectType == OBJECT_PLAYER_2) {
				object->SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
				object->OnConveyor(0, NULL, 0);
				{
					int y = (current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
					int x = (current.m_xFixed >> FIXED_POINT_FRACTION_BITS);
					CMap* map = g_pMap;
					int by = y >> GROUND_BLOCK_PIXEL_SHIFT;
					int bx = x >> GROUND_BLOCK_PIXEL_SHIFT;
					if (x < 0 || y < 0 || bx >= map->m_ground.m_width || by >= map->m_ground.m_height) {
						groundZ = 0;
					}
					else {
						x &= 15;
						y &= 15;
						groundZ = map->m_ground.m_ground[by * map->m_ground.m_width + bx].GetZ(x, y);
					}
				}
				if (groundZ < (current.m_zFixed >> FIXED_POINT_FRACTION_BITS)) {
					C3DVector velocity;
					velocity.m_xFixed = (m_velocityX << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_yFixed = (m_velocityY << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_zFixed = 0;
					object->StartFly(velocity, NULL);
				}
			}
			for (int j = i + 1; j < m_objectCount; j++) {
				m_objects[j - 1] = m_objects[j];
			}
			i--;
			m_objectCount--;
			{
				int y = (current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
				int x = (current.m_xFixed >> FIXED_POINT_FRACTION_BITS);
				CMap* map = g_pMap;
				int by = y >> GROUND_BLOCK_PIXEL_SHIFT;
				int bx = x >> GROUND_BLOCK_PIXEL_SHIFT;
				if (x < 0 || y < 0 || bx >= map->m_ground.m_width || by >= map->m_ground.m_height) {
					groundZ = 0;
				}
				else {
					x &= 15;
					y &= 15;
					groundZ = map->m_ground.m_ground[by * map->m_ground.m_width + bx].GetZ(x, y);
				}
			}
			if ((current.m_zFixed >> FIXED_POINT_FRACTION_BITS) <= groundZ) {
				g_pAI->StepOn(current, object, object->m_collisionFlags);
			}
		}
	}
}
