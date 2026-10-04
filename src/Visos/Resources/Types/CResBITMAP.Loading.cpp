#include "CResBITMAP.h"
#include "Visos/Resources/ResourceChunkTypes.h"

#include "Visos/Resources/Archive/CMogRes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e210
CResBITMAP* CResBITMAP::Load(unsigned int p_resourceId)
{
	CResBITMAP* res = (CResBITMAP*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		res = new CResBITMAP(p_resourceId);
		return (CResBITMAP*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_BITMAP) {
		res->UnLoad();
		return NULL;
	}
	return res;
}
