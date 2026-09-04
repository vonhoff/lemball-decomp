#include "ResPalette.h"

#include "MogRes.h"

// 68K 0x1020401a Load__11CResPALETTEFUl
// FUNCTION: LEMBALL 0x0045dd90
ResPalette* ResPalette::Load(unsigned int p_resourceId)
{
	register unsigned int id = p_resourceId;
	ResPalette* res = (ResPalette*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		return (ResPalette*) (new ResPalette(id))->CheckError();
	}
	if (res->m_chunkType != 0x50414c20) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x10100ef0 SetType__11CResPALETTEFv
// FUNCTION: LEMBALL 0x0045ea30
void ResPalette::SetType()
{
	m_chunkType = 0x50414c20;
	m_headerSkip = 4;
}

// 68K 0x10100f26 SetHeader__11CResPALETTEFv
// FUNCTION: LEMBALL 0x0045ea40
void ResPalette::SetHeader()
{
	PaletteHeader* header = (PaletteHeader*) m_name;
	m_paletteState = header->m_paletteState;
}
