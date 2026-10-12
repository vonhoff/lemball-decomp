#include "CResPres.h"

#include "Engine/Resources/Archive/CMogRes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e4d0
CResPres* CResPres::Load(unsigned long p_resourceId)
{
	CResPres* res;
	res = (CResPres*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		res = new CResPres(p_resourceId);
		return (CResPres*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_PRESENTATION) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec30
void CResPres::SetType()
{
	m_chunkType = RESOURCE_CHUNK_PRESENTATION;
}
