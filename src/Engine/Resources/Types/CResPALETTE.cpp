#include "CResPALETTE.h"

#include <stddef.h>

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

// FUNCTION: LEMBALL 0x0045dd90
CResPALETTE* CResPALETTE::Load(unsigned long p_resourceId)
{
	CResPALETTE* res;
	res = (CResPALETTE*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		res = new CResPALETTE(p_resourceId);
		return (CResPALETTE*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_PALETTE) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ea30
void CResPALETTE::SetType()
{
	m_chunkType = RESOURCE_CHUNK_PALETTE;
	m_headerSkip = 4;
}

// FUNCTION: LEMBALL 0x0045ea40
void CResPALETTE::SetHeader()
{
	PaletteHeader* header = (PaletteHeader*) m_name;
	m_entryCount = header->m_entryCount;
}
