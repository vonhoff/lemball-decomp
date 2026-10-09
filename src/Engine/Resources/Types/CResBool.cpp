#include "CResBool.h"

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045dc40
CResBool* CResBool::Load(unsigned long p_resourceId)
{
	CResBool* res = (CResBool*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResBool*) (new CResBool(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_BOOL) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045e9b0
void CResBool::SetType()
{
	m_chunkType = RESOURCE_CHUNK_BOOL;
}

// FUNCTION: LEMBALL 0x0045e9c0
void CResBool::OnLoad()
{
	m_value = *(unsigned int*) m_data;
}
