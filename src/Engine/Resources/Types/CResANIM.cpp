#include "CResANIM.h"
#include "Engine/Resources/ResourceChunkTypes.h"

#include "Engine/Resources/Archive/CMogRes.h"
#include "CResZRLE.h"

// FUNCTION: LEMBALL 0x0045d610
CResANIM* CResANIM::Load(unsigned long p_resourceId)
{
	CResANIM* res = (CResANIM*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResANIM*) (new CResANIM(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_LIST) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045d6a0
void CResANIM::AllocateResources(unsigned long p_count)
{
	m_animationEntries = new CResZRLE[p_count];
}

// FUNCTION: LEMBALL 0x0045d700
void CResANIM::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadVramData(p_force);
}

// FUNCTION: LEMBALL 0x0045d720
bool CResANIM::ForceLoadVram(unsigned int p_index)
{
	if (!m_animationEntries[p_index].GetfVramLoaded()) {
		return m_animationEntries[p_index].GetfVramLoaded();
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045e720
unsigned int CResANIM::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialised != 0) {
		count = 1;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0045e730
bool CResANIM::DirectResources(unsigned long p_index, unsigned char*& p_cursor)
{
	return m_animationEntries[p_index].Direct(p_cursor, this);
}

// FUNCTION: LEMBALL 0x0045e750
bool CResANIM::DirectResources(unsigned long p_index, unsigned char*& p_headerCursor, unsigned char*& p_dataCursor)
{
	return m_animationEntries[p_index].Direct(p_headerCursor, p_dataCursor, this);
}

// FUNCTION: LEMBALL 0x0045e780
void CResANIM::UnLoadResources(unsigned long p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadExtData(p_force);
}
