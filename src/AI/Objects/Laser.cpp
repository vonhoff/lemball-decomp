#include "Laser.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../../Visos/Network/Connect.h"

// 68K 0x10613518 __ct__6CLaserFv
// FUNCTION: LEMBALL 0x00428890
Laser::Laser() : GlobalGameObject(0x2f, 0, 0)
{
}

// 68K 0x10613558 Restart__6CLaserFv
// FUNCTION: LEMBALL 0x004288b0
void Laser::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x1061358a Initialise__6CLaserFv
// FUNCTION: LEMBALL 0x004288d0
void Laser::Initialise()
{
	m_action = (eAction) 0x18;
	m_stateTimer = 0;
	m_active = 0;
	m_enabled = 0;
}

// 68K 0x10613618 Set__6CLaserFUsRC7AICOORD11eObjectType
// FUNCTION: LEMBALL 0x00428900
void Laser::Set(unsigned short p_id, const AiCoord& p_position, eObjectType p_orientation)
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
	case (eObjectType) 0x1e:
		m_action = (eAction) 0x18;
		m_autoActivate = 1;
		m_active = 1;
		break;
	case (eObjectType) 0x2f:
		m_action = (eAction) 0x18;
		m_autoActivate = 1;
		m_active = 1;
		break;
	case (eObjectType) 0x30: {
		m_autoActivate = 0;
		m_action = (eAction) 0x18;
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
	case (eObjectType) 0x31: {
		m_autoActivate = 0;
		m_action = (eAction) 0x18;
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

// 68K 0x1061378e CheckHits__6CLaserFv
// STUB: LEMBALL 0x00428ab0
bool Laser::CheckHits()
{
	return 0;
}

// 68K 0x106138ea Process__6CLaserFv
// STUB: LEMBALL 0x00428cf0
bool Laser::Process()
{
	return 0;
}

// 68K 0x10613ace Activate__6CLaserFv
// FUNCTION: LEMBALL 0x00428ec0
bool Laser::Activate()
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
	Action((eAction) 0x19);
	return 1;
}

// 68K 0x10613b58 StepOn__6CLaserFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x00428f30
bool Laser::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	if (p_object->m_objectType == (eObjectType) 2 && (int) Distance(m_position.m_xFixed >> 12,
																	m_position.m_yFixed >> 12,
																	p_position.m_xFixed >> 12,
																	p_position.m_yFixed >> 12) < 0x30) {
		Activate();
		m_target = p_object;
		return 1;
	}
	return 0;
}

// 68K 0x10613bec GetViewData__6CLaserFP9CViewData
// STUB: LEMBALL 0x00428f90
int Laser::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1011a726 DoActivate__6CLaserFv
// FUNCTION: LEMBALL 0x00429e40
void Laser::DoActivate()
{
}

// 68K 0x106135c0 __dt__6CLaserFv
Laser::~Laser()
{
}
