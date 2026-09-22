#include "CResBool.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045dc40
CResBool* CResBool::Load(unsigned int p_resourceId)
{
	CResBool* res = (CResBool*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResBool*) (new CResBool(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x424f4f4c) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e9b0
void CResBool::SetType()
{
	m_chunkType = 0x424f4f4c;
}

// FUNCTION: LEMBALL 0x0045e9c0
void CResBool::OnLoad()
{
	m_value = *(unsigned int*) m_data;
}
