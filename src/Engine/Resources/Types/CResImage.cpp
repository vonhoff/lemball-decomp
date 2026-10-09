#include "CResImage.h"

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include <stddef.h>

// SIZE 0x10
struct ImageResourceHeader {
	unsigned int m_width;
	unsigned short m_height;
	unsigned short m_unknown0x06;
	unsigned char m_depth;
	unsigned char m_flags;
	unsigned short m_unknown0x0a;
	unsigned int m_imageState;
};

// FUNCTION: LEMBALL 0x0045e160
CResImage* CResImage::Load(unsigned long p_resourceId)
{
	unsigned long id = p_resourceId;
	CResImage* res = (CResImage*) g_pActiveMogRes->Find(id);
	if (res == NULL) {
		return (CResImage*) (new CResImage(id))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_IMAGE) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e1e0
void CResImage::SetHeader()
{
	ImageResourceHeader* header = (ImageResourceHeader*) m_name;
	unsigned int width = header->m_width;
	unsigned short height = header->m_height;
	m_rasterPoint.m_x = (short) width;
	m_rasterPoint.m_y = (short) height;
	m_depth = header->m_depth;
	m_flags = header->m_flags;
	m_imageState = header->m_imageState;
}

// FUNCTION: LEMBALL 0x0045eb40
void CResImage::SetType()
{
	m_chunkType = RESOURCE_CHUNK_IMAGE;
	m_headerSkip = 0x10;
}
