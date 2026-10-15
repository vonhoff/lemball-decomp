#include "CResBIN.h"

#include <stddef.h>

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

// FUNCTION: LEMBALL 0x0045e540
CResBIN* CResBIN::Load(unsigned long p_resourceId)
{
	CResBIN* res;
	if ((res = (CResBIN*) g_pActiveMogRes->Find(p_resourceId)) == NULL) {
		res = new CResBIN(p_resourceId);
		return (CResBIN*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_BIN) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec60
void CResBIN::SetType()
{
	m_chunkType = RESOURCE_CHUNK_BIN;
}
