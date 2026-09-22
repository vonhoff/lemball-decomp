#include "CBaseStat.h"

#include "../Foundation/CVsOStream.h"
#include "Visos/Foundation/CString.h"

// FUNCTION: LEMBALL 0x0045ac10
CBaseStat::CBaseStat(char* p_arg0)
{
	m_description = p_arg0;
	m_minimum = 0xffffffff;
	m_maximum = 0;
	m_total = 0;
	m_sampleCount = 0;
}

// FUNCTION: LEMBALL 0x0045ac50
CBaseStat::~CBaseStat()
{
}

// FUNCTION: LEMBALL 0x0045ac60
void CBaseStat::Update(unsigned int p_value)
{
	unsigned int count;
	unsigned int bound;

	count = m_sampleCount;
	if (count != 0) {
		m_total += p_value;
		bound = m_maximum;
		if (bound <= p_value) {
			bound = p_value;
		}
		m_maximum = bound;
		bound = m_minimum;
		if (bound >= p_value) {
			bound = p_value;
		}
		m_minimum = bound;
	}
	++count;
	m_sampleCount = count;
}

// FUNCTION: LEMBALL 0x0045ac90
CVsOStream& CBaseStat::StreamOut(CVsOStream& p_stream)
{
	if (m_sampleCount != 0) {
		p_stream << Hex8(m_total / m_sampleCount) << " " << Hex8(m_total) << " " << Hex8(m_maximum) << " "
				 << Hex8(m_minimum) << " " << Hex8(m_sampleCount) << " " << m_description << "\n";
	}
	else {
		p_stream << "----\n";
	}
	return p_stream;
}
