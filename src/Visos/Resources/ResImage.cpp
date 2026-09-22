#include "ResImage.h"

#include "CMogRes.h"

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
ResImage* ResImage::Load(unsigned int p_resourceId)
{
	register unsigned int id = p_resourceId;
	ResImage* res = (ResImage*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		return (ResImage*) (new ResImage(id))->CheckError();
	}
	if (res->m_chunkType != 0x494d4147) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e1e0
void ResImage::SetHeader()
{
	ImageResourceHeader* header = (ImageResourceHeader*) m_name;
	unsigned int width = header->m_width;
	unsigned short height = header->m_height;
	m_x = (short) width;
	m_y = (short) height;
	m_depth = header->m_depth;
	m_flags = header->m_flags;
	m_imageState = header->m_imageState;
}

// FUNCTION: LEMBALL 0x0045eb40
void ResImage::SetType()
{
	m_chunkType = 0x494d4147;
	m_headerSkip = 0x10;
}
