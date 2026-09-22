#include "CTrampolineManager.h"

#include "../Navigation/CAi.h"
#include "../Objects/CTrampoline.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0042af60
CTrampolineManager::CTrampolineManager(CAi* p_arg0, int p_arg1) : CBaseObjectManager(0x18, 0xd)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_trampolines = 0;
}

// FUNCTION: LEMBALL 0x0042afc0
void CTrampolineManager::Restart()
{
	if (m_trampolines != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_trampolines[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042aff0
void CTrampolineManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_trampolines = 0;
		return;
	}
	if (m_trampolines == 0) {
		m_trampolines = new CTrampoline[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_trampolines[i].m_manager = this;
			m_trampolines[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042b090
CTrampolineManager::~CTrampolineManager()
{
	delete[] m_trampolines;
}

// Ownership inferred from the adjacent manager functions and count at 0x34.
// FUNCTION: LEMBALL 0x0042b0b0
void CTrampolineManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x0042b440
int CTrampolineManager::TryEnableTrampolineNearPosition(const AiCoord& p_position, CGameObject* p_object)
{
	for (int i = 0;; i++) {
		if (m_count <= i) {
			return 0;
		}
		CTrampoline* trampoline = &m_trampolines[i];
		if (trampoline->m_active != 0 && trampoline->m_enabled == 0 &&
			trampoline->TryEnableNearPosition(p_position, p_object) != 0) {
			return 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0042b4a0
void CTrampolineManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_trampolines[i].m_requestEnabled = 1;
		if (m_trampolines[i].m_enabled != 0) {
			m_trampolines[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x0042b4f0
int CTrampolineManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	while (i < m_count) {
		if (m_trampolines[i].m_enabled != 0) {
			m_trampolines[i].GetViewData(*p_viewData++);
			count++;
		}
		i++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0042b550
int CTrampolineManager::Hit(const AiCoord& p_position, CGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		if (m_trampolines[i].Hit(p_position, p_object) != 0) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042b5a0
void CTrampolineManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		AiCoord position(p_x << 12, p_y << 12, p_z << 12);
		m_trampolines[m_count].Set(p_id, position);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0042b600
void CTrampolineManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
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
				id = (unsigned short) CGameObject::NextId();
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
