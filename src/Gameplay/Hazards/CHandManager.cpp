#include "CHandManager.h"

#include "CHand.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Level/LevelFormat.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00427e60
CHandManager::CHandManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_HANDS,
						 OBJECT_MANAGER_TRANSPORT_HANDS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_hands = NULL;
}

// FUNCTION: LEMBALL 0x00427ec0
void CHandManager::Restart()
{
	if (m_hands != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_hands[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x00427ef0
void CHandManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_hands = NULL;
		return;
	}
	if (m_hands == NULL) {
		m_hands = new CHand[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_hands[i].Restart();
			m_hands[i].m_manager = this;
		}
	}
}

// FUNCTION: LEMBALL 0x00427fa0
CHandManager::~CHandManager()
{
	delete[] m_hands;
}

// FUNCTION: LEMBALL 0x00427fc0
void CHandManager::ResetCount()
{
	m_count = 0;
}

// FUNCTION: LEMBALL 0x00427fd0
void CHandManager::RemoveHand(CGameObject* p_object)
{
	int index = 0;
	short id = p_object->GetId();
	for (; index < m_count; index++) {
		if (m_hands[index].GetId() == id) {
			int next = index + 1;
			m_hands[index].SetId(INVALID_OBJECT_ID);
			for (; next < m_count; next++) {
				m_hands[next - 1] = m_hands[next];
			}
			m_count--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x00428360
bool CHandManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	int i = 0;
	for (;;) {
		if (i >= m_count) {
			return false;
		}
		CHand& hand = m_hands[i];
		if (hand.m_enabled != 0 && hand.m_activated == 0 && hand.m_isRemoteObject == 0 &&
			hand.StepOn(p_position, p_object)) {
			return true;
		}
		i++;
	}
}

// FUNCTION: LEMBALL 0x004283c0
void CHandManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_hands[i].m_stateProcessed = true;
		if (m_hands[i].m_activated != 0 || m_hands[i].m_isRemoteObject != 0) {
			m_hands[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x00428410
int CHandManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	CViewData* view = p_viewData;
	for (int i = 0; i < m_count; i++) {
		m_hands[i].GetViewData(*view++);
		count++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x00428460
void CHandManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		AICOORD position;
		position.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
		position.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
		position.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
		m_hands[m_count].Set(p_id, position);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x004284c0
void CHandManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
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
