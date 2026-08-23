#include "ResZrle.h"

#include "MogRes.h"

// 68K 0x10204650 Load__8CResZRLEFUl
// FUNCTION: LEMBALL 0x0045e2b0
ResZrle* ResZrle::Load(unsigned int p_resourceId)
{
	ResZrle* res = (ResZrle*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		res = new ResZrle();
		if (res != 0) {
			res->DoLoad(p_resourceId);
			return (ResZrle*) res->CheckError();
		}
		return (ResZrle*) ((ResBase*) 0)->CheckError();
	}
	if (res->m_chunkType != 0x5a524c45) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x10204714 SetHeader__8CResZRLEFv
// FUNCTION: LEMBALL 0x0045e340
void ResZrle::SetHeader()
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

// 68K 0x10115f94 __ct__8CResZRLEFv
// FUNCTION: LEMBALL 0x0045e7e0
ResZrle::ResZrle()
	: m_height(0), m_width(0), m_originY(0), m_originX(0)
{
	Initialise();
}

// 68K 0x10116352 SetType__8CResZRLEFv
// FUNCTION: LEMBALL 0x0045e840
void ResZrle::SetType()
{
	m_chunkType = 0x5a524c45;
	m_headerSkip = 0xc;
}

// 68K 0x10115f3c __dt__8CResZRLEFv
// SYNTHETIC: LEMBALL 0x0045e850
// ResZrle::`scalar deleting destructor'
ResZrle::~ResZrle()
{
}
