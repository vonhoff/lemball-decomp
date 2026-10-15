#include "CStatManager.h"

#include <stddef.h>

#include "CBaseStat.h"
#include "Engine/Streams/CVSOStream.h"

// FUNCTION: LEMBALL 0x0045aaf0
CStatManager::CStatManager(int p_capacity)
{
	m_stats = new CBaseStat*[p_capacity];
	m_capacity = p_capacity;
	m_statCount = 0;
}

// FUNCTION: LEMBALL 0x0045ab30
CStatManager::~CStatManager()
{
	if (m_stats != NULL) {
		for (int i = 0; i < m_statCount; ++i) {
			delete m_stats[i];
		}
		delete[] m_stats;
		m_stats = NULL;
	}
}

// FUNCTION: LEMBALL 0x0045ab90
void CStatManager::Register(CBaseStat* p_stat)
{
	if (m_statCount < m_capacity) {
		m_stats[m_statCount] = p_stat;
		++m_statCount;
	}
}

// FUNCTION: LEMBALL 0x0045abb0
CVSOStream& CStatManager::StreamOut(CVSOStream& p_stream)
{
	p_stream << "Stats Output\n";
	p_stream << "---------------------------------------------------\n";
	p_stream << "Average  Total    Max      Min      nSamples Desc\n";
	for (int i = 0; i < m_statCount; ++i) {
		m_stats[i]->StreamOut(p_stream);
	}
	p_stream << "---------------------------------------------------\n";
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a1bd0
CStatManager* g_pStatManager = NULL;
