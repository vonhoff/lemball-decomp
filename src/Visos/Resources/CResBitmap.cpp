#include "CResBitmap.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045e210
CResBitmap* CResBitmap::Load(unsigned int p_resourceId)
{
	CResBitmap* res = (CResBitmap*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		res = new CResBitmap(p_resourceId);
		return (CResBitmap*) res->CheckError();
	}
	if (res->m_chunkType != 0x42544d50) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e290
void CResBitmap::SetHeader()
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
void CResBitmap::SetType()
{
	m_chunkType = 0x42544d50;
	m_headerSkip = 0xc;
}
