#include "CResEFFECT.h"

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e380
CResEFFECT* CResEFFECT::Load(unsigned int p_resourceId)
{
	CResEFFECT* res = (CResEFFECT*) g_pActiveMogRes->Find(p_resourceId);
	if (!res) {
		res = new CResEFFECT(p_resourceId);
		return (CResEFFECT*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_EFFECT) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045eba0
void CResEFFECT::SetType()
{
	m_chunkType = RESOURCE_CHUNK_EFFECT;
}
