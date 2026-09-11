#include "Lift.h"

#include "../../Map/Base/Map.h"
#include "../Base/Coord3d.h"

// 68K 0x106145ea __ct__5CLiftFv
// FUNCTION: LEMBALL 0x00424d00
Lift::Lift() : GlobalGameObject(0x212, 0, 0)
{
}

// 68K 0x10614682 CalculateCliff__5CLiftFv
// FUNCTION: LEMBALL 0x00424d30
void Lift::CalculateCliff()
{
	int startX = (short) (m_startX / 16);
	int startY = (short) (m_startY / 16);
	int endX = (short) (m_endX / 16);
	if (startY > 0) {
		for (int x = startX; x <= endX; x++) {
			Ground* ground = &g_pActiveMap->m_ground.m_ground[(startY - 1) * g_pActiveMap->m_ground.m_width + x];
			ground->m_cliff = (short) (((short) ground->m_height + 15) / 16);
		}
	}
	if (startX > 0) {
		if (startY <= endX) {
			do {
				Ground* ground = &g_pActiveMap->m_ground.m_ground[startY * g_pActiveMap->m_ground.m_width + startX - 1];
				ground->m_cliff = (short) (((short) ground->m_height + 15) / 16);
				startY++;
			} while (startY <= endX);
		}
	}
}

// 68K 0x106147aa Edit__5CLiftFisii17eLiftActivateTypeUc
// STUB: LEMBALL 0x00424df0
void Lift::Edit(int p_height,
				short p_direction,
				int p_lowHeight,
				int p_highHeight,
				eLiftActivateType p_activateType,
				unsigned int p_initialActive)
{
}

// 68K 0x10614982 Set__5CLiftFiiisii17eLiftActivateTypeUc
// FUNCTION: LEMBALL 0x00425010
void Lift::Set(int p_x,
			   int p_y,
			   int p_z,
			   short p_direction,
			   int p_lowHeight,
			   int p_highHeight,
			   eLiftActivateType p_activateType,
			   unsigned int p_initialActive)
{
	Coord3d position;
	position.m_x = p_x;
	position.m_y = p_y;
	position.m_z = p_z;
	Set(position, position, p_direction, p_lowHeight, p_highHeight, p_activateType, p_initialActive);
}

// 68K 0x10614a04 Set__5CLiftFR8tCoord3dR8tCoord3dsii17eLiftActivateTypeUc
// STUB: LEMBALL 0x00425060
void Lift::Set(const Coord3d& p_start,
			   const Coord3d& p_end,
			   short p_direction,
			   int p_lowHeight,
			   int p_highHeight,
			   eLiftActivateType p_activateType,
			   unsigned int p_initialActive)
{
}

// 68K 0x10614b04 Process__5CLiftFv
// STUB: LEMBALL 0x00425100
bool Lift::Process()
{
	return 0;
}

// 68K 0x10614dee CheckObjects__5CLiftFv
// STUB: LEMBALL 0x00425440
void Lift::CheckObjects()
{
}

// 68K 0x10614ea0 StepOn__5CLiftFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x004254a0
int Lift::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	if (m_liftId == p_object->m_liftId) {
		return 1;
	}
	int startX = m_startX - 8;
	int endX = m_endX + 7;
	int startY = m_startY - 8;
	int endY = m_endY + 7;
	const AiCoord* position = &p_position;
	int x = position->m_xFixed >> 12;
	int y = position->m_yFixed >> 12;
	if (x >= startX && x <= endX && y >= startY && y <= endY) {
		int z = position->m_zFixed >> 12;
		Map* map = g_pActiveMap;
		int blockX = startX >> 4;
		int blockY = startY >> 4;
		unsigned short groundZ;
		if (startX < 0 || startY < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(startX & 0xf, startY & 0xf);
		}
		int minZ = groundZ - 2;
		if (minZ <= z && z <= minZ + 6) {
			int i = 0;
			GameObject** object = m_objects;
			do {
				if (*object == 0) {
					m_objects[i] = p_object;
					p_object->m_liftId = m_liftId;
					if (m_activateType == 1) {
						Activate();
						return 1;
					}
					if (m_activateType == 4 && m_activationLatched != 1) {
						Activate();
					}
					return 1;
				}
				object++;
				i++;
			} while (i < 8);
		}
	}
	if (m_liftId == p_object->m_liftId) {
		p_object->m_liftId = 0xffff;
	}
	return 0;
}

// 68K 0x10614ffc Activate__5CLiftFv
// FUNCTION: LEMBALL 0x00425640
int Lift::Activate()
{
	m_active = 1;
	Action((eAction) 0x19);
	return 1;
}

// 68K 0x1061503a ActivateDeactivate__5CLiftFv
// FUNCTION: LEMBALL 0x00425660
void Lift::ActivateDeactivate()
{
	if (m_active == 0) {
		Activate();
		return;
	}
	Action((eAction) 8);
}

// 68K 0x1011a746 DoActivate__5CLiftFv
// FUNCTION: LEMBALL 0x004266d0
void Lift::DoActivate()
{
}

// 68K 0x1061462a __dt__5CLiftFv
Lift::~Lift()
{
}
