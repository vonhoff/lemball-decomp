#include "CMover.h"

#include "Gameplay/Simulation/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/Facing.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Gameplay/Groups/CPlayerLemmingGroup.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Movement/CMove3d.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"

enum {
	AUTOMATIC_MOVER_TURNING_DELAY_TICKS = 20
};
#include "Gameplay/Simulation/CAI.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"

#include <stddef.h>

#define MOVER_FOOTPRINT_HALF_SIZE 8
#define MOVER_FOOTPRINT_LAST_OFFSET 15
#define MOVER_SURFACE_Z_OFFSET 8

// FUNCTION: LEMBALL 0x0042e590
CMover::CMover() : CGlobalGameObject(OBJECT_MOVER, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0042e5e0
void CMover::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x0042e600
void CMover::Initialise()
{
	m_stateTimer = 0;
	m_active = 0;
	m_moving = 0;
	m_movementMode = MOVER_MODE_AUTOMATIC;
	m_switchRequested = 0;
	m_objectCount = 0;
	m_findOccupants = 1;
	m_action = ACTION_READY;
}

// FUNCTION: LEMBALL 0x0042e640
CMover::~CMover()
{
}

// FUNCTION: LEMBALL 0x0042e650
void CMover::SetPos()
{
	int x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxX = x + 15;
	int maxY = y + 15;
	x /= 16;
	y /= 16;
	maxX /= 16;
	maxY /= 16;
	for (int groundY = y; groundY <= maxY; groundY++) {
		for (int groundX = x; groundX <= maxX; groundX++) {
			if (groundX >= 0 && groundY >= 0 && groundX < g_pMap->m_ground.m_width &&
				groundY < g_pMap->m_ground.m_height) {
				CGround* ground = &g_pMap->m_ground.m_ground[groundY * g_pMap->m_ground.m_width + groundX];
				ground->m_collision |= GROUND_COLLISION_MOVER_PRESENT;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0042e700
bool CMover::IsAt(int p_x, int p_y, int& p_height)
{

	int x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - MOVER_FOOTPRINT_HALF_SIZE;
	int xMax = x + MOVER_FOOTPRINT_LAST_OFFSET;
	int y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - MOVER_FOOTPRINT_HALF_SIZE;
	int yMax = y + MOVER_FOOTPRINT_LAST_OFFSET;
	if (p_x >= x && p_x <= xMax && p_y >= y && p_y <= yMax) {
		p_height = (m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS) + MOVER_SURFACE_Z_OFFSET;
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042e760
void CMover::Set(unsigned short p_id, int p_pathId, unsigned int p_movementMode, int p_startNode, int p_nodeCount)
{
	SetId(p_id);
	const CPt3& position = g_pAI->GetNodePosition(p_startNode);
	m_position.m_xFixed = position.m_x;
	m_position.m_yFixed = position.m_y;
	m_position.m_zFixed = position.m_z;

	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int groundX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int groundY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	CMap* map = g_pMap;
	unsigned short z;
	if (x < 0 || y < 0 || map->m_ground.m_width <= groundX || map->m_ground.m_height <= groundY) {
		z = 0;
	}
	else {
		x &= GROUND_BLOCK_PIXEL_MASK;
		y &= GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[groundY * map->m_ground.m_width + groundX].GetZ(x, y);
	}

	m_active = 1;
	m_actionArgument = (short) p_pathId;
	m_position.m_zFixed = (unsigned int) z << FIXED_POINT_FRACTION_BITS;
	m_startNode = p_startNode;
	m_currentNode = 0;
	m_objectCount = 0;
	m_movementMode = p_movementMode;
	m_nodeCount = p_nodeCount;
}

// FUNCTION: LEMBALL 0x0042e850
void CMover::SetUpNextNode(unsigned int p_time)
{
	int nextNode = m_currentNode + 1;
	if (m_nodeCount <= nextNode) {
		nextNode = 0;
	}

	CPt3 nextPosition = g_pAI->GetNodePosition(m_startNode + nextNode);
	CMap* map = g_pMap;
	int y = nextPosition.m_y >> FIXED_POINT_FRACTION_BITS;
	int x = nextPosition.m_x >> FIXED_POINT_FRACTION_BITS;
	int blockX;
	int blockY;
	blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	int width;
	if (x < 0 || y < 0 || blockX >= (width = g_pMap->m_ground.m_width) || blockY >= g_pMap->m_ground.m_height) {
		z = 0;
	}
	else {
		x &= 15;
		y &= 15;
		z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x, y);
	}
	const unsigned int& height = (unsigned int) z;
	nextPosition.m_z = height << FIXED_POINT_FRACTION_BITS;

	int startY;
	int startX;
	startX = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	startY = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int startZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	CPt3 start(startX, startY, startZ);
	int endY;
	int endX;
	endX = nextPosition.m_x >> FIXED_POINT_FRACTION_BITS;
	endY = nextPosition.m_y >> FIXED_POINT_FRACTION_BITS;
	int endZ = nextPosition.m_z >> FIXED_POINT_FRACTION_BITS;
	CPt3 end(endX, endY, endZ);

	unsigned int distance = Distance(startX, startY, endX, endY);
	m_lastMovementTick = p_time;
	m_actionDeadline = distance + p_time;
	m_motion.Set(start, end, p_time, 1);
}

// FUNCTION: LEMBALL 0x0042e980
void CMover::FindObjectsOnTopOfMe()
{
	int objectCount = g_wObjectCount;
	const int& minX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	CMover* const& mover = this;
	const int& maxX = minX + 15;
	int minY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	const int& maxY = minY + 15;
	int index = 0;
	if (objectCount > 0) {
		do {
			CGameObject* object = g_pObjects[(unsigned short) index];
			if (object != NULL && object->GetId() != (short) INVALID_OBJECT_ID && mover->GetId() != object->GetId() &&
				object->m_objectType != OBJECT_SHEEP) {
				int objectX = object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
				int objectY = object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
				if (objectX >= minX && objectX <= maxX && minY <= objectY && maxY >= objectY) {
					mover->GetOn(object);
				}
			}
			index++;
		} while (objectCount > index);
	}
}

// FUNCTION: LEMBALL 0x0042ea40
void CMover::MoveObjects(int p_deltaX, int p_deltaY, int p_deltaZ)
{
	for (int i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		object->m_position.m_xFixed += p_deltaX << FIXED_POINT_FRACTION_BITS;
		object->m_position.m_yFixed += p_deltaY << FIXED_POINT_FRACTION_BITS;
		object->m_position.m_zFixed += p_deltaZ << FIXED_POINT_FRACTION_BITS;
	}
}

// FUNCTION: LEMBALL 0x0042eac0
void CMover::MoveOccupantsToDestination()
{
	for (int index = 0; index < m_objectCount; ++index) {
		m_objects[index]->AddDestination(m_position);
		m_objects[index]->StartMoving();
	}
}

// FUNCTION: LEMBALL 0x0042eb00
bool CMover::Process()
{
	if (m_active == 0) {
		return true;
	}
	if (m_findOccupants != 0) {
		m_findOccupants = 0;
		FindObjectsOnTopOfMe();
	}
	bool local = g_pActiveConnection == NULL || g_pActiveConnection->m_isHost != 0;
	eAction action;
	unsigned int time;
	if (local) {
		time = g_dwGameTick;
		action = m_action;
	}
	else {
		time = g_dwRemoteGameTick;
		action = m_action;
		if (m_pendingAction != action) {
			if (action == ACTION_WALKING) {
				StopObjectsMoving();
				SetUpNextNode(time);
			}
			action = m_action;
			m_pendingAction = action;
		}
		else if (action != ACTION_WALKING && action != ACTION_MOVER_WAITING_FOR_SWITCH) {
			action = ACTION_READY;
		}
	}
	VerifyObjects();
	switch (action) {
	case ACTION_NONE: {
		int next = m_currentNode + 1;
		if (m_nodeCount <= next) {
			next = 0;
		}
		m_currentNode = next;
		AICOORD oldPosition = m_position;
		int groundX;
		int y;
		int x;
		{
			const CPt3& position = g_pAI->GetNodePosition(m_startNode + next);
			m_position.m_xFixed = position.m_x;
			x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			m_position.m_yFixed = position.m_y;
			y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			groundX = x >> GROUND_BLOCK_PIXEL_SHIFT;
			m_position.m_zFixed = position.m_z;
		}
		int groundY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		CMap* map = g_pMap;
		unsigned short z;
		if (x < 0 || y < 0 || groundX >= g_pMap->m_ground.m_width || g_pMap->m_ground.m_height <= groundY) {
			z = 0;
		}
		else {
			x &= GROUND_BLOCK_PIXEL_MASK;
			y &= GROUND_BLOCK_PIXEL_MASK;
			z = map->m_ground.m_ground[groundY * map->m_ground.m_width + groundX].GetZ(x, y);
		}
		const unsigned int& height = (unsigned int) z;
		m_position.m_zFixed = height << FIXED_POINT_FRACTION_BITS;
		oldPosition.m_xFixed =
			(oldPosition.m_xFixed >> FIXED_POINT_FRACTION_BITS) - (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
		oldPosition.m_yFixed =
			(oldPosition.m_yFixed >> FIXED_POINT_FRACTION_BITS) - (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
		oldPosition.m_zFixed =
			(oldPosition.m_zFixed >> FIXED_POINT_FRACTION_BITS) - (m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS);
		MoveObjects(-oldPosition.m_xFixed, -oldPosition.m_yFixed, -oldPosition.m_zFixed);
		if (m_movementMode != MOVER_MODE_AUTOMATIC) {
			m_lastMovementTick = g_dwGameTick;
			if (local) {
				Action(ACTION_MOVER_WAITING_FOR_SWITCH);
			}
		}
		else {
			m_lastMovementTick = g_dwGameTick + AUTOMATIC_MOVER_TURNING_DELAY_TICKS;
			if (local) {
				Action(ACTION_TURNING);
			}
		}
		break;
	}
	case ACTION_TURNING:
		if (local) {
			if (g_dwGameTick < m_lastMovementTick) {
				return true;
			}
			StopObjectsMoving();
			Action(ACTION_WALKING);
		}
		SetUpNextNode(time);
		break;
	case ACTION_WALKING:
		SetPos();
		if (local && m_actionDeadline < g_dwGameTick) {
			Action(ACTION_NONE);
		}
		else if (time <= m_actionDeadline) {
			CPt3 position;
			position.m_x = 0;
			position.m_y = 0;
			position.m_z = 0;
			m_motion.Position(position, time);
			int dx = position.m_x - (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
			int dz = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
			int dy = position.m_y - (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			dz = position.m_z - dz;
			MoveObjects(dx, dy, dz);
			m_position.m_xFixed = position.m_x << FIXED_POINT_FRACTION_BITS;
			m_position.m_yFixed = position.m_y << FIXED_POINT_FRACTION_BITS;
			m_position.m_zFixed = position.m_z << FIXED_POINT_FRACTION_BITS;
		}
		break;
	case ACTION_STARTING_ROUTE:
		SetUpNextNode(time);
		SetPos();
		if (local) {
			Action(ACTION_TURNING);
		}
		break;
	case ACTION_READY:
		if (local) {
			Action(ACTION_STARTING_ROUTE);
		}
		break;
	case ACTION_MOVER_WAITING_FOR_SWITCH:
		if (m_switchRequested != 0) {
			Action(ACTION_TURNING);
			m_switchRequested = 0;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0042eeb0
void CMover::Switch()
{
	m_switchRequested = 1;
}

// FUNCTION: LEMBALL 0x0042eee0
bool CMover::IsOn(const AICOORD& p_position)
{
	int minX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxX = minX + 15;
	int minY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxY = minY + 15;
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042ef40
void CMover::VerifyObjects()
{
	int minX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxX = minX + 15;
	int minY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxY = minY + 15;
	int i = 0;
	if (m_objectCount > 0) {
		do {
			int x;
			int y;
			CGameObject* object = m_objects[i];
			x = object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			y = object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			if (minX > x || maxX < x || minY > y || maxY < y) {
				object->m_onMover = 0;
				int next = i + 1;
				if (next < m_objectCount) {
					do {
						m_objects[next - 1] = m_objects[next];
						next++;
					} while (next < m_objectCount);
				}
				i--;
				m_objectCount--;
			}
			i++;
		} while (i < m_objectCount);
	}
}

// FUNCTION: LEMBALL 0x0042eff0
bool CMover::GetOn(CGameObject* p_object)
{
	AICOORD objectPosition;
	objectPosition.m_xFixed = p_object->m_position.m_xFixed;
	objectPosition.m_yFixed = p_object->m_position.m_yFixed;
	objectPosition.m_zFixed = p_object->m_position.m_zFixed;
	int objectZ = objectPosition.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	int moverZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	if (objectZ < moverZ - 16 || objectZ > moverZ + 16) {
		return false;
	}

	if (IsOn(objectPosition) == 0) {
		return false;
	}
	CGameObject** object;
	int count = m_objectCount;
	int i;
	if (count < 10) {
		i = 0;
		if (count > 0) {
			object = m_objects;
			do {
				if (*object == p_object) {
					return true;
				}
				object++;
				i++;
			} while (i < count);
		}

		m_objects[count] = p_object;
		p_object->m_onMover = 1;
		m_objectCount++;
		StopObjectsMoving();
		if (m_action != ACTION_WALKING && p_object->m_objectType == OBJECT_PLAYER_2) {
			AICOORD destination(m_position.m_xFixed, m_position.m_yFixed, objectPosition.m_zFixed);
			p_object->AddDestination(destination);
			p_object->StartMoving();
		}
		else {
			objectPosition.m_zFixed = m_position.m_zFixed + (MOVER_SURFACE_Z_OFFSET << FIXED_POINT_FRACTION_BITS);
			p_object->m_position.m_xFixed = objectPosition.m_xFixed;
			p_object->m_position.m_yFixed = objectPosition.m_yFixed;
			p_object->m_position.m_zFixed = objectPosition.m_zFixed;
		}
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042f140
void CMover::StopObjectsMoving()
{
	int index = 0;
	if (0 < m_objectCount) {
		CGameObject** object = m_objects;
		do {
			if ((*object)->m_objectType == OBJECT_PLAYER_2) {
				((CPlayerLemming*) *object)->GetGroup()->ClearExistingWaypoints();
			}
			else {
				(*object)->ResetInstructions();
			}
			object++;
			index++;
		} while (index < m_objectCount);
	}
}

// FUNCTION: LEMBALL 0x0042fb90
void CMover::DoActivate()
{
}
