#include "ResString.h"

// 68K 0x102040ba Load__10CResSTRINGFUl
// STUB: LEMBALL 0x0045de00
ResString* ResString::Load(unsigned int p_resourceId)
{
	return 0;
}

// 68K 0x10100fae SetType__10CResSTRINGFv
// FUNCTION: LEMBALL 0x0045ea70
void ResString::SetType()
{
	m_chunkType = 0x53545247;
}

// 68K 0x10116044 __ct__10CResSTRINGFv
// STUB: LEMBALL 0x0045eaa0
ResString::ResString()
{
}

// 68K 0x10100fdc __dt__10CResSTRINGFv
// SYNTHETIC: LEMBALL 0x0045eac0
// ResString::`scalar deleting destructor'
ResString::~ResString()
{
}
