#include "CResPalette.h"

#include "CMogRes.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045dd90
CResPalette* CResPalette::Load(unsigned int p_resourceId)
{
	void* storage;
	CResPalette* res;
	register unsigned int id = p_resourceId;
	res = (CResPalette*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		storage = operator new(sizeof(CResPalette));
		if (storage != 0) {
			res = new (storage) CResPalette(id);
		}
		else {
			res = 0;
		}
		return (CResPalette*) res->CheckError();
	}
	if (res->m_chunkType != 0x50414c20) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ea30
void CResPalette::SetType()
{
	m_chunkType = 0x50414c20;
	m_headerSkip = 4;
}

// FUNCTION: LEMBALL 0x0045ea40
void CResPalette::SetHeader()
{
	PaletteHeader* header = (PaletteHeader*) m_name;
	m_paletteState = header->m_paletteState;
}
