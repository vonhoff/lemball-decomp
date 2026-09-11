#include "Flag.h"

#include "../Navigation/Ai.h"

// 68K 0x10605e54 SetSFX__5CFlagFv
// FUNCTION: LEMBALL 0x00422b30
void Flag::SetSfx()
{
	SetSndEffect(2);
}

#include "../../Map/Base/Map.h"

// 68K 0x10605e84 Process__5CFlagFv
// FUNCTION: LEMBALL 0x00422b40
bool Flag::Process()
{
	if (m_isRemoteObject == 0 && m_objectType == (eObjectType) 0xb) {
		if (m_action == (eAction) 0x18 && m_unk0x11c == 0) {
			int x = m_position.m_xFixed >> 12;
			int y = m_position.m_yFixed >> 12;
			unsigned short z;
			int blockX = x >> 4;
			int blockY = y >> 4;
			Map* map = g_pMap;

			if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				int cellX = x & 0xf;
				int cellY = y & 0xf;
				z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
			}
			else {
				z = 0;
			}
			m_position.m_zFixed = (int) z << 12;
		}
		return 1;
	}
	return Collectable::Process();
}

// 68K 0x10605f2e Collected__5CFlagFv
// FUNCTION: LEMBALL 0x00422c00
int Flag::Collected()
{
	m_activator->AddObject(m_objectType, this);
	g_pAI->Score(2500);
	g_pAI->m_flagCounts[0]--;
	return 1;
}

// 68K 0x10119982 __dt__5CFlagFv
Flag::~Flag()
{
}
