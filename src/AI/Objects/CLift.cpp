#include "CLift.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/Coord3d.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

extern unsigned short g_wMovingLiftCount;

// FUNCTION: LEMBALL 0x00424d00
CLift::CLift() : CGlobalGameObject(TERRAIN_LIFT, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00424d20
CLift::~CLift()
{
}

// FUNCTION: LEMBALL 0x00424d30
void CLift::CalculateCliff()
{
	int startX = (short) (m_start.m_x / 16);
	int startY = (short) (m_start.m_y / 16);
	int endX = (short) (m_end.m_x / 16);
	if (startY > 0) {
		for (int x = startX; x <= endX; x++) {
			CGround* ground = &g_pActiveMap->m_ground.m_ground[(startY - 1) * g_pActiveMap->m_ground.m_width + x];
			ground->m_cliff = (short) (((short) ground->m_height + 15) / 16);
		}
	}
	if (startX > 0) {
		if (startY <= endX) {
			do {
				CGround* ground =
					&g_pActiveMap->m_ground.m_ground[startY * g_pActiveMap->m_ground.m_width + startX - 1];
				ground->m_cliff = (short) (((short) ground->m_height + 15) / 16);
				startY++;
			} while (startY <= endX);
		}
	}
}

// FUNCTION: LEMBALL 0x00424df0
void CLift::Edit(int p_height,
				 short p_direction,
				 int p_lowHeight,
				 int p_highHeight,
				 eLiftActivateType p_activateType,
				 unsigned int p_initialActive)
{
	if (p_lowHeight == -1) {
		p_lowHeight = p_height;
	}
	m_lowHeight = p_lowHeight;
	m_highHeight = p_highHeight;
	m_direction = p_direction;
	m_defaultActive = p_initialActive;
	m_activateType = p_activateType;
	m_action = ACTION_0x18;
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
				CGround* ground = g_pActiveMap->m_ground.m_ground + by * g_pActiveMap->m_ground.m_width + bx;
				ground->m_collision |= 0x8020;
			}
			m_mapCell = g_pActiveMap->m_ground.m_ground + g_pActiveMap->m_ground.m_width * by + bx;
			m_mapCell->m_height = p_height;
			m_mapCell->m_cliff = (p_height + 15) / 16;
		}
	}
	CalculateCliff();
}

// FUNCTION: LEMBALL 0x00425010
void CLift::Set(int p_x,
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

// FUNCTION: LEMBALL 0x00425060
void CLift::Set(const Coord3d& p_start,
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

// FUNCTION: LEMBALL 0x00425100
bool CLift::Process()
{
	unsigned int time;
	if (g_pActiveConnection != 0 && !g_pActiveConnection->m_isHost) {
		time = g_dwRemoteGameTick;
	}
	else {
		time = g_dwGameTick;
	}
	switch (m_action) {
	case ACTION_8:
		m_active = 0;
		m_action = ACTION_0x18;
		break;
	case ACTION_0x19:
		m_active = 1;
		m_activationLatched = 1;
		SetSndEffect(SFX_LIFT);
		if (m_active && (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost)) {
			m_stateTimer = time;
			if (m_direction == 1) {
				Action(ACTION_0x1f);
			}
			else {
				Action(ACTION_0x23);
			}
		}
		break;
	case ACTION_0x1f:
		m_movementStartHeight = m_start.m_z;
		m_direction = 1;
		m_active = 1;
		m_action = ACTION_0x20;
		m_start.m_z = m_movementStartHeight + time - m_stateTimer;
		break;
	case ACTION_0x20:
		m_start.m_z = (short) m_movementStartHeight - (short) m_stateTimer + (short) time;
		if (m_start.m_z >= m_highHeight) {
			m_start.m_z = m_highHeight;
			m_active = m_defaultActive;
			m_direction = -1;
			if (m_defaultActive && (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost)) {
				m_stateTimer = time;
				Action(ACTION_0x23);
			}
			else {
				m_action = ACTION_0x18;
			}
		}
		break;
	case ACTION_0x22:
		m_start.m_z = (short) m_movementStartHeight - (short) time + (short) m_stateTimer;
		if (m_start.m_z <= m_lowHeight) {
			m_start.m_z = m_lowHeight;
			m_active = m_defaultActive;
			m_direction = 1;
			if (m_defaultActive && (g_pActiveConnection == 0 || g_pActiveConnection->m_isHost)) {
				m_stateTimer = time;
				Action(ACTION_0x1f);
			}
			else {
				m_action = ACTION_0x18;
			}
		}
		break;
	case ACTION_0x23:
		m_movementStartHeight = m_start.m_z;
		m_direction = -1;
		m_action = ACTION_0x22;
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

// FUNCTION: LEMBALL 0x00425440
void CLift::CheckObjects()
{
	int count = 8;
	CGameObject** object = m_objects;
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

// FUNCTION: LEMBALL 0x004254a0
int CLift::StepOn(const AiCoord& p_position, CGameObject* p_object)
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
		CMap* map = g_pActiveMap;
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
			CGameObject** object = m_objects;
			do {
				if (*object == 0) {
					m_objects[i] = p_object;
					p_object->m_liftId = m_liftId;
					if (m_activateType == LIFT_ACTIVATE_STEP) {
						Activate();
						return 1;
					}
					if (m_activateType == LIFT_ACTIVATE_STEP_ONCE && m_activationLatched != 1) {
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

// FUNCTION: LEMBALL 0x00425640
int CLift::Activate()
{
	m_active = 1;
	Action(ACTION_0x19);
	return 1;
}

// FUNCTION: LEMBALL 0x00425660
void CLift::ActivateDeactivate()
{
	if (m_active == 0) {
		Activate();
		return;
	}
	Action(ACTION_8);
}

// FUNCTION: LEMBALL 0x004266d0
void CLift::DoActivate()
{
}
