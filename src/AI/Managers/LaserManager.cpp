#include "LaserManager.h"

#include "../Objects/Laser.h"

// 68K 0x1061403a __ct__13CLaserManagerFP3CAIi
// STUB: LEMBALL 0x00429320
LaserManager::LaserManager(Ai* p_arg0, int p_arg1)
{
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
// STUB: LEMBALL 0x004293b0
void LaserManager::Initialise(int p_capacity)
{
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

// 68K 0x10614500 LoadLevel__13CLaserManagerFPUciUc
// STUB: LEMBALL 0x00429950
void LaserManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
}

// 68K 0x106141f0 __dt__13CLaserManagerFv
LaserManager::~LaserManager()
{
}
