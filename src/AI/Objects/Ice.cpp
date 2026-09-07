#include "Ice.h"

#include "../../Control/Game/Game.h"
#include "../../Map/Base/Map.h"
#include "../Base/Coord3d.h"

// 68K 0x1061176c __ct__4CIceFv
// FUNCTION: LEMBALL 0x0042ca70
Ice::Ice() : GlobalGameObject(0x33, 0, 0)
{
}

// 68K 0x106117aa Restart__4CIceFv
// FUNCTION: LEMBALL 0x0042ca90
void Ice::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x106117da Initialise__4CIceFv
// FUNCTION: LEMBALL 0x0042cab0
void Ice::Initialise()
{
	m_stateTimer = 0;
	m_switched = 0;
	m_enabled = 0;
	m_objectCount = 0;
	m_action = (eAction) 0x18;
	m_lastMovementTick = g_dwGameTick;
}

// 68K 0x10611870 Set__4CIceFUsRC8tCoord3dRC8tCoord3diiUc
// FUNCTION: LEMBALL 0x0042cb00
void Ice::Set(unsigned short p_id,
			  const Coord3d& p_cornerA,
			  const Coord3d& p_cornerB,
			  int p_velocityX,
			  int p_velocityY,
			  unsigned char p_initialSwitched)
{
	SetId(p_id);
	m_enabled = 1;
	m_objectCount = 0;
	m_lastMovementTick = g_dwGameTick;
	m_velocityX = p_velocityX;
	m_velocityY = p_velocityY;
	m_initialSwitched = p_initialSwitched;
	m_switched = p_initialSwitched;

	int minX = p_cornerA.m_x;
	int maxX = p_cornerB.m_x;
	int minY = p_cornerA.m_y;
	int maxY = p_cornerB.m_y;
	if (maxX < minX) {
		int temporary = minX;
		minX = maxX;
		maxX = temporary;
	}
	if (maxY < minY) {
		int temporary = minY;
		minY = maxY;
		maxY = temporary;
	}
	m_minX = (short) minX;
	m_minY = (short) minY;
	m_maxX = (short) maxX;
	m_maxY = (short) maxY;

	m_minZ = 0;
	if (m_minX >= 0 && m_minY >= 0 && (m_minX >> 4) < g_pMap->m_ground.m_width &&
		g_pMap->m_ground.m_height > (m_minY >> 4)) {
		m_minZ = (short) g_pMap->m_ground.m_ground[(m_minY >> 4) * g_pMap->m_ground.m_width + (m_minX >> 4)].GetZ(
			m_minX & 0xf,
			m_minY & 0xf);
	}

	m_maxZ = 0;
	if (m_maxX >= 0 && m_maxY >= 0 && (m_maxX >> 4) < g_pMap->m_ground.m_width &&
		g_pMap->m_ground.m_height > (m_maxY >> 4)) {
		m_maxZ = (short) g_pMap->m_ground.m_ground[(m_maxY >> 4) * g_pMap->m_ground.m_width + (m_maxX >> 4)].GetZ(
			m_maxX & 0xf,
			m_maxY & 0xf);
	}

	m_position.m_xFixed = ((int) p_cornerA.m_x) << 12;
	m_position.m_yFixed = ((int) p_cornerA.m_y) << 12;
	m_position.m_zFixed = ((int) p_cornerA.m_z) << 12;
	for (int y = minY; y <= maxY; y += 0x10) {
		for (int x = minX; x <= maxX; x += 0x10) {
			int blockX = (x + ((x >> 31) & 0xf)) >> 4;
			if (blockX >= 0) {
				int blockY = (y + ((y >> 31) & 0xf)) >> 4;
				if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && blockY < g_pMap->m_ground.m_height) {
					Ground* ground = g_pMap->m_ground.m_ground + g_pMap->m_ground.m_width * blockY + blockX;
					ground->m_collision |= 0x8000;
				}
			}
		}
	}

	if (m_velocityX == 0 && m_velocityY == 0) {
		m_velocityX = 1;
		m_velocityY = 1;
	}
}

// 68K 0x10611a44 Process__4CIceFv
// STUB: LEMBALL 0x0042cd70
bool Ice::Process()
{
	return 0;
}

// 68K 0x1061200c StepOn__4CIceFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0042d380
bool Ice::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x1061217a Leave__4CIceFP14CPlayerLemming
// STUB: LEMBALL 0x0042d4d0
void Ice::Leave(PlayerLemming* p_lemming)
{
}

// 68K 0x1061220e Switch__4CIceFv
// FUNCTION: LEMBALL 0x0042d550
void Ice::Switch()
{
	RequestAction(0x1a);
}

// 68K 0x10612236 Switched__4CIceFv
// STUB: LEMBALL 0x0042d560
void Ice::Switched()
{
}

// 68K 0x1061181a __dt__4CIceFv
Ice::~Ice()
{
}
