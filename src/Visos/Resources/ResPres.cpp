#include "ResPres.h"

#include "CMogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045e4d0
ResPres* ResPres::Load(unsigned int p_resourceId)
{
	void* storage;
	ResPres* res;
	res = (ResPres*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		storage = operator new(sizeof(ResPres));
		if (storage != 0) {
			res = new (storage) ResPres(p_resourceId);
		}
		else {
			res = 0;
		}
		return (ResPres*) res->CheckError();
	}
	if (res->m_chunkType != kChunkPres) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec30
void ResPres::SetType()
{
	m_chunkType = kChunkPres;
}
