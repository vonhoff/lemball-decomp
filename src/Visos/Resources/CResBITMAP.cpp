#include "CResBITMAP.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045e210
CResBITMAP* CResBITMAP::Load(unsigned int p_resourceId)
{
	CResBITMAP* res = (CResBITMAP*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		res = new CResBITMAP(p_resourceId);
		return (CResBITMAP*) res->CheckError();
	}
	if (res->m_chunkType != 0x42544d50) {
		res->UnLoad();
		return 0;
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
	m_chunkType = 0x42544d50;
	m_headerSkip = 0xc;
}
