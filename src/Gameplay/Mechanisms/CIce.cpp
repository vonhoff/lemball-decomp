#include "CIce.h"

#include <stdlib.h>

#include "Application/SoundEffects.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/FixedPointConversion.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"

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
	unsigned short minZ = g_pMap->m_ground.GetZ(minGroundX, minGroundY);
	m_min.m_z = (short) minZ;

	int maxGroundY = m_max.m_y;
	int maxGroundX = m_max.m_x;
	unsigned short maxZ = g_pMap->m_ground.GetZ(maxGroundX, maxGroundY);
	m_max.m_z = (short) maxZ;

	m_position.m_xFixed = IntegerToFixed(p_cornerA.m_x);
	m_position.m_yFixed = IntegerToFixed(p_cornerA.m_y);
	m_position.m_zFixed = IntegerToFixed(p_cornerA.m_z);
	for (int y = minY; y <= maxY; y += GROUND_BLOCK_PIXEL_SIZE) {
		for (int x = minX; x <= maxX; x += GROUND_BLOCK_PIXEL_SIZE) {
			int blockX = x / GROUND_BLOCK_PIXEL_SIZE;
			if (blockX >= 0) {
				int blockY = y / GROUND_BLOCK_PIXEL_SIZE;
				int width;
				CMap* map;
				if (blockY >= 0 && (width = g_pMap->m_ground.m_width) > blockX &&
					blockY < (map = g_pMap)->m_ground.m_height) {
					CGround* ground = map->m_ground.m_ground + ((unsigned int) width * blockY + blockX);
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
		AICOORD position(object->m_position);
		int dx = (int) ((unsigned int) m_velocityX * elapsed * FIXED_POINT_ONE) / 8;
		int dy = (int) ((unsigned int) m_velocityY * elapsed * FIXED_POINT_ONE) / 8;
		int ax = abs(dx >> FIXED_POINT_FRACTION_BITS);
		int ay = abs(dy >> FIXED_POINT_FRACTION_BITS);
		while (ax > GROUND_BLOCK_PIXEL_MASK || ay > GROUND_BLOCK_PIXEL_MASK) {
			ax /= 2;
			ay /= 2;
			dx /= 2;
			dy /= 2;
		}
		position.m_xFixed += (unsigned int) dx;
		position.m_yFixed += (unsigned int) dy;
		unsigned short terrainZ = g_pMap->m_ground.GetZ(position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
														position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
		int groundZ = terrainZ;
		int z = position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		if (z < groundZ) {
			position.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
		}
		else if (groundZ < z) {
			z = (int) ((unsigned int) z - (unsigned int) elapsed * (GROUND_BLOCK_PIXEL_SIZE / 4));
			if (z < groundZ) {
				z = groundZ;
			}
			position.m_zFixed = (int) ((unsigned int) z << FIXED_POINT_FRACTION_BITS);
		}
		if (m_velocityX != 0) {
			if (m_velocityY == 0) {
				int fraction = (position.m_yFixed & (GROUND_BLOCK_PIXEL_MASK << FIXED_POINT_FRACTION_BITS)) >>
							   FIXED_POINT_FRACTION_BITS;
				if (fraction > GROUND_BLOCK_PIXEL_SIZE / 2) {
					position.m_yFixed -= IntegerToFixed(1);
				}
				else if (fraction < GROUND_BLOCK_PIXEL_SIZE / 2) {
					position.m_yFixed += IntegerToFixed(1);
				}
			}
		}
		else {
			int fraction = (position.m_xFixed & (GROUND_BLOCK_PIXEL_MASK << FIXED_POINT_FRACTION_BITS)) >>
						   FIXED_POINT_FRACTION_BITS;
			if (fraction > GROUND_BLOCK_PIXEL_SIZE / 2) {
				position.m_xFixed -= IntegerToFixed(1);
			}
			else if (fraction < GROUND_BLOCK_PIXEL_SIZE / 2) {
				position.m_xFixed += IntegerToFixed(1);
			}
		}
		object->m_position = position;
	}
	for (i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		AICOORD current(object->m_position);
		int x = current.m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int y = current.m_yFixed >> FIXED_POINT_FRACTION_BITS;
		if (x < minX || x > maxX || y < minY || y > maxY) {
			object->m_hidden = false;
			object->m_action = ACTION_NONE;
			object->m_actionDeadline = g_dwGameTick;
			unsigned short groundZ;
			if (object->m_objectType == OBJECT_PLAYER_2) {
				object->SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
				object->OnConveyor(0, NULL, 0);
				groundZ = g_pMap->m_ground.GetZ(current.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
				if (groundZ < (current.m_zFixed >> FIXED_POINT_FRACTION_BITS)) {
					C3DVector velocity;
					velocity.m_xFixed.m_value = (int) ((unsigned int) m_velocityX << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_yFixed.m_value = (int) ((unsigned int) m_velocityY << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_zFixed.m_value = 0;
					object->StartFly(velocity, NULL);
				}
			}
			for (int j = i + 1; j < m_objectCount; j++) {
				m_objects[j - 1] = m_objects[j];
			}
			i--;
			m_objectCount--;
			groundZ = g_pMap->m_ground.GetZ(current.m_xFixed >> FIXED_POINT_FRACTION_BITS,
											current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			if ((current.m_zFixed >> FIXED_POINT_FRACTION_BITS) <= groundZ) {
				g_pAI->StepOn(current, object, object->m_collisionFlags);
			}
		}
	}
	for (i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		AICOORD position(object->m_position);
		unsigned short groundZ = g_pMap->m_ground.GetZ(position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													   position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
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
			AICOORD current(object->m_position);
			object->m_hidden = false;
			object->m_action = ACTION_NONE;
			object->m_actionDeadline = g_dwGameTick;
			unsigned short groundZ;
			if (object->m_objectType == OBJECT_PLAYER_2) {
				object->SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
				object->OnConveyor(0, NULL, 0);
				groundZ = g_pMap->m_ground.GetZ(current.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
				if (groundZ < (current.m_zFixed >> FIXED_POINT_FRACTION_BITS)) {
					C3DVector velocity;
					velocity.m_xFixed.m_value = (int) ((unsigned int) m_velocityX << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_yFixed.m_value = (int) ((unsigned int) m_velocityY << FIXED_POINT_FRACTION_BITS) / 6;
					velocity.m_zFixed.m_value = 0;
					object->StartFly(velocity, NULL);
				}
			}
			for (int j = i + 1; j < m_objectCount; j++) {
				m_objects[j - 1] = m_objects[j];
			}
			i--;
			m_objectCount--;
			groundZ = g_pMap->m_ground.GetZ(current.m_xFixed >> FIXED_POINT_FRACTION_BITS,
											current.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			if ((current.m_zFixed >> FIXED_POINT_FRACTION_BITS) <= groundZ) {
				g_pAI->StepOn(current, object, object->m_collisionFlags);
			}
		}
	}
}
