#include "ResTune.h"

#include "MogRes.h"

// FUNCTION: LEMBALL 0x0045e460
ResTune* ResTune::Load(unsigned int p_resourceId)
{
	ResTune* res = (ResTune*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResTune*) (new ResTune(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != kChunkTune) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec00
void ResTune::SetType()
{
	m_chunkType = kChunkTune;
}

// SYNTHETIC: LEMBALL 0x0045ec10
// ResTune::`scalar deleting destructor'
ResTune::~ResTune()
{
}
