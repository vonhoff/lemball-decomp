#include "Door.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"

// 68K 0x106062b0 __ct__5CDoorFv
// FUNCTION: LEMBALL 0x0040d470
Door::Door() : GlobalGameObject(0x19, 0, 0)
{
}

// 68K 0x106062f0 Restart__5CDoorFv
// FUNCTION: LEMBALL 0x0040d490
void Door::Restart()
{
	GlobalGameObject::Restart();
}

// 68K 0x10606316 Set__5CDoorF11eObjectTypeUsiii
// STUB: LEMBALL 0x0040d4a0
void Door::Set(eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z)
{
}

// 68K 0x106065bc Delete__5CDoorFv
// FUNCTION: LEMBALL 0x0040d760
void Door::Delete()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;

	switch (m_objectType) {
	case (eObjectType) 0x19:
		if (blockX >= 0) {
			if (blockY + 1 >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY + 1) {
				g_pMap->m_ground.m_ground[(blockY + 1) * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
			}
			if (blockX >= 0) {
				if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
				}
				if (blockX >= 0 && --blockY >= 0 && blockX < g_pMap->m_ground.m_width &&
					g_pMap->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
				}
			}
		}
		break;
	case (eObjectType) 0x1a:
		if (blockX + 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX + 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX + 1].m_collision &= ~1;
		}
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision &= ~1;
		}
		break;
	}
}

// 68K 0x106066c2 SetCollision__5CDoorFv
// FUNCTION: LEMBALL 0x0040d910
void Door::SetCollision()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;

	switch (m_objectType) {
	case (eObjectType) 0x19:
		if (blockX >= 0) {
			if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
			}
			if (blockX >= 0 && --blockY >= 0 && blockX < g_pMap->m_ground.m_width &&
				g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
			}
		}
		break;
	case (eObjectType) 0x1a:
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision |= 1;
		}
		break;
	}
}

// 68K 0x10606794 ResetCollision__5CDoorFv
// FUNCTION: LEMBALL 0x0040da40
void Door::ResetCollision()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;

	switch (m_objectType) {
	case (eObjectType) 0x19:
		if (blockX >= 0) {
			if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
			}
			if (blockX >= 0 && --blockY >= 0 && blockX < g_pMap->m_ground.m_width &&
				g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
			}
		}
		break;
	case (eObjectType) 0x1a:
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision &= ~1;
		}
		break;
	}
}

// 68K 0x10606868 Process__5CDoorFv
// FUNCTION: LEMBALL 0x0040db80
bool Door::Process()
{
	if (m_isRemoteObject) {
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case (eAction) DOOR_ACTION_OPENING:
				SetSndEffect((eSoundEffect) 0xb);
				ResetCollision();
				break;
			case (eAction) DOOR_ACTION_CLOSING:
				SetCollision();
				break;
			default:
				break;
			}
			m_pendingAction = m_action;
		}
		return 1;
	}

	if (!m_activationPending) {
		return 1;
	}

	if (m_actionDeadline > g_dwGameTick) {
		return 1;
	}

	m_stateTimer = g_dwSimulationTimestamp;
	switch (m_action) {
	case (eAction) 0x1c:
		Action((eAction) 0x1d);
		m_activationPending = 0;
		break;
	case (eAction) DOOR_ACTION_OPENING:
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionDeadline = g_dwGameTick + 80;
		ResetCollision();
		Action((eAction) DOOR_ACTION_OPEN);
		break;
	case (eAction) DOOR_ACTION_OPEN:
		if (m_doorType == 0) {
			m_stateTimer = g_dwSimulationTimestamp;
			m_actionDeadline = g_dwGameTick + 20;
			SetCollision();
			SetSndEffect((eSoundEffect) 0xb);
			Action((eAction) DOOR_ACTION_CLOSING);
			return 1;
		}
		m_activationPending = 0;
		break;
	case (eAction) DOOR_ACTION_CLOSING:
		m_activationPending = 0;
		Action((eAction) 0x1e);
		break;
	default:
		break;
	}

	return 1;
}

// 68K 0x106069f4 Unlock__5CDoorFv
// FUNCTION: LEMBALL 0x0040dd00
void Door::Unlock()
{
	if (m_action >= (eAction) 0x1c && m_action <= (eAction) 0x1d) {
		m_actionDeadline = 0x14;
		SetSndEffect((eSoundEffect) 0xb);
		RequestAction((eAction) DOOR_ACTION_OPENING);
	}
}

// 68K 0x10606a54 IsUsable__5CDoorF7eAction
// FUNCTION: LEMBALL 0x0040dd30
bool Door::IsUsable(eAction p_action)
{
	return p_action == (eAction) 0x18 || (p_action >= (eAction) 0x1d && p_action <= (eAction) 0x1e);
}

// 68K 0x10606a96 Hits__5CDoorFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0040dd80
int Door::Hits(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x10606be8 DoActivate__5CDoorFv
// STUB: LEMBALL 0x0040dec0
void Door::DoActivate()
{
}

// 68K 0x10119b74 __dt__5CDoorFv
Door::~Door()
{
}
