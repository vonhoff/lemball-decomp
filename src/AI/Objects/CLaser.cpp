#include "CLaser.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVsMath.h"
#include "../../Visos/Network/CConnect.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "CViewData.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x00428890
CLaser::CLaser() : CGlobalGameObject(OBJECT_LASER_VERTICAL, 0, 0)
{
}

// FUNCTION: LEMBALL 0x004288b0
void CLaser::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x004288d0
void CLaser::Initialise()
{
	m_action = ACTION_0x18;
	m_stateTimer = 0;
	m_active = 0;
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x004288f0
CLaser::~CLaser()
{
}

// FUNCTION: LEMBALL 0x00428900
void CLaser::Set(unsigned short p_id, const AiCoord& p_position, eObjectType p_orientation)
{
	SetId(p_id);
	int x = p_position.m_xFixed;
	m_position.m_xFixed = x;
	int y = p_position.m_yFixed;
	int blockX = (x >> 12) / 16;
	m_position.m_yFixed = y;
	m_position.m_zFixed = p_position.m_zFixed;
	int blockY = (y >> 12) / 16;
	m_enabled = 1;
	m_objectType = p_orientation;

	switch (p_orientation) {
	case OBJECT_LASER_HORIZONTAL:
		m_action = ACTION_0x18;
		m_autoActivate = 1;
		m_active = 1;
		break;
	case OBJECT_LASER_VERTICAL:
		m_action = ACTION_0x18;
		m_autoActivate = 1;
		m_active = 1;
		break;
	case OBJECT_LASER_EMITTER_H: {
		m_autoActivate = 0;
		m_action = ACTION_0x18;
		m_active = 1;
		for (int i = 1; i < 8; i++) {
			int collisionX = blockX + i;
			if (collisionX >= 0 && blockY >= 0 && collisionX < g_pMap->m_ground.m_width &&
				blockY < g_pMap->m_ground.m_height) {
				g_pMap->m_ground.m_ground[g_pMap->m_ground.m_width * blockY + collisionX].m_collision |= 0x8000;
			}
		}
		break;
	}
	case OBJECT_LASER_EMITTER_V: {
		m_autoActivate = 0;
		m_action = ACTION_0x18;
		m_active = 1;
		for (int i = 1; i < 8; i++) {
			int collisionY = blockY + i;
			if (blockX >= 0 && collisionY >= 0 && blockX < g_pMap->m_ground.m_width &&
				collisionY < g_pMap->m_ground.m_height) {
				g_pMap->m_ground.m_ground[g_pMap->m_ground.m_width * collisionY + blockX].m_collision |= 0x8000;
			}
		}
		break;
	}
	}
	m_actionDeadline = g_dwGameTick + 0x3c;
}

// STUB: LEMBALL 0x00428ab0
bool CLaser::CheckHits()
{
	return 0;
}

// FUNCTION: LEMBALL 0x00428cf0
bool CLaser::Process()
{
	if (m_isRemoteObject != 0) {
		m_active = m_action != ACTION_0x18;
		if (m_action == ACTION_0x1a && m_target == 0) {
			CheckHits();
		}
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case ACTION_0x17:
				if (m_target != 0) {
					m_target->m_unk0x2c = 1;
					m_target = 0;
				}
				Action(ACTION_0x18);
				break;
			case ACTION_0x19:
				m_target = 0;
				break;
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (g_pActiveConnection != 0 && g_pActiveConnection->m_isHost != 0) {
		return 1;
	}
	if (m_active != 0) {
		switch (m_action) {
		case ACTION_0x17:
			if (m_target != 0) {
				m_target->SetSndEffect(SFX_ELECCY);
				m_target->m_unk0x2c = 1;
				m_target = 0;
			}
			Action(ACTION_0x18);
			return 1;
		case ACTION_0x18:
			if (m_actionDeadline < g_dwGameTick) {
				Activate();
				return 1;
			}
			break;
		case ACTION_0x19:
			if (m_unk0xd0 < g_dwGameTick) {
				m_target = 0;
				Action(ACTION_0x1a);
				return 1;
			}
			break;
		case ACTION_0x1a:
			if (m_target == 0) {
				CheckHits();
			}
			if (m_actionDeadline < g_dwGameTick) {
				m_enabled = 1;
				m_active = m_autoActivate;
				m_actionDeadline = g_dwGameTick + 0x3c;
				if (m_target != 0) {
					m_target->m_unk0x2c = 1;
					m_target = 0;
				}
				Action(ACTION_0x17);
			}
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00428ec0
bool CLaser::Activate()
{
	m_active = 1;
	if (g_pActiveConnection != 0 && g_pActiveConnection->m_isHost != 0) {
		return 0;
	}
	m_lastMovementTick = g_dwGameTick;
	m_unk0xd0 = g_dwGameTick + 6;
	m_actionDeadline = g_dwGameTick + 0x18;
	m_target = 0;
	m_stateTimer = g_dwSimulationTimestamp;
	Action(ACTION_0x19);
	return 1;
}

// FUNCTION: LEMBALL 0x00428f30
bool CLaser::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	if (p_object->m_objectType == OBJECT_PLAYER_2 && (int) Distance(m_position.m_xFixed >> 12,
																	m_position.m_yFixed >> 12,
																	p_position.m_xFixed >> 12,
																	p_position.m_yFixed >> 12) < 0x30) {
		Activate();
		m_target = p_object;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00428f90
int CLaser::GetViewData(CViewData* p_viewData)
{
	CGameObject::GetViewData(*p_viewData++);
	int count = 1;
	if (m_action == ACTION_0x1a) {
		switch (m_objectType) {
		case OBJECT_LASER_HORIZONTAL:
		case OBJECT_LASER_EMITTER_H: {
			int x = (m_position.m_xFixed >> 12) + 0x26;
			int y = m_position.m_yFixed >> 12;
			int z = (m_position.m_zFixed >> 12) + 3;
			for (unsigned int i = 1; i < 8; i++) {
				unsigned short height = 0;
				int blockX = x >> 4;
				int blockY = y >> 4;
				if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && blockY < g_pMap->m_ground.m_height) {
					height = g_pMap->m_ground.GetGroundCell(blockX, blockY)->GetZ(x & 0xf, y & 0xf);
				}
				if (height > z) {
					break;
				}
				CGameObject::GetViewData(*p_viewData);
				p_viewData->m_positionX = x;
				p_viewData->m_positionY = y;
				p_viewData->m_positionZ = z;
				p_viewData->m_objectType = OBJECT_LASER_VERTICAL_BEAM;
				p_viewData->m_facingDirection = 0;
				p_viewData->m_action = ACTION_0x1a;
				p_viewData++;
				count++;
				x += 0x10;
			}
			break;
		}
		case OBJECT_LASER_VERTICAL:
		case OBJECT_LASER_EMITTER_V: {
			int x = m_position.m_xFixed >> 12;
			int y = (m_position.m_yFixed >> 12) + 0x14;
			int z = (m_position.m_zFixed >> 12) + 3;
			for (unsigned int i = 1; i < 8; i++) {
				unsigned short height = 0;
				int blockX = x >> 4;
				int blockY = y >> 4;
				if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && blockY < g_pMap->m_ground.m_height) {
					height = g_pMap->m_ground.GetGroundCell(blockX, blockY)->GetZ(x & 0xf, y & 0xf);
				}
				if (height > z) {
					break;
				}
				CGameObject::GetViewData(*p_viewData);
				p_viewData->m_positionX = x;
				p_viewData->m_positionY = y;
				p_viewData->m_positionZ = z;
				p_viewData->m_objectType = OBJECT_LASER_HORIZONTAL_BEAM;
				p_viewData->m_facingDirection = 0;
				p_viewData->m_action = ACTION_0x1a;
				p_viewData++;
				count++;
				y += 0x10;
			}
			break;
		}
		}
	}
	return count;
}

// FUNCTION: LEMBALL 0x00429e40
void CLaser::DoActivate()
{
}
