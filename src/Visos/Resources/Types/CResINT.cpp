#include "CResINT.h"

#include "Visos/Resources/Archive/CMogRes.h"
#include "Visos/Resources/ResourceChunkTypes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045dcb0
CResINT* CResINT::Load(unsigned int p_resourceId)
{
	CResINT* res = (CResINT*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResINT*) (new CResINT(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_INT) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e8f0
CResINT::CResINT()
{
	Initialise();
}

// FUNCTION: LEMBALL 0x0045e910
void CResINT::SetType()
{
	m_chunkType = RESOURCE_CHUNK_INT;
}

// FUNCTION: LEMBALL 0x0045e920
void CResINT::OnLoad()
{
	m_value = ((IntPayload*) m_data)->m_value;
}
