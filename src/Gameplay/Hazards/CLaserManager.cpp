#include "CLaserManager.h"

#include "CLaser.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00429320
CLaserManager::CLaserManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_LASERS,
						 OBJECT_MANAGER_TRANSPORT_LASERS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_lasers = NULL;
}

// FUNCTION: LEMBALL 0x00429380
void CLaserManager::Restart()
{
	if (m_lasers != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_lasers[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x004293b0
void CLaserManager::Initialise(int p_capacity)
{
	m_count = 0;
	if (p_capacity == 0) {
		m_lasers = NULL;
		return;
	}
	if (m_lasers == NULL) {
		m_lasers = new CLaser[m_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_lasers[i].m_manager = this;
			m_lasers[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x00429450
CLaserManager::~CLaserManager()
{
	delete[] m_lasers;
}

// FUNCTION: LEMBALL 0x00429470
void CLaserManager::RemoveLaser(CLaser* p_laser)
{
	int count = m_count;
	for (int index = 0; index < count; index++) {
		if (p_laser == &m_lasers[index]) {
			m_lasers[index++].SetId(INVALID_OBJECT_ID);
			for (int next = index; next < m_count; next++) {
				m_lasers[next - 1] = m_lasers[next];
			}
			m_count--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x004297e0
void CLaserManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x004297f0
bool CLaserManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	int i = 0;
	for (;;) {
		if (i >= m_count) {
			return false;
		}
		CLaser& laser = m_lasers[i];
		if (laser.m_enabled != 0 && laser.m_autoActivate == 0 && laser.m_action == ACTION_READY &&
			laser.StepOn(p_position, p_object)) {
			return true;
		}
		i++;
	}
}

// FUNCTION: LEMBALL 0x00429850
void CLaserManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_lasers[i].m_stateProcessed = true;
		if (m_lasers[i].m_active != 0 || m_lasers[i].m_isRemoteObject != 0) {
			m_lasers[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x004298a0
int CLaserManager::GetViewData(CViewData* p_viewData)
{
	CViewData* output;
	int count = 0;
	int i = 0;
	if (0 < m_count) {
		output = p_viewData;
		do {
			int laserCount = m_lasers[i].GetViewData(output);
			count += laserCount;
			output += laserCount;
			i++;
		} while (i < m_count);
	}
	return count;
}

// FUNCTION: LEMBALL 0x004298f0
void CLaserManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_orientation)
{
	if (m_count < m_capacity) {
		AICOORD position(p_x << FIXED_POINT_FRACTION_BITS,
						 p_y << FIXED_POINT_FRACTION_BITS,
						 p_z << FIXED_POINT_FRACTION_BITS);
		m_lasers[m_count].Set(p_id, position, p_orientation);
		m_count++;
	}
}

#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Simulation/CAI.h"

// FUNCTION: LEMBALL 0x00429950
void CLaserManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
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
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
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
