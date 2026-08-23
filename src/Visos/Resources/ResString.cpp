#include "ResString.h"

#include "MogRes.h"

// 68K 0x102040ba Load__10CResSTRINGFUl
// FUNCTION: LEMBALL 0x0045de00
ResString* ResString::Load(unsigned int p_resourceId)
{
	ResString* res = (ResString*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResString*) (new ResString(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x53545247) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x10100fae SetType__10CResSTRINGFv
// FUNCTION: LEMBALL 0x0045ea70
void ResString::SetType()
{
	m_chunkType = 0x53545247;
}

// 68K 0x10116044 __ct__10CResSTRINGFv
// FUNCTION: LEMBALL 0x0045eaa0
ResString::ResString()
{
	Initialise();
}

// 68K 0x10100fdc __dt__10CResSTRINGFv
// SYNTHETIC: LEMBALL 0x0045eac0
// ResString::`scalar deleting destructor'
ResString::~ResString()
{
}
