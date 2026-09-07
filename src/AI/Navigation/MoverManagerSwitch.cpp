#include "Mover.h"
#include "MoverManager.h"

// 68K 0x10618322 Switch__13CMoverManagerF9swMessagei
// FUNCTION: LEMBALL 0x0042f620
void MoverManager::Switch(int p_message, int p_id)
{
	int index = 0;
	while (index < m_count) {
		if ((unsigned short) m_movers[index].GetId() == p_id) {
			if (p_message == 4) {
				m_movers[index].Switch();
			}
			return;
		}
		index++;
	}
}
