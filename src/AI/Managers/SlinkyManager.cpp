#include "SlinkyManager.h"

#include "../Navigation/Ai.h"
#include "../Objects/Slinky.h"
#include "../Objects/ViewData.h"

// 68K 0x1061f090 __ct__14CSlinkyManagerFP3CAIi
// FUNCTION: LEMBALL 0x0040b8e0
SlinkyManager::SlinkyManager(Ai* p_arg0, int p_arg1)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_slinkies = 0;
}

// 68K 0x1061f0ca Restart__14CSlinkyManagerFv
// FUNCTION: LEMBALL 0x0040b900
void SlinkyManager::Restart()
{
	if (m_slinkies != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_slinkies[i].Restart();
		}
	}
}

// 68K 0x1061f128 Initialise__14CSlinkyManagerFi
// FUNCTION: LEMBALL 0x0040b930
void SlinkyManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_slinkies = 0;
		return;
	}
	if (m_slinkies == 0) {
		m_slinkies = new Slinky[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_slinkies[i].Restart();
		}
	}
}

// 68K 0x1061f1d6 __dt__14CSlinkyManagerFv
SlinkyManager::~SlinkyManager()
{
}

// 68K 0x1061f230 Add__14CSlinkyManagerFiiiii
// STUB: LEMBALL 0x0040b9e0
void SlinkyManager::Add(int p_id, int p_minX, int p_minY, int p_maxX, int p_maxY)
{
}

// 68K 0x1061f2aa GetViewData__14CSlinkyManagerFP9CViewData
// FUNCTION: LEMBALL 0x0040bdd0
int SlinkyManager::GetViewData(ViewData* p_viewData)
{
	int i = 0;
	int count = 0;
	if (m_count > 0) {
		Slinky* slinky = m_slinkies;
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

// 68K 0x1061f322 Process__14CSlinkyManagerFv
// FUNCTION: LEMBALL 0x0040be20
void SlinkyManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_slinkies[i].Process();
	}
}

// 68K 0x1061f37a LoadLevel__14CSlinkyManagerFPUciUc
// FUNCTION: LEMBALL 0x0040be50
void SlinkyManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
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
				id = (unsigned short) GameObject::NextId();
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
