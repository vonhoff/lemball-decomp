#include "BaseStat.h"

#include "../Foundation/VsOStream.h"

BaseStat::BaseStat()
{
}

// 68K 0x10218df8 __ct__9CBaseStatFPc
// FUNCTION: LEMBALL 0x0045ac10
BaseStat::BaseStat(char* p_arg0)
{
	m_description = p_arg0;
	m_minimum = 0xffffffff;
	m_maximum = 0;
	m_total = 0;
	m_sampleCount = 0;
}

// 68K 0x10218e5e __dt__9CBaseStatFv
// FUNCTION: LEMBALL 0x0045ac50
BaseStat::~BaseStat()
{
}

// 68K 0x10218eb2 Update__9CBaseStatFUl
// FUNCTION: LEMBALL 0x0045ac60
void BaseStat::Update(unsigned int p_value)
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

// 68K 0x10218f14 StreamOut__9CBaseStatFR10CVSOStream
// FUNCTION: LEMBALL 0x0045ac90
VsOStream& BaseStat::StreamOut(VsOStream& p_stream)
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
