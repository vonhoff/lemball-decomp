#include "InvisibleSwitchManager.h"

#include "../Objects/InvisibleSwitch.h"

// 68K 0x10613140 __ct__23CInvisibleSwitchManagerFP3CAIi
// FUNCTION: LEMBALL 0x0040a210
InvisibleSwitchManager::InvisibleSwitchManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x20, 0x15)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_switches = 0;
}

// 68K 0x106131d6 Restart__23CInvisibleSwitchManagerFv
// FUNCTION: LEMBALL 0x0040a270
void InvisibleSwitchManager::Restart()
{
	if (m_switches != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_switches[i].Restart();
		}
	}
}

// 68K 0x10613244 Initialise__23CInvisibleSwitchManagerFi
// FUNCTION: LEMBALL 0x0040a2a0
void InvisibleSwitchManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_switches = 0;
		return;
	}
	if (m_switches == 0) {
		m_switches = new InvisibleSwitch[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_switches[i].Restart();
			m_switches[i].m_manager = this;
		}
	}
}

// 68K 0x10613398 StepOn__23CInvisibleSwitchManagerFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0040a370
void InvisibleSwitchManager::StepOn(const AiCoord& p_position, GameObject* p_object)
{
}

// 68K 0x1061341c Process__23CInvisibleSwitchManagerFv
// STUB: LEMBALL 0x0040a3b0
void InvisibleSwitchManager::Process()
{
}

// 68K 0x10613484 LoadLevel__23CInvisibleSwitchManagerFPUciUc
// FUNCTION: LEMBALL 0x0040a490
void InvisibleSwitchManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
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

// 68K 0x1061331c __dt__23CInvisibleSwitchManagerFv
InvisibleSwitchManager::~InvisibleSwitchManager()
{
}
