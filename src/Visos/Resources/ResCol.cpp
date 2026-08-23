#include "ResCol.h"

#include "MogRes.h"

// FUNCTION: LEMBALL 0x0045dd20
ResCol* ResCol::Load(unsigned int p_resourceId)
{
	register unsigned int id = p_resourceId;
	ResCol* res = (ResCol*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		return (ResCol*) (new ResCol(id))->CheckError();
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

// SYNTHETIC: LEMBALL 0x0045ea10
// ResCol::`scalar deleting destructor'
ResCol::~ResCol()
{
}
