#include "../CResBITMAP.h"

#include "../CMogRes.h"

// FUNCTION: LEMBALL 0x0045e210
CResBITMAP* CResBITMAP::Load(unsigned int p_resourceId)
{
	CResBITMAP* res = (CResBITMAP*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		res = new CResBITMAP(p_resourceId);
		return (CResBITMAP*) res->CheckError();
	}
	if (res->m_chunkType != 0x42544d50) {
		res->UnLoad();
		return 0;
	}
	return res;
}
