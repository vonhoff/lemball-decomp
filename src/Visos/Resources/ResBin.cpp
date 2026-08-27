#include "ResBin.h"

#include "MogRes.h"

// 68K 0x10204846 Load__7CResBINFUl
// FUNCTION: LEMBALL 0x0045e540
ResBin* ResBin::Load(unsigned int p_resourceId)
{
	ResBin* res = (ResBin*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResBin*) (new ResBin(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x42494e20) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x101010ae SetType__7CResBINFv
// FUNCTION: LEMBALL 0x0045ec60
void ResBin::SetType()
{
	m_chunkType = 0x42494e20;
}

// 68K 0x101010d8 __dt__7CResBINFv
ResBin::~ResBin()
{
}

