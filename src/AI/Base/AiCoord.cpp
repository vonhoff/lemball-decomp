#include "AiCoord.h"

// FUNCTION: LEMBALL 0x0040c2b0
AiCoord& AiCoord::operator=(const AiCoord& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
	return *this;
}

// FUNCTION: LEMBALL 0x00410b50
AiCoord::AiCoord(const AiCoord& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
}
