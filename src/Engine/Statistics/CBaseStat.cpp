#include "CBaseStat.h"

#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/CString.h"

enum eStatInitialBound {
	STAT_MINIMUM_INITIAL_UPPER_BOUND = 0xffffffffUL
};

// FUNCTION: LEMBALL 0x0045ac10
CBaseStat::CBaseStat(char* p_description)
{
	m_description = p_description;
	m_minimum = STAT_MINIMUM_INITIAL_UPPER_BOUND;
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
CVSOStream& CBaseStat::StreamOut(CVSOStream& p_stream)
{
	if (m_sampleCount != 0) {
		p_stream << HEX8(m_total / m_sampleCount) << " " << HEX8(m_total) << " " << HEX8(m_maximum) << " "
				 << HEX8(m_minimum) << " " << HEX8(m_sampleCount) << " " << m_description << "\n";
	}
	else {
		p_stream << "----\n";
	}
	return p_stream;
}
