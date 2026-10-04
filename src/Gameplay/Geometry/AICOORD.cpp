#include "AICOORD.h"

// FUNCTION: LEMBALL 0x00410b50
AICOORD::AICOORD(const AICOORD& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
}
