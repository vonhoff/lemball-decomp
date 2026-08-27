#include "ResPatch.h"

#include "MogRes.h"

// FUNCTION: LEMBALL 0x0045e3f0
ResPatch* ResPatch::Load(unsigned int p_resourceId)
{
	ResPatch* res = (ResPatch*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResPatch*) (new ResPatch(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != kChunkPtch) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ebd0
void ResPatch::SetType()
{
	m_chunkType = kChunkPtch;
}

ResPatch::~ResPatch()
{
}

