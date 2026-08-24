#include "StatManager.h"

#include "BaseStat.h"
#include "../Foundation/VsMem.h"
#include "../Foundation/VsOStream.h"

// 68K 0x10218c1a __ct__12CStatManagerFi
// FUNCTION: LEMBALL 0x0045aaf0
StatManager::StatManager(int p_arg0)
{
	m_stats = (BaseStat**) operator new((unsigned int) (p_arg0 * sizeof(BaseStat*)));
	m_capacity = p_arg0;
	m_statCount = 0;
}

// 68K 0x10218c74 __dt__12CStatManagerFv
// FUNCTION: LEMBALL 0x0045ab30
StatManager::~StatManager()
{
	int i;

	if (m_stats != 0) {
		i = 0;
		while (i < m_statCount) {
			if (m_stats[i] != 0) {
				m_stats[i]->~BaseStat();
				operator delete(m_stats[i]);
			}
			++i;
		}
		operator delete(m_stats);
		m_stats = 0;
	}
}

// 68K 0x10218d00 Register__12CStatManagerFP9CBaseStat
// FUNCTION: LEMBALL 0x0045ab90
void StatManager::Register(BaseStat* p_stat)
{
	if (m_statCount < m_capacity) {
		m_stats[m_statCount] = p_stat;
		++m_statCount;
	}
}

// 68K 0x10218d50 StreamOut__12CStatManagerFR10CVSOStream
// FUNCTION: LEMBALL 0x0045abb0
VsOStream& StatManager::StreamOut(VsOStream& p_stream)
{
	int i;

	// STRING: LEMBALL 0x004a1bd4 "Stats Output\n"
	// STRING: LEMBALL 0x004a1be4 "---------------------------------------------------\n"
	// STRING: LEMBALL 0x004a1c1c "Average  Total    Max      Min      nSamples Desc\n"
	// STRING: LEMBALL 0x004a1c50 "---------------------------------------------------\n"
	p_stream << "Stats Output\n";
	p_stream << "---------------------------------------------------\n";
	p_stream << "Average  Total    Max      Min      nSamples Desc\n";
	p_stream << "---------------------------------------------------\n";
	i = 0;
	while (i < m_statCount) {
		if (m_stats[i] != 0) {
			m_stats[i]->StreamOut(p_stream);
		}
		++i;
	}
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a1bd0
StatManager* g_pStatManager = 0;
