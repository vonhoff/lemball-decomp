#include "CResINT.h"

// FUNCTION: LEMBALL 0x0045e8f0
CResINT::CResINT()
{
	Initialise();
}

// FUNCTION: LEMBALL 0x0045e910
void CResINT::SetType()
{
	m_chunkType = 0x494e5420;
}

// FUNCTION: LEMBALL 0x0045e920
void CResINT::OnLoad()
{
	m_value = ((IntPayload*) m_data)->m_value;
}
