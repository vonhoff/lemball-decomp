#include "ResPalette.h"

// 68K 0x1020401a Load__11CResPALETTEFUl
// STUB: LEMBALL 0x0045dd90
ResPalette* ResPalette::Load(unsigned int p_resourceId)
{
	return 0;
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

// 68K 0x10100f5c __dt__11CResPALETTEFv
// SYNTHETIC: LEMBALL 0x0045ea50
// ResPalette::`scalar deleting destructor'
ResPalette::~ResPalette()
{
}
