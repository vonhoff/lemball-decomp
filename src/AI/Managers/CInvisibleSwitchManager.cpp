#include "CInvisibleSwitchManager.h"

#include "../Objects/CInvisibleSwitch.h"
#include "AI/Base/tCoord3d.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0040a210
CInvisibleSwitchManager::CInvisibleSwitchManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_INVISIBLE_SWITCHES,
						 OBJECT_MANAGER_TRANSPORT_INVISIBLE_SWITCHES)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_switches = NULL;
}

// FUNCTION: LEMBALL 0x0040a270
void CInvisibleSwitchManager::Restart()
{
	CInvisibleSwitchManager* owner = this;
	if (owner->m_switches != NULL) {
		for (int i = 0; i < owner->m_capacity; i++) {
			owner->m_switches[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040a2a0
void CInvisibleSwitchManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_switches = NULL;
		return;
	}
	if (m_switches == NULL) {
		m_switches = new CInvisibleSwitch[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			CInvisibleSwitch& entry = m_switches[i];
			entry.Restart();
			m_switches[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x0040a350
CInvisibleSwitchManager::~CInvisibleSwitchManager()
{
	delete[] m_switches;
}

// FUNCTION: LEMBALL 0x0040a370
void CInvisibleSwitchManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		m_switches[i].StepOn(p_position, p_object);
	}
}

// FUNCTION: LEMBALL 0x0040a3b0
void CInvisibleSwitchManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_switches[i].Process();
	}
}

// FUNCTION: LEMBALL 0x0040a3e0
void CInvisibleSwitchManager::AddSwitch(unsigned short p_id, const tCoord3d& p_min, const tCoord3d& p_max)
{
	if (m_count < m_capacity) {
		m_switches[m_count].SetId(p_id);
		m_switches[m_count].Set(p_min, p_max);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0040a440
void CInvisibleSwitchManager::AddPointSwitch(unsigned short p_id, short p_x, short p_y, short p_z)
{
	tCoord3d minimum;
	minimum.m_x = p_x;
	minimum.m_y = p_y;
	minimum.m_z = p_z;
	tCoord3d maximum;
	maximum.m_x = p_x;
	maximum.m_y = p_y;
	maximum.m_z = p_z;
	AddSwitch(p_id, minimum, maximum);
}

// FUNCTION: LEMBALL 0x0040a490
void CInvisibleSwitchManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count;
	int i;

	count = *(unsigned short*) p_data;
	p_data = p_data + 2;
	Initialise(count);
	m_count = count;
	i = 0;
	while (i < m_count) {
		m_switches[i].Load(p_data);
		i = i + 1;
	}
}
