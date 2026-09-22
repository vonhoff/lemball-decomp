#include "CInvisibleSwitchManager.h"

#include "../Objects/CInvisibleSwitch.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0040a210
CInvisibleSwitchManager::CInvisibleSwitchManager(CAi* p_arg0, int p_arg1) : CBaseObjectManager(0x20, 0x15)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_switches = 0;
}

// FUNCTION: LEMBALL 0x0040a270
void CInvisibleSwitchManager::Restart()
{
	if (m_switches != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_switches[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040a2a0
void CInvisibleSwitchManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_switches = 0;
		return;
	}
	if (m_switches == 0) {
		m_switches = new CInvisibleSwitch[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_switches[i].Restart();
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
void CInvisibleSwitchManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
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
void CInvisibleSwitchManager::AddSwitch(unsigned short p_id, const Coord3d& p_min, const Coord3d& p_max)
{
	if (m_count < m_capacity) {
		m_switches[m_count].SetId(p_id);
		m_switches[m_count].Set(p_min, p_max);
		m_count++;
	}
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
