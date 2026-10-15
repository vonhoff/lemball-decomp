#include "CResBITMAP.h"

#include <stddef.h>

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

// FUNCTION: LEMBALL 0x0045e210
CResBITMAP* CResBITMAP::Load(unsigned long p_resourceId)
{
	CResBITMAP* res = (CResBITMAP*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		res = new CResBITMAP(p_resourceId);
		return (CResBITMAP*) res->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_BITMAP) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e290
void CResBITMAP::SetHeader()
{
	const BitmapHeader* header = (const BitmapHeader*) m_entryHeader;
	unsigned short height = (unsigned short) header->m_height;
	unsigned int width = header->m_width;
	m_rasterPoint.m_x = (unsigned short) width;
	m_rasterPoint.m_y = height;
	m_depth = header->m_depth;
	m_flags = header->m_flags;
}

// FUNCTION: LEMBALL 0x0045eb70
void CResBITMAP::SetType()
{
	m_chunkType = RESOURCE_CHUNK_BITMAP;
	m_headerSkip = 0xc;
}
