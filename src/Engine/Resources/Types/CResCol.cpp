#include "CResCol.h"

#include "Engine/Resources/Archive/CMogRes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045dd20
CResCol* CResCol::Load(unsigned long p_resourceId)
{
	CResCol* res;
	res = (CResCol*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		res = new CResCol(p_resourceId);
		return (CResCol*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_COLOUR) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e9f0
void CResCol::SetType()
{
	m_chunkType = RESOURCE_CHUNK_COLOUR;
}

// FUNCTION: LEMBALL 0x0045ea00
void CResCol::OnLoad()
{
	m_colour = *(unsigned int*) m_data;
}
