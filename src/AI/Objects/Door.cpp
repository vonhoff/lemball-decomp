#include "Door.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/GameObject.h"
#include "AI/Base/GlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/Ground.h"
#include "Map/Ground/GroundArray.h"
#include "Views/Sound/SoundEffects.h"

extern unsigned short g_wNextDoorIndex;

// 68K 0x106062b0 __ct__5CDoorFv
// FUNCTION: LEMBALL 0x0040d470
Door::Door() : GlobalGameObject(OBJECT_DOOR_1, 0, 0)
{
}

// 68K 0x106062f0 Restart__5CDoorFv
// FUNCTION: LEMBALL 0x0040d490
void Door::Restart()
{
	GlobalGameObject::Restart();
}

// 68K 0x10606316 Set__5CDoorF11eObjectTypeUsiii
// FUNCTION: LEMBALL 0x0040d4a0
void Door::Set(eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z)
{
	m_objectType = p_objectType;
	m_spawnPosition.m_xFixed = p_x << 12;
	m_spawnPosition.m_yFixed = p_y << 12;
	m_spawnPosition.m_zFixed = p_z << 12;
	m_doorType = p_doorType;
	m_doorIndex = g_wNextDoorIndex++;
	m_action = (eAction) 0x1e;
	if (m_doorType != 0) {
		m_action = (eAction) 0x1d;
	}
	switch ((unsigned short) m_doorType) {
	case 0:
		m_actionArgument = -1;
		break;
	case 1:
		m_actionArgument = 0x15;
		break;
	case 2:
		m_actionArgument = 0x16;
		break;
	case 3:
		m_actionArgument = 0x17;
		break;
	case 4:
		m_actionArgument = 0x14;
		break;
	}
	m_setTick = g_dwGameTick;
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_activationPending = 0;
	int blockY;
	int blockX = p_x / 16;
	blockY = p_y / 16;
	Map* groundMap;
	switch (m_objectType) {
	case 0x19:
		groundMap = g_pMap;
		if (blockX >= 0 && blockY + 1 >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY + 1 < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY + 1)->m_collision |= 0x8000;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY + 1 >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY + 1 < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY + 1)->m_collision |= 1;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= 0x8000;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= 1;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY - 1 >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY - 1 < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY - 1)->m_collision |= 0x8000;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && --blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= 1;
		}
		break;
	case 0x1a:
		g_pMap->m_ground.SetCollision(blockX + 1, blockY, 0x8000);
		g_pMap->m_ground.SetCollision(blockX + 1, blockY, 1);
		g_pMap->m_ground.SetCollision(blockX, blockY, 0x8000);
		g_pMap->m_ground.SetCollision(blockX, blockY, 1);
		g_pMap->m_ground.SetCollision(--blockX, blockY, 0x8000);
		groundMap = g_pMap;
		if (blockX >= 0 && blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= 1;
		}
		break;
	}
}

// 68K 0x106065bc Delete__5CDoorFv
// FUNCTION: LEMBALL 0x0040d760
void Door::Delete()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;

	switch (m_objectType) {
	case (eObjectType) 0x19:
		if (blockX >= 0 && blockY + 1 >= 0 && blockX < g_pMap->m_ground.m_width &&
			g_pMap->m_ground.m_height > blockY + 1) {
			g_pMap->m_ground.m_ground[(blockY + 1) * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
		}
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
		}
		if (blockX >= 0 && --blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
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
			if (blockX >= 0 && --blockY >= 0) {
				Map* map = g_pMap;
				int width = map->m_ground.m_width;
				if (blockX < width && map->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= 1;
				}
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
			if (blockX >= 0 && --blockY >= 0) {
				Map* map = g_pMap;
				int width = map->m_ground.m_width;
				if (blockX < width && map->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision &= ~1;
				}
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
				SetSndEffect(SFX_DOOROPEN);
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
			SetSndEffect(SFX_DOOROPEN);
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
		SetSndEffect(SFX_DOOROPEN);
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
// FUNCTION: LEMBALL 0x0040dd80
int Door::Hits(const AiCoord& p_position, GameObject* p_object)
{
	int x = p_position.m_xFixed >> 12;
	int y = p_position.m_yFixed >> 12;
	int doorX = m_position.m_xFixed >> 12;
	int doorY = m_position.m_yFixed >> 12;
	doorX -= 40;
	doorY -= 8;
	int maxX = doorX + 48;
	int maxY = doorY + 16;
	if (doorX <= x && maxX >= x && doorY <= y && maxY >= y) {
		switch (m_action) {
		case (eAction) 0x1c:
		case (eAction) 0x1d:
			if (p_object->HasObject((eObjectType) (unsigned short) m_actionArgument)) {
				m_actionDeadline = 20;
				SetSndEffect(SFX_DOOROPEN);
				RequestAction((eAction) 0x20);
				return 1;
			}
			m_actionDeadline = 40;
			RequestAction((eAction) 0x1c);
			return 0;
		case (eAction) 0x1e:
			m_actionDeadline = 20;
			SetSndEffect(SFX_DOOROPEN);
			RequestAction((eAction) 0x20);
			return 1;
		case (eAction) 0x20:
			return 1;
		case (eAction) 0x21:
			return 0;
		case (eAction) 0x22:
			return 1;
		default:
			return 1;
		}
	}
	return 0;
}

// 68K 0x10606be8 DoActivate__5CDoorFv
// FUNCTION: LEMBALL 0x0040dec0
void Door::DoActivate()
{
	m_activationPending = 1;
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline += g_dwGameTick;
	if (m_action != (eAction) 0x1c) {
		int actionArgument = (unsigned short) m_actionArgument;
		int score;
		switch (actionArgument) {
		case 0x14:
			score = 0x19;
			break;
		case 0x15:
		case 0x16:
		case 0x17:
			score = 0x4b;
			break;
		default:
			score = 0x19;
			break;
		}
		g_pAI->Score(score);
	}
}
