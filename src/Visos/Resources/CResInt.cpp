#include "CResInt.h"

// FUNCTION: LEMBALL 0x0045e8f0
CResInt::CResInt()
{
	Initialise();
}

// FUNCTION: LEMBALL 0x0045e910
void CResInt::SetType()
{
	m_chunkType = 0x494e5420;
}

// FUNCTION: LEMBALL 0x0045e920
void CResInt::OnLoad()
{
	m_value = ((IntPayload*) m_data)->m_value;
}
