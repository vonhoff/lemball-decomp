#include "CBalloonPost.h"

#include "../../Map/Base/CMap.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/ObjectTypes.h"
#include "CTheBalloonPost.h"
#include "CViewData.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x00429f50
CBalloonPost::CBalloonPost(CAi* p_arg0, CMap* p_arg1)
{
	m_ai = p_arg0;
	m_map = p_arg1;
	m_posts[0] = new CTheBalloonPost(OBJECT_BALLOON_1, 0);
	m_posts[1] = new CTheBalloonPost(OBJECT_BALLOON_3, 0);
	m_posts[2] = new CTheBalloonPost(OBJECT_BALLOON_5, 0);
	m_posts[3] = new CTheBalloonPost(OBJECT_BALLOON_7, 0);
}

// FUNCTION: LEMBALL 0x0042a030
void CBalloonPost::Restart()
{
	m_activeMask = 0;
	m_posts[0]->Restart();
	m_posts[1]->Restart();
	m_posts[2]->Restart();
	m_posts[3]->Restart();
}

// FUNCTION: LEMBALL 0x0042a070
CBalloonPost::~CBalloonPost()
{
	delete m_posts[0];
	delete m_posts[1];
	delete m_posts[2];
	delete m_posts[3];
}

// FUNCTION: LEMBALL 0x0042a0b0
bool CBalloonPost::FindPost(eObjectType p_objectType, AiCoord& p_position)
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

// FUNCTION: LEMBALL 0x0042a170
void CBalloonPost::Process()
{
	if ((m_activeMask & 1) != 0) {
		int y = m_positions[0].m_yFixed >> 12;
		int x = m_positions[0].m_xFixed >> 12;
		int blockX = x >> 4;
		int blockY = y >> 4;
		CMap* map = m_map;
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
		CMap* map = m_map;
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
		CMap* map = m_map;
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
		CMap* map = m_map;
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

// FUNCTION: LEMBALL 0x0042a320
int CBalloonPost::GetViewData(CViewData* p_viewData)
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

// FUNCTION: LEMBALL 0x0042a3a0
void CBalloonPost::ActivatePostAtPosition(int p_x, int p_y, int p_z, eObjectType p_type)
{
	int mask;
	int index;
	switch (p_type) {
	case 0x28:
		mask = 1;
		index = 0;
		break;
	case 0x2a:
		mask = 2;
		index = 1;
		break;
	case 0x2c:
		mask = 4;
		index = 2;
		break;
	case 0x2e:
		mask = 8;
		index = 3;
		break;
	default:
		return;
	}
	m_activeMask |= mask;
	m_positions[index].m_xFixed = p_x << 12;
	m_positions[index].m_yFixed = p_y << 12;
	m_positions[index].m_zFixed = p_z << 12;
	CTheBalloonPost* post = m_posts[index];
	post->m_position.m_xFixed = p_x << 12;
	post->m_position.m_yFixed = p_y << 12;
	post->m_position.m_zFixed = p_z << 12;
	m_posts[index]->m_active = 1;
}

// FUNCTION: LEMBALL 0x0042a460
void CBalloonPost::DeactivatePost(CTheBalloonPost* p_post)
{
	int mask;
	int index;
	switch (p_post->m_objectType) {
	case 0x28:
		mask = ~1;
		index = 0;
		break;
	case 0x2a:
		mask = ~2;
		index = 1;
		break;
	case 0x2c:
		mask = ~4;
		index = 2;
		break;
	case 0x2e:
		mask = ~8;
		index = 3;
		break;
	default:
		return;
	}
	m_activeMask &= mask;
	m_posts[index]->m_active = 0;
}

// FUNCTION: LEMBALL 0x0042a4e0
void CBalloonPost::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	m_activeMask = *reinterpret_cast<unsigned short*>(p_data);
	p_data += 2;

	int count = 4;
	AiCoord* position = m_positions;
	CTheBalloonPost** post = m_posts;
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
		CTheBalloonPost* currentPost = *post;
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
