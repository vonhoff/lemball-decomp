#include "CMover.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../../Visos/Network/CConnect.h"
#include "../Groups/CPlayerLemmingGroup.h"
#include "../Objects/CPlayerLemming.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/CMove3d.h"
#include "AI/Base/CPt3.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "CAI.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

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
	m_movementMode = 0;
	m_switchRequested = 0;
	m_objectCount = 0;
	m_findOccupants = 1;
	m_action = ACTION_0x18;
}

// FUNCTION: LEMBALL 0x0042e640
CMover::~CMover()
{
}

// FUNCTION: LEMBALL 0x0042e650
void CMover::SetPos()
{
	int x = (m_position.m_xFixed >> 12) - 8;
	int maxX = x + 15;
	int y = (m_position.m_yFixed >> 12) - 8;
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
				ground->m_collision |= 0x10;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0042e700
bool CMover::IsAt(int p_x, int p_y, int& p_height)
{
	int x = (m_position.m_xFixed >> 12) - 8;
	int xMax = x + 15;
	int y = (m_position.m_yFixed >> 12) - 8;
	int yMax = y + 15;
	if (p_x >= x && p_x <= xMax && p_y >= y && p_y <= yMax) {
		p_height = (m_position.m_zFixed >> 12) + 8;
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

	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int groundX = x >> 4;
	int groundY = y >> 4;
	CMap* map = g_pMap;
	unsigned short z;
	if (x < 0 || y < 0 || map->m_ground.m_width <= groundX || map->m_ground.m_height <= groundY) {
		z = 0;
	}
	else {
		x &= 0xf;
		y &= 0xf;
		z = map->m_ground.m_ground[groundY * map->m_ground.m_width + groundX].GetZ(x, y);
	}

	m_active = 1;
	m_actionArgument = (short) p_pathId;
	m_position.m_zFixed = (unsigned int) z << 12;
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
	int x = nextPosition.m_x >> 12;
	int y = nextPosition.m_y >> 12;
	unsigned short z = 0;
	if (x >= 0 && y >= 0 && (x >> 4) < map->m_ground.m_width && map->m_ground.m_height > (y >> 4)) {
		z = map->m_ground.m_ground[(y >> 4) * map->m_ground.m_width + (x >> 4)].GetZ(x & 0xf, y & 0xf);
	}
	nextPosition.m_z = (unsigned int) z << 12;

	CPt3 start;
	start.m_x = m_position.m_xFixed >> 12;
	start.m_y = m_position.m_yFixed >> 12;
	start.m_z = m_position.m_zFixed >> 12;

	CPt3 end;
	end.m_x = nextPosition.m_x >> 12;
	end.m_y = nextPosition.m_y >> 12;
	end.m_z = nextPosition.m_z >> 12;

	unsigned int distance = Distance(start.m_x, start.m_y, end.m_x, end.m_y);
	m_lastMovementTick = p_time;
	m_actionDeadline = distance + p_time;
	m_motion.Set(start, end, p_time, 1);
}

// FUNCTION: LEMBALL 0x0042e980
void CMover::FindObjectsOnTopOfMe()
{
	int minX = (m_position.m_xFixed >> 12) - 8;
	int maxX = minX + 15;
	int minY = (m_position.m_yFixed >> 12) - 8;
	int maxY = minY + 15;
	int objectCount = (unsigned short) g_wObjectCount;
	if (objectCount > 0) {
		int index = 0;
		do {
			CGameObject* object = g_pObjects[(unsigned short) index];
			if (object != 0 && object->GetId() != (short) 0xffff && GetId() != object->GetId() &&
				object->m_objectType != OBJECT_SHEEP) {
				int objectX = object->m_position.m_xFixed >> 12;
				int objectY = object->m_position.m_yFixed >> 12;
				if (objectX >= minX && objectX <= maxX && objectY >= minY && objectY <= maxY) {
					GetOn(object);
				}
			}
			index++;
		} while (index < objectCount);
	}
}

// FUNCTION: LEMBALL 0x0042ea40
void CMover::MoveObjects(int p_deltaX, int p_deltaY, int p_deltaZ)
{
	for (int i = 0; i < m_objectCount; i++) {
		CGameObject* object = m_objects[i];
		object->m_position.m_xFixed += p_deltaX << 12;
		object->m_position.m_yFixed += p_deltaY << 12;
		object->m_position.m_zFixed += p_deltaZ << 12;
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
	bool local = g_pActiveConnection == 0 || g_pActiveConnection->m_isHost != 0;
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
			if (action == ACTION_2) {
				StopObjectsMoving();
				SetUpNextNode(time);
			}
			action = m_action;
			m_pendingAction = action;
		}
		else if (action != ACTION_2 && action != ACTION_0x24) {
			action = ACTION_0x18;
		}
	}
	VerifyObjects();
	switch (action) {
	case ACTION_NONE: {
		int next = m_currentNode + 1;
		if (m_nodeCount <= next) {
			next = 0;
		}
		int oldZ;
		int oldY;
		int oldX;
		oldX = m_position.m_xFixed;
		oldY = m_position.m_yFixed;
		oldZ = m_position.m_zFixed;
		m_currentNode = next;
		const CPt3& position = g_pAI->GetNodePosition(m_startNode + next);
		m_position.m_xFixed = position.m_x;
		m_position.m_yFixed = position.m_y;
		m_position.m_zFixed = position.m_z;
		int x = m_position.m_xFixed >> 12;
		int y = m_position.m_yFixed >> 12;
		int groundX = x >> 4;
		int groundY = y >> 4;
		CMap* map = g_pMap;
		unsigned short z;
		if (x < 0 || y < 0 || groundX >= g_pMap->m_ground.m_width || g_pMap->m_ground.m_height <= groundY) {
			z = 0;
		}
		else {
			z = map->m_ground.m_ground[groundY * map->m_ground.m_width + groundX].GetZ(x & 0xf, y & 0xf);
		}
		m_position.m_zFixed = (unsigned int) z << 12;
		int dx = -((oldX >> 12) - (m_position.m_xFixed >> 12));
		int dy = -((oldY >> 12) - (m_position.m_yFixed >> 12));
		int dz = -((oldZ >> 12) - (m_position.m_zFixed >> 12));
		MoveObjects(dx, dy, dz);
		if (m_movementMode != 0) {
			m_lastMovementTick = g_dwGameTick;
			if (local) {
				Action(ACTION_0x24);
			}
		}
		else {
			m_lastMovementTick = g_dwGameTick + 0x14;
			if (local) {
				Action(ACTION_1);
			}
		}
		break;
	}
	case ACTION_1:
		if (local) {
			if (g_dwGameTick < m_lastMovementTick) {
				return true;
			}
			StopObjectsMoving();
			Action(ACTION_2);
		}
		SetUpNextNode(time);
		break;
	case ACTION_2:
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
			MoveObjects(position.m_x - (m_position.m_xFixed >> 12),
						position.m_y - (m_position.m_yFixed >> 12),
						position.m_z - (m_position.m_zFixed >> 12));
			m_position.m_xFixed = position.m_x << 12;
			m_position.m_yFixed = position.m_y << 12;
			m_position.m_zFixed = position.m_z << 12;
		}
		break;
	case ACTION_0x14:
		SetUpNextNode(time);
		SetPos();
		if (local) {
			Action(ACTION_1);
		}
		break;
	case ACTION_0x18:
		if (local) {
			Action(ACTION_0x14);
		}
		break;
	case ACTION_0x24:
		if (m_switchRequested != 0) {
			Action(ACTION_1);
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
bool CMover::IsOn(const AiCoord& p_position)
{
	int minX = (m_position.m_xFixed >> 12) - 8;
	int minY = (m_position.m_yFixed >> 12) - 8;
	int maxX = minX + 15;
	int maxY = minY + 15;
	int x = p_position.m_xFixed >> 12;
	int y = p_position.m_yFixed >> 12;
	if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042ef40
void CMover::VerifyObjects()
{
	int minX = (m_position.m_xFixed >> 12) - 8;
	int maxX = minX + 15;
	int minY = (m_position.m_yFixed >> 12) - 8;
	int maxY = minY + 15;
	int i = 0;
	if (m_objectCount > 0) {
		do {
			int x;
			int y;
			CGameObject* object = m_objects[i];
			x = object->m_position.m_xFixed >> 12;
			y = object->m_position.m_yFixed >> 12;
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
	AiCoord objectPosition;
	objectPosition.m_xFixed = p_object->m_position.m_xFixed;
	objectPosition.m_yFixed = p_object->m_position.m_yFixed;
	objectPosition.m_zFixed = p_object->m_position.m_zFixed;
	int objectZ = objectPosition.m_zFixed >> 12;
	int moverZ = m_position.m_zFixed >> 12;
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
		if (m_action != ACTION_2 && p_object->m_objectType == OBJECT_PLAYER_2) {
			AiCoord destination(m_position.m_xFixed, m_position.m_yFixed, objectPosition.m_zFixed);
			p_object->AddDestination(destination);
			p_object->StartMoving();
		}
		else {
			objectPosition.m_zFixed = m_position.m_zFixed + 0x8000;
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
