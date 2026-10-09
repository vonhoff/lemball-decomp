#include "CResCol.h"

#include "Engine/Resources/Archive/CMogRes.h"

#include <new.h>
#include <stddef.h>

// FUNCTION: LEMBALL 0x0045dd20
CResCol* CResCol::Load(unsigned long p_resourceId)
{
	void* storage;
	CResCol* res;
	unsigned long id = p_resourceId;
	res = (CResCol*) g_pActiveMogRes->Find(id);
	if (res == NULL) {
		storage = operator new(sizeof(CResCol));
		if (storage != NULL) {
			res = new (storage) CResCol(id);
		}
		else {
			res = NULL;
		}
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
