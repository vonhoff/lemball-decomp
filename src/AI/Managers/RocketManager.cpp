#include "RocketManager.h"

#include "../Navigation/Ai.h"
#include "../Objects/Rocket.h"

// 68K 0x1061dcfe __ct__14CRocketManagerFP3CAIi
// FUNCTION: LEMBALL 0x00426ac0
RocketManager::RocketManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x14, 9)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_rockets = 0;
}

// 68K 0x1061dd8a Restart__14CRocketManagerFv
// FUNCTION: LEMBALL 0x00426b20
void RocketManager::Restart()
{
	if (m_rockets != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_rockets[i].Restart();
		}
	}
}

// 68K 0x1061ddee Initialise__14CRocketManagerFi
// FUNCTION: LEMBALL 0x00426b50
void RocketManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_rockets = 0;
		return;
	}
	if (m_rockets == 0) {
		m_rockets = new Rocket[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_rockets[i].Restart();
			m_rockets[i].m_manager = this;
		}
	}
}

// 68K 0x1061df34 StepOn__14CRocketManagerFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x00426fb0
int RocketManager::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	int i = 0;
	while (true) {
		if (m_count <= i) {
			return 0;
		}
		Rocket* rocket = &m_rockets[i];
		if (rocket->m_active != 0 && rocket->m_action == (eAction) 0x18 &&
			rocket->m_requestedAction == (eAction) 0x18 && rocket->StepOn(p_position, p_object) != 0) {
			return 1;
		}
		i++;
	}
}

// 68K 0x1061dffa Process__14CRocketManagerFv
// FUNCTION: LEMBALL 0x00427010
void RocketManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_rockets[i].m_requestEnabled = 1;
		m_rockets[i].Process();
	}
}

// 68K 0x1061e06e GetViewData__14CRocketManagerFP9CViewData
// STUB: LEMBALL 0x00427050
int RocketManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1061e10a Add__14CRocketManagerFUsiii
// FUNCTION: LEMBALL 0x004270b0
void RocketManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		AiCoord position(p_x << 12, p_y << 12, p_z << 12);
		m_rockets[m_count].Set(p_id, position);
		m_count++;
	}
}

// 68K 0x1061e1ac LoadLevel__14CRocketManagerFPUciUc
// FUNCTION: LEMBALL 0x00427110
void RocketManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) GameObject::NextId();
			}
			unsigned short x = *(unsigned short*) p_data;
			p_data += 2;
			unsigned short y = *(unsigned short*) p_data;
			p_data += 2;
			unsigned short z = *(unsigned short*) p_data;
			p_data += 2;
			Add(id, x, y, z);
			remaining--;
		} while (remaining != 0);
	}
}

// 68K 0x1061debc __dt__14CRocketManagerFv
RocketManager::~RocketManager()
{
}
