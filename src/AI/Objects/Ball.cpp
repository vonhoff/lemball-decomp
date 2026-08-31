#include "Ball.h"

#include "../../Map/Base/Map.h"
#include "../Managers/BallManager.h"
#include "../Navigation/Ai.h"

// 68K 0x1060364a __ct__5CBallFv
// STUB: LEMBALL 0x00421660
Ball::Ball()
{
}

// 68K 0x106036a8 Restart__5CBallFv
// FUNCTION: LEMBALL 0x00421690
void Ball::Restart()
{
	GameObject::Restart();
	m_action = (eAction) 0x25;
	m_speed = g_anTurnDelayCursor[m_objectType];
}

// 68K 0x106036f0 Set__5CBallF7AICOORD7AICOORDi
// STUB: LEMBALL 0x004216c0
void Ball::Set(AiCoord p_start, AiCoord p_destination, int p_speed)
{
}

// 68K 0x106037c0 StartMovement__5CBallFUc
// STUB: LEMBALL 0x00421770
void Ball::StartMovement(unsigned char p_direction)
{
}

// 68K 0x10603926 Move__5CBallFv
// STUB: LEMBALL 0x00421870
bool Ball::Move()
{
	return 0;
}

// 68K 0x10603a98 HitBullet__5CBallFP7CBullet
// FUNCTION: LEMBALL 0x00421aa0
void Ball::HitBullet(Bullet* p_bullet)
{
	Delete();
}

// 68K 0x10603ace Delete__5CBallFv
// STUB: LEMBALL 0x00421ab0
void Ball::Delete()
{
}

// 68K 0x10603b26 SetHeightCorrect__5CBallFv
// FUNCTION: LEMBALL 0x00421b40
void Ball::SetHeightCorrect()
{
	int x = m_position.m_xFixed >> 12;
	int y = m_position.m_yFixed >> 12;
	Map* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (unsigned int) z << 12;
}

// 68K 0x10603b92 Process__5CBallFv
// STUB: LEMBALL 0x00421bc0
bool Ball::Process()
{
	return 0;
}

// 68K 0x10603d56 LoadLevel__5CBallFRPUc
// STUB: LEMBALL 0x00421da0
void Ball::LoadLevel(unsigned char** p_data)
{
}

// 68K 0x101196dc __dt__5CBallFv
Ball::~Ball()
{
}
