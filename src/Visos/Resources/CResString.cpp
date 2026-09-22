#include "CResString.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045de00
CResString* CResString::Load(unsigned int p_resourceId)
{
	CResString* res = (CResString*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResString*) (new CResString(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x53545247) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ea70
void CResString::SetType()
{
	m_chunkType = 0x53545247;
}

// FUNCTION: LEMBALL 0x0045eaa0
CResString::CResString()
{
	Initialise();
}
