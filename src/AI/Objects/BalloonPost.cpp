#include "BalloonPost.h"

#include "../../Map/Base/Map.h"

// 68K 0x10602f0e __ct__12CBalloonPostFP3CAI
// STUB: LEMBALL 0x00429f50
BalloonPost::BalloonPost(Ai* p_arg0, Map* p_arg1)
{
}

#include "TheBalloonPost.h"

// 68K 0x10603000 Restart__12CBalloonPostFv
// FUNCTION: LEMBALL 0x0042a030
void BalloonPost::Restart()
{
	m_activeMask = 0;
	m_posts[0]->Restart();
	m_posts[1]->Restart();
	m_posts[2]->Restart();
	m_posts[3]->Restart();
}

// 68K 0x10603072 __dt__12CBalloonPostFv
// FUNCTION: LEMBALL 0x0042a070
BalloonPost::~BalloonPost()
{
	delete m_posts[0];
	delete m_posts[1];
	delete m_posts[2];
	delete m_posts[3];
}

// 68K 0x1060312c FindPost__12CBalloonPostF11eObjectTypeR7AICOORD
// STUB: LEMBALL 0x0042a0b0
bool BalloonPost::FindPost(eObjectType p_objectType, AiCoord& p_position)
{
	return 0;
}

// 68K 0x106031fe Process__12CBalloonPostFv
// FUNCTION: LEMBALL 0x0042a170
void BalloonPost::Process()
{
	if ((m_activeMask & 1) != 0) {
		int y = m_positions[0].m_yFixed >> 12;
		int x = m_positions[0].m_xFixed >> 12;
		int blockX = x >> 4;
		int blockY = y >> 4;
		Map* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
			z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
		}
		else {
			z = 0;
		}
		m_positions[0].m_zFixed = z << 12;
		m_posts[0]->m_position.m_zFixed = z << 12;
	}
	if ((m_activeMask & 2) != 0) {
		int y = m_positions[1].m_yFixed >> 12;
		int x = m_positions[1].m_xFixed >> 12;
		int blockX = x >> 4;
		int blockY = y >> 4;
		Map* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
			z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
		}
		else {
			z = 0;
		}
		m_positions[1].m_zFixed = z << 12;
		m_posts[1]->m_position.m_zFixed = z << 12;
	}
	if ((m_activeMask & 4) != 0) {
		int y = m_positions[2].m_yFixed >> 12;
		int x = m_positions[2].m_xFixed >> 12;
		int blockX = x >> 4;
		int blockY = y >> 4;
		Map* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
			z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
		}
		else {
			z = 0;
		}
		m_positions[2].m_zFixed = z << 12;
		m_posts[2]->m_position.m_zFixed = z << 12;
	}
	if ((m_activeMask & 8) != 0) {
		int y = m_positions[3].m_yFixed >> 12;
		int x = m_positions[3].m_xFixed >> 12;
		int blockX = x >> 4;
		int blockY = y >> 4;
		Map* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
			z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
		}
		else {
			z = 0;
		}
		m_positions[3].m_zFixed = z << 12;
		m_posts[3]->m_position.m_zFixed = z << 12;
	}
}

// 68K 0x10603386 GetViewData__12CBalloonPostFP9CViewData
// STUB: LEMBALL 0x0042a320
int BalloonPost::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10603486 LoadLevel__12CBalloonPostFPUciUc
// STUB: LEMBALL 0x0042a4e0
unsigned short BalloonPost::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	return 0;
}
