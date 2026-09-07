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
// STUB: LEMBALL 0x0042e650
void Mover::SetPos()
{
}

// 68K 0x10617410 Set__6CMoverFUsiUcii
// STUB: LEMBALL 0x0042e760
void Mover::Set(unsigned short p_id, int p_pathId, undefined4 p_movementMode, int p_startNode, int p_nodeCount)
{
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
// STUB: LEMBALL 0x0042e980
unsigned int Mover::FindObjectsOnTopOfMe()
{
	return 0;
}

// 68K 0x1061772a MoveObjects__6CMoverFiii
// STUB: LEMBALL 0x0042ea40
void Mover::MoveObjects(int p_deltaX, int p_deltaY, int p_deltaZ)
{
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
// STUB: LEMBALL 0x0042ef40
void Mover::VerifyObjects()
{
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
	if (objectZ < moverZ - 16 || objectZ > moverZ + 16 || IsOn(objectPosition) == 0) {
		return false;
	}

	if (m_objectCount >= 10) {
		return false;
	}
	GameObject** object = m_objects;
	int i = 0;
	if (m_objectCount > 0) {
		do {
			if (*object == p_object) {
				return true;
			}
			object++;
			i++;
		} while (i < m_objectCount);
	}

	m_objects[m_objectCount] = p_object;
	p_object->m_unk0x11c = 1;
	m_objectCount++;
	StopObjectsMoving();
	if (m_action == (eAction) 2 && p_object->m_objectType == (eObjectType) 2) {
		AiCoord destination(m_position.m_xFixed, m_position.m_yFixed, objectPosition.m_zFixed);
		p_object->AddDestination(destination);
		p_object->StartMoving();
	}
	return true;
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
