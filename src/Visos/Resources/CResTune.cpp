#include "CResTune.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045e460
CResTune* CResTune::Load(unsigned int p_resourceId)
{
	CResTune* res = (CResTune*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResTune*) (new CResTune(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != kChunkTune) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec00
void CResTune::SetType()
{
	m_chunkType = kChunkTune;
}
