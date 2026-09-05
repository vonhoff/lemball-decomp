#include "../../Map/Base/Map.h"
#include "../Base/GameObject.h"
#include "../Navigation/Ai.h"
#include "TrapDoorManager.h"

// 68K 0x106219d8 LoadLevel__16CTrapDoorManagerFPUciUc
// FUNCTION: LEMBALL 0x0040ca40
void TrapDoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	int count = *data++;
	int selections[4];
	for (int selection = 0; selection < 4; selection++) {
		selections[selection] = 0;
	}
	for (int i = 0; i < count; i++) {
		unsigned short id;
		if (*(unsigned int*) &p_skip == 0) {
			id = GameObject::NextLoadingId();
		}
		AiCoord position;
		position.m_xFixed = *data++ << 12;
		position.m_yFixed = *data++ << 12;
		data++;
		int x = position.m_xFixed >> 12;
		int y = position.m_yFixed >> 12;
		int blockX = x >> 4;
		int blockY = y >> 4;
		Map* map = g_pMap;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
		}
		else {
			z = 0;
		}
		position.m_zFixed = (unsigned int) z << 12;
		selections[i] = *data++;
		if (*(unsigned int*) &p_skip == 0) {
			AddNewDoor(id, position, 1, 0);
		}
		g_pAI->AddANetworkStart(position.m_xFixed >> 12, position.m_yFixed >> 12, position.m_zFixed >> 12, i);
	}
	g_pAI->SetNetworkTrapDoors(count, selections[0], selections[1], selections[2], selections[3]);
}
