#include "AI/Managers/CLiftManager.h"

#include "AI/Base/CGameObject.h"
#include "AI/Base/Coord3d.h"
#include "AI/Navigation/CAI.h"
#include "AI/Objects/CLift.h"

// FUNCTION: LEMBALL 0x00425fc0
void CLiftManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	Coord3d start;
	Coord3d end;
	Coord3d position;
	unsigned short* data;
	unsigned short count;

	data = (unsigned short*) p_data;
	count = *data++;
	Initialise(count);
	if (m_ai->m_levelVersion >= 3 && count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short id;
			unsigned int initialActive;
			eLiftActivateType activateType;
			int lowHeight;
			int highHeight;
			short direction;

			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}

			if (m_ai->m_levelVersion >= 5) {

				initialActive = *data++;
				activateType = (eLiftActivateType) *data++;
				start.m_x = (short) *data++;
				start.m_y = (short) *data++;
				start.m_z = (short) *data++;
				end.m_x = (short) *data++;
				end.m_y = (short) *data++;
				end.m_z = (short) *data++;
				lowHeight = (short) *data++;
				highHeight = (short) *data++;
				direction = (short) *data++;
				m_lifts[m_count].SetId(id);
				m_lifts[m_count].Set(start, end, direction, lowHeight, highHeight, activateType, initialActive);
			}
			else {

				initialActive = *data++;
				activateType = (eLiftActivateType) *data++;
				position.m_x = (short) *data++;
				position.m_y = (short) *data++;
				position.m_z = (short) *data++;
				lowHeight = (short) *data++;
				highHeight = (short) *data++;
				direction = (short) *data++;
				m_lifts[m_count].SetId(id);
				m_lifts[m_count].Set(position.m_x,
									 position.m_y,
									 position.m_z,
									 direction,
									 lowHeight,
									 highHeight,
									 activateType,
									 initialActive);
			}

			m_count++;
			remaining--;
		} while (remaining != 0);
	}
}
