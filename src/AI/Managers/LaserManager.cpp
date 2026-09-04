#include "LaserManager.h"

#include "../Objects/Laser.h"

// 68K 0x1061403a __ct__13CLaserManagerFP3CAIi
// FUNCTION: LEMBALL 0x00429320
LaserManager::LaserManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x15, 0xa)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_lasers = 0;
}

// 68K 0x106140c6 Restart__13CLaserManagerFv
// FUNCTION: LEMBALL 0x00429380
void LaserManager::Restart()
{
	if (m_lasers != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_lasers[i].Restart();
		}
	}
}

// 68K 0x1061412a Initialise__13CLaserManagerFi
// FUNCTION: LEMBALL 0x004293b0
void LaserManager::Initialise(int p_capacity)
{
	m_count = 0;
	if (p_capacity == 0) {
		m_lasers = 0;
		return;
	}
	if (m_lasers == 0) {
		m_lasers = new Laser[m_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_lasers[i].m_manager = this;
			m_lasers[i].Restart();
		}
	}
}

// 68K 0x10614266 StepOn__13CLaserManagerFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x004297f0
bool LaserManager::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x10614332 Process__13CLaserManagerFv
// STUB: LEMBALL 0x00429850
void LaserManager::Process()
{
}

// 68K 0x106143d2 GetViewData__13CLaserManagerFP9CViewData
// STUB: LEMBALL 0x004298a0
int LaserManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1061444c Add__13CLaserManagerFUsiii11eObjectType
// STUB: LEMBALL 0x004298f0
void LaserManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_orientation)
{
}

#include "../Navigation/Ai.h"

// 68K 0x10614500 LoadLevel__13CLaserManagerFPUciUc
// FUNCTION: LEMBALL 0x00429950
void LaserManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data++;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		unsigned short id;
		eObjectType orientation;
		unsigned short x;
		unsigned short y;
		unsigned short z;
		do {
			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) GameObject::NextId();
			}
			orientation = (eObjectType) *data++;
			x = *data++;
			y = *data++;
			z = *data++;
			Add(id, x, y, z, orientation);
			remaining--;
		} while (remaining != 0);
	}
}

// 68K 0x106141f0 __dt__13CLaserManagerFv
LaserManager::~LaserManager()
{
}
