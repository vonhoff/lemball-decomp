#include "CLift.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/tCoord3d.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

#include <stddef.h>

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
	int startX = (short) (m_start.m_x / GROUND_BLOCK_PIXEL_SIZE);
	int startY = (short) (m_start.m_y / GROUND_BLOCK_PIXEL_SIZE);
	int endX = (short) (m_end.m_x / GROUND_BLOCK_PIXEL_SIZE);
	if (startY > 0) {
		for (int x = startX; x <= endX; x++) {
			CGround* ground = &g_pActiveMap->m_ground.m_ground[(startY - 1) * g_pActiveMap->m_ground.m_width + x];
			ground->m_cliff = (short) (((short) ground->m_height + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE);
		}
	}
	if (startX > 0) {
		if (startY <= endX) {
			do {
				CGround* ground =
					&g_pActiveMap->m_ground.m_ground[startY * g_pActiveMap->m_ground.m_width + startX - 1];
				ground->m_cliff =
					(short) (((short) ground->m_height + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE);
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
	if (p_lowHeight == LIFT_LOW_HEIGHT_FOLLOWS_START_HEIGHT) {
		p_lowHeight = p_height;
	}
	m_lowHeight = p_lowHeight;
	m_highHeight = p_highHeight;
	m_direction = p_direction;
	m_defaultActive = p_initialActive;
	m_activateType = p_activateType;
	m_action = ACTION_READY;
	m_activationLatched = LIFT_ACTIVATION_NOT_LATCHED;
	for (int i = 0; i < 8; i++) {
		m_objects[i] = NULL;
	}
	m_start.m_z = p_height;
	m_end.m_z = p_height;
	if (m_lowHeight == m_start.m_z && m_direction != LIFT_DIRECTION_RISING) {
		m_direction = LIFT_DIRECTION_RISING;
	}
	else if (m_highHeight == m_start.m_z && m_direction == LIFT_DIRECTION_RISING) {
		m_direction = LIFT_DIRECTION_LOWERING;
	}
	for (int x = m_start.m_x; x <= m_end.m_x; x += 16) {
		for (int y = m_start.m_y; y <= m_end.m_y; y += 16) {
			int bx = x / GROUND_BLOCK_PIXEL_SIZE;
			int by = y / GROUND_BLOCK_PIXEL_SIZE;
			switch (p_activateType) {
			case LIFT_ACTIVATE_SWITCH_TOGGLE:
				m_active = 0;
				break;
			case LIFT_ACTIVATE_STEP:
				m_active = 0;
				break;
			case LIFT_ACTIVATE_CONTINUOUS:
				Activate();
				break;
			case LIFT_ACTIVATE_SWITCH_ONCE:
				m_active = 0;
				break;
			case LIFT_ACTIVATE_STEP_ONCE:
				m_active = 0;
				m_defaultActive = 0;
				break;
			}
			if (bx >= 0 && by >= 0 && bx < g_pActiveMap->m_ground.m_width && by < g_pActiveMap->m_ground.m_height) {
				CGround* ground = g_pActiveMap->m_ground.m_ground + by * g_pActiveMap->m_ground.m_width + bx;
				ground->m_collision |= GROUND_COLLISION_OBJECT_INTERACTION | GROUND_COLLISION_SPECIAL_RENDER;
			}
			m_mapCell = g_pActiveMap->m_ground.m_ground + g_pActiveMap->m_ground.m_width * by + bx;
			m_mapCell->m_height = p_height;
			m_mapCell->m_cliff = (p_height + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE;
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
	tCoord3d position;
	position.m_x = p_x;
	position.m_y = p_y;
	position.m_z = p_z;
	Set(position, position, p_direction, p_lowHeight, p_highHeight, p_activateType, p_initialActive);
}

// FUNCTION: LEMBALL 0x00425060
void CLift::Set(tCoord3d& p_start,
				tCoord3d& p_end,
				short p_direction,
				int p_lowHeight,
				int p_highHeight,
				eLiftActivateType p_activateType,
				unsigned int p_initialActive)
{
	int startY = p_start.m_y;
	int startZ = p_start.m_z;
	int startX = p_start.m_x;
	m_position.m_yFixed = startY << FIXED_POINT_FRACTION_BITS;
	m_position.m_zFixed = startZ << FIXED_POINT_FRACTION_BITS;
	m_position.m_xFixed = startX << FIXED_POINT_FRACTION_BITS;
	m_liftId = g_wMovingLiftCount++;
	m_start = p_start;
	m_end = p_end;
	Edit(m_start.m_z, p_direction, p_lowHeight, p_highHeight, p_activateType, p_initialActive);
}

// FUNCTION: LEMBALL 0x00425100
bool CLift::Process()
{
	unsigned int time;
	if (g_pActiveConnection != NULL && !g_pActiveConnection->m_isHost) {
		time = g_dwRemoteGameTick;
	}
	else {
		time = g_dwGameTick;
	}
	switch (m_action) {
	case ACTION_DEAD:
		m_active = 0;
		m_action = ACTION_READY;
		break;
	case ACTION_ACTIVATING:
		m_active = 1;
		m_activationLatched = LIFT_ACTIVATION_LATCHED;
		SetSndEffect(SFX_LIFT);
		if (m_active && (g_pActiveConnection == NULL || g_pActiveConnection->m_isHost)) {
			m_stateTimer = time;
			if (m_direction == LIFT_DIRECTION_RISING) {
				Action(ACTION_LIFT_START_RISING);
			}
			else {
				Action(ACTION_LIFT_START_LOWERING);
			}
		}
		break;
	case ACTION_LIFT_START_RISING: {
		int startHeight = m_start.m_z;
		m_movementStartHeight = startHeight;
		m_direction = LIFT_DIRECTION_RISING;
		m_start.m_z = startHeight + time - m_stateTimer;
		m_active = 1;
		m_action = ACTION_LIFT_RISING;
		break;
	}
	case ACTION_LIFT_RISING:
		m_start.m_z = m_movementStartHeight - m_stateTimer + time;
		if (m_start.m_z >= m_highHeight) {
			m_start.m_z = m_highHeight;
			m_active = m_defaultActive;
			m_direction = LIFT_DIRECTION_LOWERING;
			if (m_defaultActive && (g_pActiveConnection == NULL || g_pActiveConnection->m_isHost)) {
				m_stateTimer = time;
				Action(ACTION_LIFT_START_LOWERING);
			}
			else {
				m_action = ACTION_READY;
			}
		}
		break;
	case ACTION_LIFT_LOWERING:
		m_start.m_z = m_movementStartHeight - time + m_stateTimer;
		if (m_start.m_z <= m_lowHeight) {
			m_start.m_z = m_lowHeight;
			m_active = m_defaultActive;
			m_direction = LIFT_DIRECTION_RISING;
			if (m_defaultActive && (g_pActiveConnection == NULL || g_pActiveConnection->m_isHost)) {
				m_stateTimer = time;
				Action(ACTION_LIFT_START_RISING);
			}
			else {
				m_action = ACTION_READY;
			}
		}
		break;
	case ACTION_LIFT_START_LOWERING:
		m_movementStartHeight = m_start.m_z;
		m_direction = LIFT_DIRECTION_LOWERING;
		m_start.m_z = m_movementStartHeight - time + m_stateTimer;
		m_action = ACTION_LIFT_LOWERING;
		m_active = 1;
		break;
	}
	int height = m_start.m_z;
	for (int y = m_start.m_y; y <= m_end.m_y; y += 16) {
		short startX = m_start.m_x;
		m_mapCell = g_pActiveMap->m_ground.m_ground + g_pActiveMap->m_ground.m_width * (y / GROUND_BLOCK_PIXEL_SIZE) +
					(short) (startX / GROUND_BLOCK_PIXEL_SIZE);
		for (int x = startX; x <= m_end.m_x; x += 16) {
			m_mapCell->m_height = height;
			m_mapCell->m_cliff = (height + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE;
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
		if (*object != NULL) {
			if ((*object)->QOnBalloon() || !(*object)->OnLift(m_start, m_end)) {
				(*object)->m_liftId = INVALID_OBJECT_ID;
				*object = NULL;
			}
		}
		object++;
	} while (--count);
}

// FUNCTION: LEMBALL 0x00425640
int CLift::Activate()
{
	m_active = 1;
	Action(ACTION_ACTIVATING);
	return 1;
}

// FUNCTION: LEMBALL 0x00425660
void CLift::ActivateDeactivate()
{
	if (m_active == 0) {
		Activate();
		return;
	}
	Action(ACTION_DEAD);
}

// FUNCTION: LEMBALL 0x004266d0
void CLift::DoActivate()
{
}
