#include "CTrampolineManager.h"

#include "CTrampoline.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0042af60
CTrampolineManager::CTrampolineManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_TRAMPOLINES,
						 OBJECT_MANAGER_TRANSPORT_TRAMPOLINES)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_trampolines = NULL;
}

// FUNCTION: LEMBALL 0x0042afc0
void CTrampolineManager::Restart()
{
	if (m_trampolines != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_trampolines[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0042aff0
void CTrampolineManager::Initialise(int p_capacity)
{
	int i;
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_trampolines = NULL;
		return;
	}
	if (m_trampolines == NULL) {
		m_trampolines = new CTrampoline[p_capacity];
		for (i = 0; i < m_capacity; i++) {
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

// FUNCTION: LEMBALL 0x0042b0b0
void CTrampolineManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x0042b0c0
void CTrampolineManager::RemoveTrampoline(CTrampoline* p_trampoline)
{
	for (int index = 0; index < m_count; index++) {
		if (&m_trampolines[index] == p_trampoline) {
			m_trampolines[index].SetId(INVALID_OBJECT_ID);
			for (int next = index + 1; next < m_count; next++) {
				m_trampolines[next - 1] = m_trampolines[next];
			}
			m_count--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0042b440
int CTrampolineManager::TryEnableNear(const AICOORD& p_position, CGameObject* p_object)
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
		m_trampolines[i].m_stateProcessed = true;
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
int CTrampolineManager::Hit(const AICOORD& p_position, CGameObject* p_object)
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
		AICOORD position;
		position.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
		position.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
		position.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
		m_trampolines[m_count].Set(p_id, position);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0042b600
void CTrampolineManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned char* cursor = p_data;
	unsigned short count = *(unsigned short*) cursor;
	cursor += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
				id = *(unsigned short*) cursor;
				cursor += 2;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			unsigned short x = *(unsigned short*) cursor;
			cursor += 2;
			unsigned short y = *(unsigned short*) cursor;
			cursor += 2;
			unsigned short z = *(unsigned short*) cursor;
			cursor += 2;
			Add(id, x, y, z);
			remaining--;
		} while (remaining != 0);
	}
}
