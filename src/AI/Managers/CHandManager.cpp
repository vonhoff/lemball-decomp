#include "CHandManager.h"

#include "../Navigation/CAi.h"
#include "../Objects/CHand.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x00427e60
CHandManager::CHandManager(CAi* p_arg0, int p_arg1) : CBaseObjectManager(0x16, 0xb)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_hands = 0;
}

// FUNCTION: LEMBALL 0x00427ec0
void CHandManager::Restart()
{
	if (m_hands != 0) {
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
		m_hands = 0;
		return;
	}
	if (m_hands == 0) {
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

// FUNCTION: LEMBALL 0x00428360
bool CHandManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	int i = 0;
	for (;;) {
		if (i >= m_count) {
			return 0;
		}
		CHand& hand = m_hands[i];
		if (hand.m_enabled != 0 && hand.m_activated == 0 && hand.m_isRemoteObject == 0 &&
			hand.StepOn(p_position, p_object)) {
			return 1;
		}
		i++;
	}
}

// FUNCTION: LEMBALL 0x004283c0
void CHandManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_hands[i].m_requestEnabled = 1;
		if (m_hands[i].m_activated != 0 || m_hands[i].m_isRemoteObject != 0) {
			m_hands[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x00428410
int CHandManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	for (int i = 0; i < m_count; i++) {
		m_hands[i].GetViewData(*p_viewData++);
		count++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x00428460
void CHandManager::Add(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		AiCoord position(p_x << 12, p_y << 12, p_z << 12);
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
