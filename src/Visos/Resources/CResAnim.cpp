#include "CResAnim.h"

#include "CMogRes.h"
#include "Visos/Resources/CResZrle.h"

// FUNCTION: LEMBALL 0x0045d610
CResAnim* CResAnim::Load(unsigned long p_resourceId)
{
	CResAnim* res = (CResAnim*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResAnim*) (new CResAnim(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045d6a0
void CResAnim::AllocateResources(unsigned int p_count)
{
	m_animationEntries = new CResZrle[p_count];
}

// FUNCTION: LEMBALL 0x0045d700
void CResAnim::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadVramData(p_force);
}

// FUNCTION: LEMBALL 0x0045d720
bool CResAnim::ForceLoadVram(unsigned int p_index)
{
	if (!m_animationEntries[p_index].GetfVramLoaded()) {
		return m_animationEntries[p_index].GetfVramLoaded();
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045e720
unsigned int CResAnim::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialized != 0) {
		count = 1;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0045e730
bool CResAnim::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	return m_animationEntries[p_index].Direct(*p_cursor, this);
}

// FUNCTION: LEMBALL 0x0045e750
bool CResAnim::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	return m_animationEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this);
}

// FUNCTION: LEMBALL 0x0045e780
void CResAnim::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadExtData(p_force);
}
