#include "CResBIN.h"

#include "CMogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045e540
CResBIN* CResBIN::Load(unsigned int p_resourceId)
{
	void* storage;
	CResBIN* res;
	if ((res = (CResBIN*) g_pActiveMogRes->Find(p_resourceId)) == 0) {
		storage = operator new(sizeof(CResBIN));
		if (storage != 0) {
			res = new (storage) CResBIN(p_resourceId);
		}
		else {
			res = 0;
		}
		return (CResBIN*) res->CheckError();
	}
	if (res->m_chunkType != 0x42494e20) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec60
void CResBIN::SetType()
{
	m_chunkType = 0x42494e20;
}
