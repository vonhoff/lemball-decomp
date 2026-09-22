#include "ResBool.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045dc40
ResBool* ResBool::Load(unsigned int p_resourceId)
{
	ResBool* res = (ResBool*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResBool*) (new ResBool(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x424f4f4c) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e9b0
void ResBool::SetType()
{
	m_chunkType = 0x424f4f4c;
}

// FUNCTION: LEMBALL 0x0045e9c0
void ResBool::OnLoad()
{
	m_value = *(unsigned int*) m_data;
}
