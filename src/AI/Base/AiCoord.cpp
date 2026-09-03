#include "AiCoord.h"

// FUNCTION: LEMBALL 0x00410b50
AiCoord::AiCoord(const AiCoord& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
}
