#include "CResZRLE.h"

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045e2b0
CResZRLE* CResZRLE::Load(unsigned long p_resourceId)
{
	register unsigned int id = p_resourceId;
	CResZRLE* res = (CResZRLE*) g_pActiveMogRes->Find(id);
	if (res == NULL) {
		return (CResZRLE*) (new CResZRLE(id))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_ZRLE) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e340
void CResZRLE::SetHeader()
{
	ZrleHeader* header = (ZrleHeader*) m_name;
	short y = header->m_y;
	short x = header->m_x;
	m_x = x;
	m_y = y;
	y = header->m_height;
	x = header->m_width;
	m_width = x;
	m_height = y;
	y = header->m_originY;
	x = header->m_originX;
	m_originX = x;
	m_originY = y;
}

// FUNCTION: LEMBALL 0x0045e7e0
CResZRLE::CResZRLE()
{
	m_height = 0;
	m_width = 0;
	m_originY = 0;
	m_originX = 0;
	Initialise();
}

// FUNCTION: LEMBALL 0x0045e840
void CResZRLE::SetType()
{
	m_chunkType = RESOURCE_CHUNK_ZRLE;
	m_headerSkip = 0xc;
}
