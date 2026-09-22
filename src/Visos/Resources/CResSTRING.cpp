#include "CResSTRING.h"

#include "CMogRes.h"

// FUNCTION: LEMBALL 0x0045de00
CResSTRING* CResSTRING::Load(unsigned int p_resourceId)
{
	CResSTRING* res = (CResSTRING*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResSTRING*) (new CResSTRING(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x53545247) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ea70
void CResSTRING::SetType()
{
	m_chunkType = 0x53545247;
}

// FUNCTION: LEMBALL 0x0045eaa0
CResSTRING::CResSTRING()
{
	Initialise();
}
