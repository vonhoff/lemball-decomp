#include "CResEffect.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045e380
CResEffect* CResEffect::Load(unsigned int p_resourceId)
{
	CResEffect* res = (CResEffect*) g_pActiveMogRes->Find(p_resourceId);
	if (!res) {
		res = new CResEffect(p_resourceId);
		return (CResEffect*) res->CheckError();
	}
	if (res->m_chunkType != 0x45464620) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045eba0
void CResEffect::SetType()
{
	m_chunkType = 0x45464620;
}
