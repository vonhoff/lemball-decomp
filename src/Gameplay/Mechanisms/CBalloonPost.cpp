#include "CBalloonPost.h"

#include "CTheBalloonPost.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"

enum {
	BALLOON_POST_BALLOON_1_ACTIVE_MASK = 0x01,
	BALLOON_POST_BALLOON_3_ACTIVE_MASK = 0x02,
	BALLOON_POST_BALLOON_5_ACTIVE_MASK = 0x04,
	BALLOON_POST_BALLOON_7_ACTIVE_MASK = 0x08
};

// FUNCTION: LEMBALL 0x00429f50
CBalloonPost::CBalloonPost(CAI* p_ai, CMap* p_map)
{
	m_ai = p_ai;
	m_map = p_map;
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
bool CBalloonPost::FindPost(eObjectType p_objectType, AICOORD& p_position)
{
	switch (p_objectType) {
	case OBJECT_BALLOON_1:
		p_position.m_xFixed = m_positions[0].m_xFixed;
		p_position.m_yFixed = m_positions[0].m_yFixed;
		p_position.m_zFixed = m_positions[0].m_zFixed;
		return m_activeMask & BALLOON_POST_BALLOON_1_ACTIVE_MASK;
	case OBJECT_BALLOON_3:
		p_position.m_xFixed = m_positions[1].m_xFixed;
		p_position.m_yFixed = m_positions[1].m_yFixed;
		p_position.m_zFixed = m_positions[1].m_zFixed;
		return m_activeMask & BALLOON_POST_BALLOON_3_ACTIVE_MASK;
	case OBJECT_BALLOON_5:
		p_position.m_xFixed = m_positions[2].m_xFixed;
		p_position.m_yFixed = m_positions[2].m_yFixed;
		p_position.m_zFixed = m_positions[2].m_zFixed;
		return m_activeMask & BALLOON_POST_BALLOON_5_ACTIVE_MASK;
	case OBJECT_BALLOON_7:
		p_position.m_xFixed = m_positions[3].m_xFixed;
		p_position.m_yFixed = m_positions[3].m_yFixed;
		p_position.m_zFixed = m_positions[3].m_zFixed;
		return m_activeMask & BALLOON_POST_BALLOON_7_ACTIVE_MASK;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x0042a170
void CBalloonPost::Process()
{
	if ((m_activeMask & BALLOON_POST_BALLOON_1_ACTIVE_MASK) != 0) {
		int width;
		int y = m_positions[0].m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int x = m_positions[0].m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		CMap* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < (width = map->m_ground.m_width) && blockY < map->m_ground.m_height) {
			x &= GROUND_BLOCK_PIXEL_MASK;
			y &= GROUND_BLOCK_PIXEL_MASK;
			z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x, y);
		}
		else {
			z = 0;
		}
		int height = z << FIXED_POINT_FRACTION_BITS;
		m_positions[0].m_zFixed = height;
		m_posts[0]->m_position.m_zFixed = height;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_3_ACTIVE_MASK) != 0) {
		int width;
		int y = m_positions[1].m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int x = m_positions[1].m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		CMap* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < (width = map->m_ground.m_width) && blockY < map->m_ground.m_height) {
			x &= GROUND_BLOCK_PIXEL_MASK;
			y &= GROUND_BLOCK_PIXEL_MASK;
			z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x, y);
		}
		else {
			z = 0;
		}
		int height = z << FIXED_POINT_FRACTION_BITS;
		m_positions[1].m_zFixed = height;
		m_posts[1]->m_position.m_zFixed = height;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_5_ACTIVE_MASK) != 0) {
		int width;
		int y = m_positions[2].m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int x = m_positions[2].m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		CMap* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < (width = map->m_ground.m_width) && blockY < map->m_ground.m_height) {
			x &= GROUND_BLOCK_PIXEL_MASK;
			y &= GROUND_BLOCK_PIXEL_MASK;
			z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x, y);
		}
		else {
			z = 0;
		}
		int height = z << FIXED_POINT_FRACTION_BITS;
		m_positions[2].m_zFixed = height;
		m_posts[2]->m_position.m_zFixed = height;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_7_ACTIVE_MASK) != 0) {
		int width;
		int y = m_positions[3].m_yFixed >> FIXED_POINT_FRACTION_BITS;
		int x = m_positions[3].m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		CMap* map = m_map;
		unsigned short z;
		if (x >= 0 && y >= 0 && blockX < (width = map->m_ground.m_width) && blockY < map->m_ground.m_height) {
			x &= GROUND_BLOCK_PIXEL_MASK;
			y &= GROUND_BLOCK_PIXEL_MASK;
			z = map->m_ground.m_ground[blockY * width + blockX].GetZ(x, y);
		}
		else {
			z = 0;
		}
		int height = z << FIXED_POINT_FRACTION_BITS;
		m_positions[3].m_zFixed = height;
		m_posts[3]->m_position.m_zFixed = height;
	}
}

// FUNCTION: LEMBALL 0x0042a320
int CBalloonPost::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	if ((m_activeMask & BALLOON_POST_BALLOON_1_ACTIVE_MASK) != 0) {
		m_posts[0]->GetViewData(p_viewData[count]);
		count++;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_3_ACTIVE_MASK) != 0) {
		m_posts[1]->GetViewData(p_viewData[count]);
		count++;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_5_ACTIVE_MASK) != 0) {
		m_posts[2]->GetViewData(p_viewData[count]);
		count++;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_7_ACTIVE_MASK) != 0) {
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
	case OBJECT_BALLOON_1:
		mask = BALLOON_POST_BALLOON_1_ACTIVE_MASK;
		index = 0;
		break;
	case OBJECT_BALLOON_3:
		mask = BALLOON_POST_BALLOON_3_ACTIVE_MASK;
		index = 1;
		break;
	case OBJECT_BALLOON_5:
		mask = BALLOON_POST_BALLOON_5_ACTIVE_MASK;
		index = 2;
		break;
	case OBJECT_BALLOON_7:
		mask = BALLOON_POST_BALLOON_7_ACTIVE_MASK;
		index = 3;
		break;
	default:
		return;
	}
	m_activeMask |= mask;
	m_positions[index].m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_positions[index].m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_positions[index].m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
	CTheBalloonPost* post = m_posts[index];
	post->m_position.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	post->m_position.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	post->m_position.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
	m_posts[index]->m_active = 1;
}

// FUNCTION: LEMBALL 0x0042a460
void CBalloonPost::DeactivatePost(CTheBalloonPost* p_post)
{
	int mask;
	int index;
	switch (p_post->m_objectType) {
	case OBJECT_BALLOON_1:
		mask = ~BALLOON_POST_BALLOON_1_ACTIVE_MASK;
		index = 0;
		break;
	case OBJECT_BALLOON_3:
		mask = ~BALLOON_POST_BALLOON_3_ACTIVE_MASK;
		index = 1;
		break;
	case OBJECT_BALLOON_5:
		mask = ~BALLOON_POST_BALLOON_5_ACTIVE_MASK;
		index = 2;
		break;
	case OBJECT_BALLOON_7:
		mask = ~BALLOON_POST_BALLOON_7_ACTIVE_MASK;
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

	struct SerializedPosition {
		unsigned short x;
		unsigned short y;
		unsigned short z;
	};

	int count = BALLOON_POST_COUNT;
	AICOORD* position = m_positions;
	CTheBalloonPost** post = m_posts;
	unsigned short z;
	unsigned short x;
	unsigned short y;
	do {
		const SerializedPosition* data = reinterpret_cast<const SerializedPosition*>(p_data);
		x = data->x;
		y = data->y;
		z = data->z;
		p_data += sizeof(*data);

		position->m_xFixed = x << FIXED_POINT_FRACTION_BITS;
		position->m_yFixed = y << FIXED_POINT_FRACTION_BITS;
		position->m_zFixed = z << FIXED_POINT_FRACTION_BITS;
		CTheBalloonPost* currentPost = *post;
		currentPost->m_position.m_xFixed = x << FIXED_POINT_FRACTION_BITS;
		currentPost->m_position.m_yFixed = y << FIXED_POINT_FRACTION_BITS;
		currentPost->m_position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
		(*post)->m_active = 0;

		position++;
		post++;
		count--;
	} while (count != 0);

	if ((m_activeMask & BALLOON_POST_BALLOON_1_ACTIVE_MASK) != 0) {
		m_posts[0]->m_active = 1;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_3_ACTIVE_MASK) != 0) {
		m_posts[1]->m_active = 1;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_5_ACTIVE_MASK) != 0) {
		m_posts[2]->m_active = 1;
	}
	if ((m_activeMask & BALLOON_POST_BALLOON_7_ACTIVE_MASK) != 0) {
		m_posts[3]->m_active = 1;
	}
}
