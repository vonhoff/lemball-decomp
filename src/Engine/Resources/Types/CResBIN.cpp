#include "CResBIN.h"

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include <new.h>
#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e540
CResBIN* CResBIN::Load(unsigned int p_resourceId)
{
	void* storage;
	CResBIN* res;
	if ((res = (CResBIN*) g_pActiveMogRes->Find(p_resourceId)) == NULL) {
		storage = operator new(sizeof(CResBIN));
		if (storage != NULL) {
			res = new (storage) CResBIN(p_resourceId);
		}
		else {
			res = NULL;
		}
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
