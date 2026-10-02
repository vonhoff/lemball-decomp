#include "../CGroundAnim.h"

#include "AI/Base/tCoord3d.h"

// FUNCTION: LEMBALL 0x0040d2e0
void CGroundAnim::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;

	if (count != 0) {
		unsigned int remaining = count;
		do {
			tCoord3d coordinate;
			coordinate.m_x = *(unsigned short*) p_data;
			p_data += 2;
			coordinate.m_y = *(unsigned short*) p_data;
			p_data += 2;
			coordinate.m_z = *(unsigned short*) p_data;
			p_data += 2;
			unsigned int startFrame = *(unsigned short*) p_data;
			p_data += 2;
			unsigned int endFrame = *(unsigned short*) p_data;
			p_data += 2;
			Add(coordinate, startFrame, endFrame);
			remaining--;
		} while (remaining != 0);
	}
}
