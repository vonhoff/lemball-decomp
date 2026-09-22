#include "CResPres.h"

#include "CMogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045e4d0
CResPres* CResPres::Load(unsigned int p_resourceId)
{
	void* storage;
	CResPres* res;
	res = (CResPres*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		storage = operator new(sizeof(CResPres));
		if (storage != 0) {
			res = new (storage) CResPres(p_resourceId);
		}
		else {
			res = 0;
		}
		return (CResPres*) res->CheckError();
	}
	if (res->m_chunkType != kChunkPres) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec30
void CResPres::SetType()
{
	m_chunkType = kChunkPres;
}
