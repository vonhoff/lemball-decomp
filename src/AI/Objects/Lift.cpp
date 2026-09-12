#include "Lift.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Network/Connect.h"
#include "../Base/Coord3d.h"

extern unsigned short g_wMovingLiftCount;

// 68K 0x106145ea __ct__5CLiftFv
// FUNCTION: LEMBALL 0x00424d00
Lift::Lift() : GlobalGameObject(0x212, 0, 0)
{
}

// 68K 0x10614682 CalculateCliff__5CLiftFv
// FUNCTION: LEMBALL 0x00424d30
void Lift::CalculateCliff()
{
	int startX = (short) (m_start.m_x / 16);
	int startY = (short) (m_start.m_y / 16);
	int endX = (short) (m_end.m_x / 16);
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
// FUNCTION: LEMBALL 0x00424df0
void Lift::Edit(int p_height,
				short p_direction,
				int p_lowHeight,
				int p_highHeight,
				eLiftActivateType p_activateType,
				unsigned int p_initialActive)
{
	if (p_lowHeight == -1) {
		p_lowHeight = p_height;
	}
	m_direction = p_direction;
	m_lowHeight = p_lowHeight;
	m_defaultActive = p_initialActive;
	m_highHeight = p_highHeight;
	m_activateType = p_activateType;
	m_action = 0x18;
	m_activationLatched = 0;
	for (int i = 0; i < 8; i++) {
		m_objects[i] = 0;
	}
	m_start.m_z = p_height;
	m_end.m_z = p_height;
	if (m_lowHeight == m_start.m_z && m_direction != 1) {
		m_direction = 1;
	}
	else if (m_highHeight == m_start.m_z && m_direction == 1) {
		m_direction = -1;
	}
	for (int x = m_start.m_x; x <= m_end.m_x; x += 16) {
		for (int y = m_start.m_y; y <= m_end.m_y; y += 16) {
			int bx = x / 16;
			int by = y / 16;
			switch (p_activateType) {
			case 0:
				m_active = 0;
				break;
			case 1:
				m_active = 0;
				break;
			case 2:
				Activate();
				break;
			case 3:
				m_active = 0;
				break;
			case 4:
				m_active = 0;
				m_defaultActive = 0;
				break;
			}
			if (bx >= 0 && by >= 0 && bx < g_pActiveMap->m_ground.m_width && by < g_pActiveMap->m_ground.m_height) {
				Ground* ground = g_pActiveMap->m_ground.m_ground + by * g_pActiveMap->m_ground.m_width + bx;
				ground->m_collision |= 0x8020;
			}
			m_mapCell = g_pActiveMap->m_ground.m_ground + g_pActiveMap->m_ground.m_width * by + bx;
			m_mapCell->m_height = p_height;
			m_mapCell->m_cliff = (p_height + 15) / 16;
		}
	}
	CalculateCliff();
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
// FUNCTION: LEMBALL 0x00425060
void Lift::Set(const Coord3d& p_start,
			   const Coord3d& p_end,
			   short p_direction,
			   int p_lowHeight,
			   int p_highHeight,
			   eLiftActivateType p_activateType,
			   unsigned int p_initialActive)
{
	m_position.m_xFixed = p_start.m_x << 12;
	m_position.m_yFixed = p_start.m_y << 12;
	m_position.m_zFixed = p_start.m_z << 12;
	m_liftId = g_wMovingLiftCount++;
	m_start = p_start;
	m_end = p_end;
	Edit(m_start.m_z, p_direction, p_lowHeight, p_highHeight, p_activateType, p_initialActive);
}

// 68K 0x10614b04 Process__5CLiftFv
// FUNCTION: LEMBALL 0x00425100
bool Lift::Process()
{
	unsigned int time;
	if (g_pActiveConnection != 0 && !g_pActiveConnection->m_isHost) {
		time = g_dwRemoteGameTick;
	}
	else {
		time = g_dwGameTick;
	}
	switch (m_action) {
	case 8:
		m_active = 0;
		m_action = 0x18;
		break;
	case 0x19:
		m_active = 1;
		m_activationLatched = 1;
		SetSndEffect(0x2c);
		if (m_active && (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost)) {
			m_stateTimer = time;
			if (m_direction == 1) {
				Action(0x1f);
			}
			else {
				Action(0x23);
			}
		}
		break;
	case 0x1f:
		m_movementStartHeight = m_start.m_z;
		m_direction = 1;
		m_active = 1;
		m_action = 0x20;
		m_start.m_z = m_movementStartHeight + time - m_stateTimer;
		break;
	case 0x20:
		m_start.m_z = (short) m_movementStartHeight - (short) m_stateTimer + (short) time;
		if (m_start.m_z >= m_highHeight) {
			m_start.m_z = m_highHeight;
			m_active = m_defaultActive;
			m_direction = -1;
			if (m_defaultActive && (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost)) {
				m_stateTimer = time;
				Action(0x23);
			}
			else {
				m_action = 0x18;
			}
		}
		break;
	case 0x22:
		m_start.m_z = (short) m_movementStartHeight - (short) time + (short) m_stateTimer;
		if (m_start.m_z <= m_lowHeight) {
			m_start.m_z = m_lowHeight;
			m_active = m_defaultActive;
			m_direction = 1;
			if (m_defaultActive && (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost)) {
				m_stateTimer = time;
				Action(0x1f);
			}
			else {
				m_action = 0x18;
			}
		}
		break;
	case 0x23:
		m_movementStartHeight = m_start.m_z;
		m_direction = -1;
		m_action = 0x22;
		m_active = 1;
		m_start.m_z = m_movementStartHeight - time + m_stateTimer;
		break;
	}
	int height = m_start.m_z;
	for (int y = m_start.m_y; y <= m_end.m_y; y += 16) {
		short startX = m_start.m_x;
		m_mapCell = g_pActiveMap->m_ground.m_ground + g_pActiveMap->m_ground.m_width * (y / 16) + (short) (startX / 16);
		for (int x = startX; x <= m_end.m_x; x += 16) {
			m_mapCell->m_height = height;
			m_mapCell->m_cliff = (height + 15) / 16;
			m_mapCell++;
		}
	}
	return true;
}

// 68K 0x10614dee CheckObjects__5CLiftFv
// FUNCTION: LEMBALL 0x00425440
void Lift::CheckObjects()
{
	int count = 8;
	GameObject** object = m_objects;
	do {
		if (*object != 0) {
			if ((*object)->QOnBalloon() || !(*object)->OnLift(m_start, m_end)) {
				(*object)->m_liftId = 0xffff;
				*object = 0;
			}
		}
		object++;
	} while (--count);
}

// 68K 0x10614ea0 StepOn__5CLiftFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x004254a0
int Lift::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	if (m_liftId == p_object->m_liftId) {
		return 1;
	}
	int startX = m_start.m_x - 8;
	int endX = m_end.m_x + 7;
	int startY = m_start.m_y - 8;
	int endY = m_end.m_y + 7;
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
