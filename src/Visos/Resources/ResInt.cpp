#include "ResInt.h"

// 68K 0x10116002 __ct__7CResINTFv
// FUNCTION: LEMBALL 0x0045e8f0
ResInt::ResInt()
{
	Initialise();
}

// 68K 0x10100e22 SetType__7CResINTFv
// FUNCTION: LEMBALL 0x0045e910
void ResInt::SetType()
{
	m_chunkType = 0x494e5420;
}

// 68K 0x10100e4c OnLoad__7CResINTFv
// FUNCTION: LEMBALL 0x0045e920
void ResInt::OnLoad()
{
	m_value = ((IntPayload*) m_data)->m_value;
}

// 68K 0x10100e7a __dt__7CResINTFv
// SYNTHETIC: LEMBALL 0x0045e930
// ResInt::`scalar deleting destructor'
ResInt::~ResInt()
{
}
