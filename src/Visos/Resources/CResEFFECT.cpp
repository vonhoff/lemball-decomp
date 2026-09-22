#include "CResEFFECT.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045e380
CResEFFECT* CResEFFECT::Load(unsigned int p_resourceId)
{
	CResEFFECT* res = (CResEFFECT*) g_pActiveMogRes->Find(p_resourceId);
	if (!res) {
		res = new CResEFFECT(p_resourceId);
		return (CResEFFECT*) res->CheckError();
	}
	if (res->m_chunkType != 0x45464620) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045eba0
void CResEFFECT::SetType()
{
	m_chunkType = 0x45464620;
}
