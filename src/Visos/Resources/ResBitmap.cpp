#include "ResBitmap.h"

#include "MogRes.h"

// 68K 0x10204542 Load__10CResBITMAPFUl
// FUNCTION: LEMBALL 0x0045e210
ResBitmap* ResBitmap::Load(unsigned int p_resourceId)
{
	ResBitmap* res = (ResBitmap*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResBitmap*) (new ResBitmap(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x42544d50) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x102045f4 SetHeader__10CResBITMAPFv
// FUNCTION: LEMBALL 0x0045e290
void ResBitmap::SetHeader()
{
	BitmapHeader* header = (BitmapHeader*) m_name;
	unsigned short height = header->m_height;
	unsigned int width = header->m_width;
	m_x = (unsigned short) width;
	m_y = height;
	m_depth = header->m_depth;
	m_flags = header->m_flags;
}

// 68K 0x101162c2 SetType__10CResBITMAPFv
// FUNCTION: LEMBALL 0x0045eb70
void ResBitmap::SetType()
{
	m_chunkType = 0x42544d50;
	m_headerSkip = 0xc;
}

// 68K 0x101162f6 __dt__10CResBITMAPFv
// SYNTHETIC: LEMBALL 0x0045eb80
// ResBitmap::`scalar deleting destructor'
ResBitmap::~ResBitmap()
{
}
