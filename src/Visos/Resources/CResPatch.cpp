#include "CResPatch.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045e3f0
CResPatch* CResPatch::Load(unsigned int p_resourceId)
{
	CResPatch* res = (CResPatch*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResPatch*) (new CResPatch(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != kChunkPtch) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ebd0
void CResPatch::SetType()
{
	m_chunkType = kChunkPtch;
}
