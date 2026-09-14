#include "ResBool.h"

// FUNCTION: LEMBALL 0x0045e9b0
void ResBool::SetType()
{
	m_chunkType = 0x424f4f4c;
}

// FUNCTION: LEMBALL 0x0045e9c0
void ResBool::OnLoad()
{
	m_value = *(unsigned int*) m_data;
}
