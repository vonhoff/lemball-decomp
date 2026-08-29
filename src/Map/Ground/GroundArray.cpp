#include "GroundArray.h"
#include "Ground.h"

// 68K 0x10119a8e SetCollision__12CGroundArrayFiii
// STUB: LEMBALL 0x0040eb90
void GroundArray::SetCollision(int p_x, int p_y, int p_collision)
{
}

// 68K 0x10900402 Clear__12CGroundArrayFv
// FUNCTION: LEMBALL 0x00430370
void GroundArray::Clear()
{
	int y = 0;
	if (m_height > 0) {
		do {
			for (int x = 0; x < m_width; x++) {
				Ground* ground = m_ground + y * m_width + x;
				ground->m_objectType = (eObjectType) 0x209;
				ground->m_objectData = 0;
				ground->m_collision = 0;
				ground->m_height = 0;
				ground->m_cliff = 0;
			}
			y++;
		} while (y < m_height);
	}
}

// 68K 0x101184d4 GetZ__12CGroundArrayFii
// FUNCTION: LEMBALL 0x00431520
unsigned short GroundArray::GetZ(int p_x, int p_y)
{
	int blockX = p_x >> 4;
	int blockY = p_y >> 4;
	if (p_x >= 0 && p_y >= 0 && blockX < m_width && m_height > blockY) {
		return m_ground[blockY * m_width + blockX].GetZ(p_x & 0xf, p_y & 0xf);
	}
	return 0;
}
