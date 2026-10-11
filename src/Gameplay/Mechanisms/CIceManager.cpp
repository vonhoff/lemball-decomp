#include "CIceManager.h"

#include "CIce.h"
#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Mechanisms/SwitchEntry.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0042d7a0
CIceManager::CIceManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_ICE,
						 OBJECT_MANAGER_TRANSPORT_ICE)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_ice = NULL;
}

// FUNCTION: LEMBALL 0x0042d800
void CIceManager::Restart()
{
	if (m_ice != NULL) {
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
		m_ice = NULL;
		return;
	}
	if (m_ice == NULL) {
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

// FUNCTION: LEMBALL 0x0042d900
void CIceManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x0042d910
void CIceManager::RemoveIce(CIce* p_ice)
{
	for (int index = 0; index < m_count; index++) {
		if (&m_ice[index] == p_ice) {
			m_ice[index].SetId(INVALID_OBJECT_ID);
			for (int next = index + 1; next < m_count; next++) {
				m_ice[next - 1] = m_ice[next];
			}
			m_count--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0042dd00
void CIceManager::Process()
{
	for (int index = 0; index < m_count; index++) {
		m_ice[index].Process();
	}
}

// FUNCTION: LEMBALL 0x0042dd30
int CIceManager::GetViewData(CViewData* p_viewData)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0042dd40
bool CIceManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	for (int index = 0; index < m_count; ++index) {
		if (m_ice[index].StepOn(p_position, p_object)) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0042dd90
void CIceManager::Switch(swMessage p_message, int p_id)
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
		if (p_message == SW_ICE) {
			m_ice[index].Switch();
		}
	}
}

// FUNCTION: LEMBALL 0x0042ddf0
void CIceManager::Add(unsigned short p_id,
					  const tCoord3d& p_cornerA,
					  const tCoord3d& p_cornerB,
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
int CIceManager::ExportIceRecords(tCoord3d p_records[][2])
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
	unsigned char* data = p_data;
	unsigned short count = *(const unsigned short*) data;
	data += sizeof(unsigned short);
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
				id = *(const unsigned short*) data;
				data += sizeof(unsigned short);
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}

			tCoord3d cornerA;
			tCoord3d cornerB;
			cornerA.m_x = *(const unsigned short*) data;
			data += sizeof(unsigned short);
			cornerA.m_y = *(const unsigned short*) data;
			data += sizeof(unsigned short);
			cornerA.m_z = *(const unsigned short*) data;
			data += sizeof(unsigned short);
			cornerB.m_x = *(const unsigned short*) data;
			data += sizeof(unsigned short);
			cornerB.m_y = *(const unsigned short*) data;
			data += sizeof(unsigned short);
			cornerB.m_z = *(const unsigned short*) data;
			data += sizeof(unsigned short);
			int velocityX = (short) *(const unsigned short*) data;
			data += sizeof(unsigned short);
			int velocityY = (short) *(const unsigned short*) data;
			data += sizeof(unsigned short);
			unsigned int initialSwitched = 1;
			if (m_ai->m_levelVersion >= LEVEL_VERSION_WITH_INITIAL_ICE_SWITCH_STATE) {
				initialSwitched = *(const unsigned short*) data;
				data += sizeof(unsigned short);
			}
			Add(id, cornerA, cornerB, velocityX, velocityY, initialSwitched);
			remaining--;
		} while (remaining != 0);
	}
}
