#include "CGroundArray.h"

#include "AI/Base/ObjectTypes.h"
#include "CGround.h"

// FUNCTION: LEMBALL 0x0040eb70
CGround* CGroundArray::GetGroundCell(int p_x, int p_y)
{
	return &m_ground[m_width * p_y + p_x];
}

// FUNCTION: LEMBALL 0x0040eb90
void CGroundArray::SetCollision(int p_x, int p_y, int p_collision)
{
	if (p_x >= 0 && p_y >= 0 && p_x < m_width && p_y < m_height) {
		m_ground[p_y * m_width + p_x].m_collision |= (unsigned short) p_collision;
	}
}

// FUNCTION: LEMBALL 0x00430370
void CGroundArray::Clear()
{
	CGround* ground;
	int x;
	int y = 0;
	if (m_height > 0) {
		do {
			x = 0;
			for (;;) {
				if (x >= m_width) {
					break;
				}
				ground = m_ground + y * m_width + x;
				ground->m_objectType = TERRAIN_BLOX_4;
				ground->m_objectData = 0;
				ground->m_collision = 0;
				ground->m_height = 0;
				ground->m_cliff = 0;
				x++;
			}
			y++;
		} while (y < m_height);
	}
}

// FUNCTION: LEMBALL 0x00431520
unsigned short CGroundArray::GetZ(int p_x, int p_y)
{
	int blockX = p_x >> 4;
	int blockY = p_y >> 4;
	if (p_x >= 0 && p_y >= 0 && blockX < m_width && m_height > blockY) {
		return m_ground[blockY * m_width + blockX].GetZ(p_x & 0xf, p_y & 0xf);
	}
	return 0;
}
