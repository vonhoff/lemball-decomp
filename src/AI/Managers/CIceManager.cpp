#include "CIceManager.h"

#include "../Base/Coord3d.h"
#include "../Navigation/CAi.h"
#include "../Objects/CIce.h"
#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0042d7a0
CIceManager::CIceManager(CAi* p_arg0, int p_arg1) : CBaseObjectManager(0x19, 0xe)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_ice = 0;
}

// FUNCTION: LEMBALL 0x0042d800
void CIceManager::Restart()
{
	if (m_ice != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_ice[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042d830
void CIceManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_ice = 0;
		return;
	}
	if (m_ice == 0) {
		m_ice = new CIce[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_ice[i].Restart();
			m_ice[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x0042d8e0
CIceManager::~CIceManager()
{
	delete[] m_ice;
}

// FUNCTION: LEMBALL 0x0042dd00
void CIceManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_ice[i].Process();
	}
}

// FUNCTION: LEMBALL 0x0042dd30
int CIceManager::GetViewData(CViewData* p_viewData)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0042dd40
bool CIceManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	for (int index = 0; index < m_count; ++index) {
		if (m_ice[index].StepOn(p_position, p_object)) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042dd90
void CIceManager::Switch(int p_message, int p_id)
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

// FUNCTION: LEMBALL 0x0042ddf0
void CIceManager::Add(unsigned short p_id,
					  const Coord3d& p_cornerA,
					  const Coord3d& p_cornerB,
					  int p_velocityX,
					  int p_velocityY,
					  unsigned int p_initialSwitched)
{
	if (m_count < m_capacity) {
		m_ice[m_count].Set(p_id, p_cornerA, p_cornerB, p_velocityX, p_velocityY, p_initialSwitched);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0042de40
int CIceManager::ExportIceRecords(Coord3d p_records[][2])
{
	for (int i = 0; i < m_count; i++) {
		CIce* ice = &m_ice[i];
		(*p_records)[0] = ice->m_min;
		(*p_records)[1] = ice->m_max;
		p_records++;
	}
	return m_count;
}

// FUNCTION: LEMBALL 0x0042dea0
void CIceManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data++;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}

			Coord3d cornerA;
			Coord3d cornerB;
			cornerA.m_x = (short) *data++;
			cornerA.m_y = (short) *data++;
			cornerA.m_z = (short) *data++;
			cornerB.m_x = (short) *data++;
			cornerB.m_y = (short) *data++;
			cornerB.m_z = (short) *data++;
			int velocityX = (short) *data++;
			int velocityY = (short) *data++;
			unsigned int initialSwitched = 1;
			if (m_ai->m_levelVersion >= 10) {
				initialSwitched = *data++;
			}
			Add(id, cornerA, cornerB, velocityX, velocityY, initialSwitched);
			remaining--;
		} while (remaining != 0);
	}
}
