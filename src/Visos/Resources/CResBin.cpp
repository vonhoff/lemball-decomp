#include "CResBin.h"

#include "CMogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045e540
CResBin* CResBin::Load(unsigned int p_resourceId)
{
	void* storage;
	CResBin* res;
	if ((res = (CResBin*) g_pActiveMogRes->Find(p_resourceId)) == 0) {
		storage = operator new(sizeof(CResBin));
		if (storage != 0) {
			res = new (storage) CResBin(p_resourceId);
		}
		else {
			res = 0;
		}
		return (CResBin*) res->CheckError();
	}
	if (res->m_chunkType != 0x42494e20) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ec60
void CResBin::SetType()
{
	m_chunkType = 0x42494e20;
}
