#include "Pt3.h"

#include "AiCoord.h"

// FUNCTION: LEMBALL 0x00429e50
void Pt3::InitializeFromAiCoord(const AiCoord& p_coordinate)
{
	m_x = p_coordinate.m_xFixed >> 12;
	m_y = p_coordinate.m_yFixed >> 12;
	m_z = p_coordinate.m_zFixed >> 12;
}
