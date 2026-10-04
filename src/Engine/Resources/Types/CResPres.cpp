#include "CResPres.h"

#include "Engine/Resources/Archive/CMogRes.h"

#include <new.h>
#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e4d0
CResPres* CResPres::Load(unsigned int p_resourceId)
{
	void* storage;
	CResPres* res;
	res = (CResPres*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		storage = operator new(sizeof(CResPres));
		if (storage != NULL) {
			res = new (storage) CResPres(p_resourceId);
		}
		else {
			res = NULL;
		}
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
