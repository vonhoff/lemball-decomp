#include "Mover.h"

#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../Groups/PlayerLemmingGroup.h"
#include "../Objects/PlayerLemming.h"
#include "Ai.h"

// 68K 0x106171a6 __ct__6CMoverFv
// FUNCTION: LEMBALL 0x0042e590
Mover::Mover() : GlobalGameObject((eObjectType) 0x34, 0, 0)
{
}

// 68K 0x1061723c Restart__6CMoverFv
// FUNCTION: LEMBALL 0x0042e5e0
void Mover::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x1061726e Initialise__6CMoverFv
// FUNCTION: LEMBALL 0x0042e600
void Mover::Initialise()
{
	m_stateTimer = 0;
	m_active = 0;
	m_moving = 0;
	m_movementMode = 0;
	m_switchRequested = 0;
	m_objectCount = 0;
	m_findOccupants = 1;
	m_action = (eAction) 0x18;
}

// 68K 0x1061730e SetPos__6CMoverFv
// FUNCTION: LEMBALL 0x0042e650
void Mover::SetPos()
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
				Ground* ground = &g_pMap->m_ground.m_ground[groundY * g_pMap->m_ground.m_width + groundX];
				ground->m_collision |= 0x10;
			}
		}
	}
}

// 68K 0x106173a0 IsAt__6CMoverFiiRi
// FUNCTION: LEMBALL 0x0042e700
bool Mover::IsAt(int p_x, int p_y, int& p_height)
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

// 68K 0x10617410 Set__6CMoverFUsiUcii
// FUNCTION: LEMBALL 0x0042e760
void Mover::Set(unsigned short p_id, int p_pathId, undefined4 p_movementMode, int p_startNode, int p_nodeCount)
{
	SetId(p_id);
	const Pt3& position = g_pAI->GetNodePosition(p_startNode);
	m_position.m_xFixed = position.m_x;
	m_position.m_yFixed = position.m_y;
	m_position.m_zFixed = position.m_z;

	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int groundX = x >> 4;
	int groundY = y >> 4;
	Map* map = g_pMap;
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

// 68K 0x106174da SetUpNextNode__6CMoverFUl
// FUNCTION: LEMBALL 0x0042e850
void Mover::SetUpNextNode(unsigned int p_time)
{
	int nextNode = m_currentNode + 1;
	if (m_nodeCount <= nextNode) {
		nextNode = 0;
	}

	Pt3 nextPosition = g_pAI->GetNodePosition(m_startNode + nextNode);
	Map* map = g_pMap;
	int x = nextPosition.m_x >> 12;
	int y = nextPosition.m_y >> 12;
	unsigned short z = 0;
	if (x >= 0 && y >= 0 && (x >> 4) < map->m_ground.m_width && map->m_ground.m_height > (y >> 4)) {
		z = map->m_ground.m_ground[(y >> 4) * map->m_ground.m_width + (x >> 4)].GetZ(x & 0xf, y & 0xf);
	}
	nextPosition.m_z = (unsigned int) z << 12;

	Pt3 start;
	start.m_x = m_position.m_xFixed >> 12;
	start.m_y = m_position.m_yFixed >> 12;
	start.m_z = m_position.m_zFixed >> 12;

	Pt3 end;
	end.m_x = nextPosition.m_x >> 12;
	end.m_y = nextPosition.m_y >> 12;
	end.m_z = nextPosition.m_z >> 12;

	unsigned int distance = Distance(start.m_x, start.m_y, end.m_x, end.m_y);
	m_lastMovementTick = p_time;
	m_actionDeadline = distance + p_time;
	m_motion.Set(start, end, p_time, 1);
}

// 68K 0x1061760a FindObjectsOnTopOfMe__6CMoverFv
// FUNCTION: LEMBALL 0x0042e980
void Mover::FindObjectsOnTopOfMe()
{
	int minX = (m_position.m_xFixed >> 12) - 8;
	int maxX = minX + 15;
	int minY = (m_position.m_yFixed >> 12) - 8;
	int maxY = minY + 15;
	int objectCount = (unsigned short) g_wObjectCount;
	if (objectCount > 0) {
		int index = 0;
		do {
			GameObject* object = g_pObjects[(unsigned short) index];
			if (object != 0 && object->GetId() != (short) 0xffff && GetId() != object->GetId() &&
				object->m_objectType != (eObjectType) 7) {
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

// 68K 0x1061772a MoveObjects__6CMoverFiii
// FUNCTION: LEMBALL 0x0042ea40
void Mover::MoveObjects(int p_deltaX, int p_deltaY, int p_deltaZ)
{
	for (int i = 0; i < m_objectCount; i++) {
		GameObject* object = m_objects[i];
		object->m_position.m_xFixed += p_deltaX << 12;
		object->m_position.m_yFixed += p_deltaY << 12;
		object->m_position.m_zFixed += p_deltaZ << 12;
	}
}

// 68K 0x106177d8 Process__6CMoverFv
// STUB: LEMBALL 0x0042eb00
bool Mover::Process()
{
	return 0;
}

// 68K 0x10617b8e Switch__6CMoverFv
// FUNCTION: LEMBALL 0x0042eeb0
void Mover::Switch()
{
	m_switchRequested = 1;
}

// 68K 0x10617bb4 IsOn__6CMoverFR7AICOORD
// FUNCTION: LEMBALL 0x0042eee0
bool Mover::IsOn(const AiCoord& p_position)
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

// 68K 0x10617c36 VerifyObjects__6CMoverFv
// FUNCTION: LEMBALL 0x0042ef40
void Mover::VerifyObjects()
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
			GameObject* object = m_objects[i];
			x = object->m_position.m_xFixed >> 12;
			y = object->m_position.m_yFixed >> 12;
			if (minX > x || maxX < x || minY > y || maxY < y) {
				object->m_unk0x11c = 0;
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

// 68K 0x10617d36 GetOn__6CMoverFP11CGameObject
// FUNCTION: LEMBALL 0x0042eff0
bool Mover::GetOn(GameObject* p_object)
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
	GameObject** object;
	int count = m_objectCount;
	if (count < 10) {
		int i = 0;
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
		p_object->m_unk0x11c = 1;
		m_objectCount++;
		StopObjectsMoving();
		if (m_action != (eAction) 2 && p_object->m_objectType == (eObjectType) 2) {
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

// 68K 0x10617e94 StopObjectsMoving__6CMoverFv
// FUNCTION: LEMBALL 0x0042f140
void Mover::StopObjectsMoving()
{
	int index = 0;
	if (0 < m_objectCount) {
		GameObject** object = m_objects;
		do {
			if ((*object)->m_objectType == (eObjectType) 2) {
				((PlayerLemming*) *object)->GetGroup()->ClearExistingWaypoints();
			}
			else {
				(*object)->ResetInstructions();
			}
			object++;
			index++;
		} while (index < m_objectCount);
	}
}

// 68K 0x1011a914 DoActivate__6CMoverFv
// FUNCTION: LEMBALL 0x0042fb90
void Mover::DoActivate()
{
}

// 68K 0x106172b6 __dt__6CMoverFv
Mover::~Mover()
{
}
