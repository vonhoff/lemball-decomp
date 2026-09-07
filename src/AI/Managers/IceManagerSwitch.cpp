#include "../Objects/Ice.h"
#include "IceManager.h"

// 68K 0x1061278a Switch__11CIceManagerF9swMessagei
// FUNCTION: LEMBALL 0x0042dd90
void IceManager::Switch(int p_message, int p_id)
{
	int index = 0;
	if (0 < m_count) {
		do {
			if ((unsigned short) m_ice[index].GetId() == p_id) {
				break;
			}
			index++;
			if (m_count <= index) {
				return;
			}
		} while (1);
		if (p_message == 5) {
			m_ice[index].Switch();
		}
	}
}
