#include "BalloonPost.h"

#include "../../Map/Base/Map.h"
#include "ViewData.h"

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
// FUNCTION: LEMBALL 0x0042a0b0
bool BalloonPost::FindPost(eObjectType p_objectType, AiCoord& p_position)
{
	switch (p_objectType) {
	case 0x28:
		p_position.m_xFixed = m_positions[0].m_xFixed;
		p_position.m_yFixed = m_positions[0].m_yFixed;
		p_position.m_zFixed = m_positions[0].m_zFixed;
		return m_activeMask & 1;
	case 0x2a:
		p_position.m_xFixed = m_positions[1].m_xFixed;
		p_position.m_yFixed = m_positions[1].m_yFixed;
		p_position.m_zFixed = m_positions[1].m_zFixed;
		return m_activeMask & 2;
	case 0x2c:
		p_position.m_xFixed = m_positions[2].m_xFixed;
		p_position.m_yFixed = m_positions[2].m_yFixed;
		p_position.m_zFixed = m_positions[2].m_zFixed;
		return m_activeMask & 4;
	case 0x2e:
		p_position.m_xFixed = m_positions[3].m_xFixed;
		p_position.m_yFixed = m_positions[3].m_yFixed;
		p_position.m_zFixed = m_positions[3].m_zFixed;
		return m_activeMask & 8;
	default:
		return false;
	}
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
// FUNCTION: LEMBALL 0x0042a320
int BalloonPost::GetViewData(ViewData* p_viewData)
{
	int count = 0;
	if ((m_activeMask & 1) != 0) {
		m_posts[0]->GetViewData(p_viewData[count]);
		count++;
	}
	if ((m_activeMask & 2) != 0) {
		m_posts[1]->GetViewData(p_viewData[count]);
		count++;
	}
	if ((m_activeMask & 4) != 0) {
		m_posts[2]->GetViewData(p_viewData[count]);
		count++;
	}
	if ((m_activeMask & 8) != 0) {
		m_posts[3]->GetViewData(p_viewData[count]);
		count++;
	}
	return count;
}

// 68K 0x10603486 LoadLevel__12CBalloonPostFPUciUc
// FUNCTION: LEMBALL 0x0042a4e0
void BalloonPost::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	m_activeMask = *reinterpret_cast<unsigned short*>(p_data);
	p_data += 2;

	int count = 4;
	AiCoord* position = m_positions;
	TheBalloonPost** post = m_posts;
	unsigned short z;
	unsigned short x;
	unsigned short y;
	do {
		x = *reinterpret_cast<unsigned short*>(p_data);
		p_data += 2;
		y = *reinterpret_cast<unsigned short*>(p_data);
		p_data += 2;
		z = *reinterpret_cast<unsigned short*>(p_data);
		p_data += 2;

		position->m_xFixed = (unsigned int) x << 12;
		position->m_yFixed = (unsigned int) y << 12;
		position->m_zFixed = (unsigned int) z << 12;
		TheBalloonPost* currentPost = *post;
		currentPost->m_position.m_xFixed = (unsigned int) x << 12;
		currentPost->m_position.m_yFixed = (unsigned int) y << 12;
		currentPost->m_position.m_zFixed = (unsigned int) z << 12;
		(*post)->m_active = 0;

		position++;
		post++;
		count--;
	} while (count != 0);

	if ((m_activeMask & 1) != 0) {
		m_posts[0]->m_active = 1;
	}
	if ((m_activeMask & 2) != 0) {
		m_posts[1]->m_active = 1;
	}
	if ((m_activeMask & 4) != 0) {
		m_posts[2]->m_active = 1;
	}
	if ((m_activeMask & 8) != 0) {
		m_posts[3]->m_active = 1;
	}
}
