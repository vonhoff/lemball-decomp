#include "ResCol.h"

#include "MogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045dd20
ResCol* ResCol::Load(unsigned int p_resourceId)
{
	void* storage;
	ResCol* res;
	register unsigned int id = p_resourceId;
	res = (ResCol*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		storage = operator new(sizeof(ResCol));
		if (storage != 0) {
			res = new (storage) ResCol(id);
		}
		else {
			res = 0;
		}
		return (ResCol*) res->CheckError();
	}
	if (res->m_chunkType != kChunkCol) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e9f0
void ResCol::SetType()
{
	m_chunkType = kChunkCol;
}

// FUNCTION: LEMBALL 0x0045ea00
void ResCol::OnLoad()
{
	m_colour = *(unsigned int*) m_data;
}
