#include "CResPatch.h"

#include "Engine/Resources/Archive/CMogRes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e3f0
CResPatch* CResPatch::Load(unsigned int p_resourceId)
{
	CResPatch* res = (CResPatch*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResPatch*) (new CResPatch(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_PATCH) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ebd0
void CResPatch::SetType()
{
	m_chunkType = RESOURCE_CHUNK_PATCH;
}
