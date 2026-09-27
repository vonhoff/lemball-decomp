#include "../CPaintGunManager.h"

#include "AI/Base/CGameObject.h"
#include "AI/Navigation/CAI.h"

// FUNCTION: LEMBALL 0x0042c610
void CPaintGunManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		unsigned short id;
		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short direction;
		do {
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			x = *(unsigned short*) p_data;
			p_data += 2;
			y = *(unsigned short*) p_data;
			p_data += 2;
			z = *(unsigned short*) p_data;
			p_data += 2;
			direction = *(unsigned short*) p_data;
			p_data += 2;
			Add(id, x, y, z, direction);
			remaining--;
		} while (remaining != 0);
	}
}
