#include "CSlinkyManager.h"

#include "../Navigation/CAI.h"
#include "../Objects/CSlinky.h"
#include "AI/Base/CGameObject.h"

// FUNCTION: LEMBALL 0x0040b8e0
CSlinkyManager::CSlinkyManager(CAI* p_ai, int p_capacity)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_slinkies = 0;
}

// FUNCTION: LEMBALL 0x0040b900
void CSlinkyManager::Restart()
{
	if (m_slinkies != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_slinkies[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040b930
void CSlinkyManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_slinkies = 0;
		return;
	}
	if (m_slinkies == 0) {
		m_slinkies = new CSlinky[p_capacity];
		// LINE: LEMBALL 0x0040b99b
		for (int i = 0; i < m_capacity; i++) {
			m_slinkies[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040b9d0
CSlinkyManager::~CSlinkyManager()
{
	if (m_slinkies != 0) {
		delete[] m_slinkies;
	}
}

// FUNCTION: LEMBALL 0x0040b9e0
void CSlinkyManager::Add(int p_id, int p_minX, int p_minY, int p_maxX, int p_maxY)
{
	if (m_count < m_capacity) {
		m_slinkies[m_count].SetId((unsigned short) p_id);
		m_slinkies[m_count].Set(p_minX, p_maxX, p_minY, p_maxY);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x0040bdd0
int CSlinkyManager::GetViewData(CViewData* p_viewData)
{
	int i = 0;
	int count = 0;
	if (m_count > 0) {
		CSlinky* slinky = m_slinkies;
		do {
			slinky->GetViewData(*p_viewData);
			p_viewData++;
			slinky++;
			count++;
			i++;
		} while (m_count > i);
	}
	return count;
}

// FUNCTION: LEMBALL 0x0040be20
void CSlinkyManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_slinkies[i].Process();
	}
}

// FUNCTION: LEMBALL 0x0040be50
void CSlinkyManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data++;
	Initialise(count);
	if (count != 0) {
		unsigned int remaining = count;
		unsigned short id;
		int minX;
		int minY;
		int maxX;
		int maxY;
		do {
			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			minX = *data++;
			minY = *data++;
			maxX = *data++;
			maxY = *data++;
			Add(id, minX, minY, maxX, maxY);
			remaining--;
		} while (remaining != 0);
	}
}
