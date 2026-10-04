#include "CResTune.h"

#include "Visos/Resources/Archive/CMogRes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e460
CResTune* CResTune::Load(unsigned int p_resourceId)
{
	CResTune* res = (CResTune*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResTune*) (new CResTune(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_TUNE) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec00
void CResTune::SetType()
{
	m_chunkType = RESOURCE_CHUNK_TUNE;
}
