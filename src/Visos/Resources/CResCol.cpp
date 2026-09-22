#include "CResCol.h"

#include "CMogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045dd20
CResCol* CResCol::Load(unsigned int p_resourceId)
{
	void* storage;
	CResCol* res;
	register unsigned int id = p_resourceId;
	res = (CResCol*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		storage = operator new(sizeof(CResCol));
		if (storage != 0) {
			res = new (storage) CResCol(id);
		}
		else {
			res = 0;
		}
		return (CResCol*) res->CheckError();
	}
	if (res->m_chunkType != kChunkCol) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e9f0
void CResCol::SetType()
{
	m_chunkType = kChunkCol;
}

// FUNCTION: LEMBALL 0x0045ea00
void CResCol::OnLoad()
{
	m_colour = *(unsigned int*) m_data;
}
