#include "CResPALETTE.h"
#include "ResourceChunkTypes.h"

#include "CMogRes.h"

#include <new.h>
#include <stddef.h>

// FUNCTION: LEMBALL 0x0045dd90
CResPALETTE* CResPALETTE::Load(unsigned int p_resourceId)
{
	void* storage;
	CResPALETTE* res;
	register unsigned int id = p_resourceId;
	res = (CResPALETTE*) g_pActiveMogRes->Find(id);
	if (res == NULL) {
		storage = operator new(sizeof(CResPALETTE));
		if (storage != NULL) {
			res = new (storage) CResPALETTE(id);
		}
		else {
			res = NULL;
		}
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
