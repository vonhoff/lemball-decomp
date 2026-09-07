#include "Mover.h"
#include "MoverManager.h"

// 68K 0x10618140 Find__13CMoverManagerFiiRi
// FUNCTION: LEMBALL 0x0042f2f0
Mover* MoverManager::Find(int p_x, int p_y, int& p_height)
{
	for (int i = 0; i < m_count; i++) {
		if (m_movers[i].IsAt(p_x, p_y, p_height)) {
			return &m_movers[i];
		}
	}
	return 0;
}
