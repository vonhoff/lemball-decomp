#include "CResBITMAP.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include "Engine/Resources/Archive/CMogRes.h"

#include <stddef.h>

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
	BitmapHeader* header = (BitmapHeader*) m_name;
	unsigned short height = header->m_height;
	unsigned int width = header->m_width;
	m_x = (unsigned short) width;
	m_y = height;
	m_depth = header->m_depth;
	m_flags = header->m_flags;
}

// FUNCTION: LEMBALL 0x0045eb70
void CResBITMAP::SetType()
{
	m_chunkType = RESOURCE_CHUNK_BITMAP;
	m_headerSkip = 0xc;
}
