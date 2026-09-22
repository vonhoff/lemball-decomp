#include "CStatManager.h"

#include "../Foundation/CVSOStream.h"
#include "CBaseStat.h"

// FUNCTION: LEMBALL 0x0045aaf0
CStatManager::CStatManager(int p_arg0)
{
	m_stats = (CBaseStat**) operator new((unsigned int) (p_arg0 * sizeof(CBaseStat*)));
	m_capacity = p_arg0;
	m_statCount = 0;
}

// FUNCTION: LEMBALL 0x0045ab30
CStatManager::~CStatManager()
{
	int i;

	if (m_stats != 0) {
		i = 0;
		while (i < m_statCount) {
			delete m_stats[i];
			++i;
		}
		operator delete(m_stats);
		m_stats = 0;
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
	int i;

	p_stream << "Stats Output\n";
	p_stream << "---------------------------------------------------\n";
	p_stream << "Average  Total    Max      Min      nSamples Desc\n";
	i = 0;
	while (i < m_statCount) {
		m_stats[i]->StreamOut(p_stream);
		++i;
	}
	p_stream << "---------------------------------------------------\n";
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a1bd0
CStatManager* g_pStatManager = 0;
