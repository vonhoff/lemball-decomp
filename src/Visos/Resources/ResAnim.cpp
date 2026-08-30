#include "ResAnim.h"

#include "MogRes.h"

// 68K 0x1020374e Load__8CResANIMFUl
// FUNCTION: LEMBALL 0x0045d610
ResAnim* ResAnim::Load(unsigned int p_resourceId)
{
	register unsigned int id = p_resourceId;
	ResAnim* res = (ResAnim*) g_pActiveMogRes->Find(id);
	if (res == 0) {
		return (ResAnim*) (new ResAnim(id))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x10203818 AllocateResources__8CResANIMFUl
// FUNCTION: LEMBALL 0x0045d6a0
void ResAnim::AllocateResources(unsigned int p_count)
{
	m_animationEntries = new ResZrle[p_count];
}

// 68K 0x1020388c UnLoadVramData__8CResANIMFUlUc
// FUNCTION: LEMBALL 0x0045d700
void ResAnim::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadVramData(p_force);
}

// 68K 0x102038da ForceLoadVram__8CResANIMFUl
// FUNCTION: LEMBALL 0x0045d720
bool ResAnim::ForceLoadVram(unsigned int p_index)
{
	if (!m_animationEntries[p_index].GetfVramLoaded()) {
		return m_animationEntries[p_index].GetfVramLoaded();
	}
	return true;
}

// 68K 0x101160fa GetnVramEntries__8CResANIMFv
// FUNCTION: LEMBALL 0x0045e720
unsigned int ResAnim::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialized != 0) {
		count = 1;
	}
	return count;
}

// 68K 0x10116134 DirectResources__8CResANIMFUlRPUc
// FUNCTION: LEMBALL 0x0045e730
bool ResAnim::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	return m_animationEntries[p_index].Direct(*p_cursor, this);
}

// 68K 0x10116182 DirectResources__8CResANIMFUlRPUcRPUc
// FUNCTION: LEMBALL 0x0045e750
bool ResAnim::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	return m_animationEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this);
}

// 68K 0x101161d8 UnLoadResources__8CResANIMFUlUc
// FUNCTION: LEMBALL 0x0045e780
void ResAnim::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadExtData(p_force);
}
