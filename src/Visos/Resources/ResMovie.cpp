#include "ResMovie.h"

#include "MogRes.h"
#include "ResourceTypeList.h"

// 68K 0x10204158 Load__9CResMOVIEFUl
// FUNCTION: LEMBALL 0x0045de70
ResMovie* ResMovie::Load(unsigned int p_resourceId)
{
	ResMovie* res = (ResMovie*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResMovie*) (new ResMovie(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x102041e0 __ct__9CResMOVIEFUl
// STUB: LEMBALL 0x0045df20
ResMovie::ResMovie(unsigned long p_resourceId)
	: ResBaseList((ResListHeader*) g_pPreloadedResourceTypes), m_movieEntries(0), m_fontEntries(0)
{
	DoLoad(p_resourceId);
	m_loaded = 0;
}

// 68K 0x1020425c __dt__9CResMOVIEFv
// FUNCTION: LEMBALL 0x0045df70
ResMovie::~ResMovie()
{
	if (m_movieEntries != 0) {
		delete[] m_movieEntries;
	}
	if (m_fontEntries != 0) {
		delete[] m_fontEntries;
	}
}

// 68K 0x102042e6 AllocateResources__9CResMOVIEFUl
// FUNCTION: LEMBALL 0x0045dfa0
void ResMovie::AllocateResources(unsigned int p_count)
{
	m_movieEntries = new ResString[p_count];
	m_fontEntries = new ResInt[p_count];
}

// 68K 0x10204396 DirectResources__9CResMOVIEFUlRPUcRPUc
// FUNCTION: LEMBALL 0x0045e060
bool ResMovie::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	if (!m_movieEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this)) {
		if (!m_fontEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this)) {
			return 0;
		}
	}
	return 1;
}

// 68K 0x1020443c DirectResources__9CResMOVIEFUlRPUc
// FUNCTION: LEMBALL 0x0045e0c0
bool ResMovie::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	if (m_movieEntries[p_index].Direct(*p_cursor, this) == 0 && m_fontEntries[p_index].Direct(*p_cursor, this) == 0) {
		return 0;
	}
	return 1;
}

// 68K 0x102044d6 UnLoadResources__9CResMOVIEFUlUc
// FUNCTION: LEMBALL 0x0045e120
void ResMovie::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_movieEntries[p_index].UnLoadExtData(p_force);
	m_fontEntries[p_index].UnLoadExtData(p_force);
}
