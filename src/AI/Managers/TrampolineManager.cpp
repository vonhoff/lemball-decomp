#include "TrampolineManager.h"

#include "../Navigation/Ai.h"
#include "../Objects/Trampoline.h"
#include "../Objects/ViewData.h"

// 68K 0x10620bf6 __ct__18CTrampolineManagerFP3CAIi
// STUB: LEMBALL 0x0042af60
TrampolineManager::TrampolineManager(Ai* p_arg0, int p_arg1)
{
}

// 68K 0x10620c88 Restart__18CTrampolineManagerFv
// FUNCTION: LEMBALL 0x0042afc0
void TrampolineManager::Restart()
{
	if (m_trampolines != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_trampolines[i].Restart();
		}
	}
}

// 68K 0x10620cf0 Initialise__18CTrampolineManagerFi
// FUNCTION: LEMBALL 0x0042aff0
void TrampolineManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_trampolines = 0;
		return;
	}
	if (m_trampolines == 0) {
		m_trampolines = new Trampoline[p_capacity];
	}
	for (int i = 0; i < m_capacity; i++) {
		m_trampolines[i].Restart();
	}
}

// 68K 0x10620e3e Process__18CTrampolineManagerFv
// FUNCTION: LEMBALL 0x0042b4a0
void TrampolineManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_trampolines[i].m_requestEnabled = 1;
		if (m_trampolines[i].m_enabled != 0) {
			m_trampolines[i].Process();
		}
	}
}

// 68K 0x10620ecc GetViewData__18CTrampolineManagerFP9CViewData
// FUNCTION: LEMBALL 0x0042b4f0
int TrampolineManager::GetViewData(ViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	while (i < m_count) {
		m_trampolines[i].GetViewData(*p_viewData);
		p_viewData++;
		count++;
		i++;
	}
	return count;
}

// 68K 0x10620f66 Hit__18CTrampolineManagerFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0042b550
int TrampolineManager::Hit(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x10620fec Add__18CTrampolineManagerFUsiii
// STUB: LEMBALL 0x0042b5a0
void TrampolineManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
}

// 68K 0x10621092 LoadLevel__18CTrampolineManagerFPUciUc
// FUNCTION: LEMBALL 0x0042b600
void TrampolineManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* stream = (unsigned short*) p_data;
	int count = *stream++;
	Initialise(count);
	while (count != 0) {
		int id;
		if (m_ai->m_levelVersion > 1) {
			id = *stream++;
		}
		else {
			id = GameObject::NextId();
		}
		int x = *stream++;
		int y = *stream++;
		int z = *stream++;
		Add((unsigned short) id, x, y, z);
		count--;
	}
}

// 68K 0x10620dc2 __dt__18CTrampolineManagerFv
TrampolineManager::~TrampolineManager()
{
}
