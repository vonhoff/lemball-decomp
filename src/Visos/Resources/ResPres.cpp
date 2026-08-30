#include "ResPres.h"

#include "MogRes.h"

// FUNCTION: LEMBALL 0x0045e4d0
ResPres* ResPres::Load(unsigned int p_resourceId)
{
	ResPres* res = (ResPres*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResPres*) (new ResPres(p_resourceId))->CheckError();
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

ResPres::~ResPres()
{
}
