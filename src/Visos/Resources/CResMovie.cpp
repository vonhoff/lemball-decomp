#include "CResMovie.h"

#include "CMogRes.h"
#include "ResourceTypeList.h"
#include "Visos/Resources/CResBaseList.h"
#include "Visos/Resources/CResInt.h"
#include "Visos/Resources/CResString.h"

// FUNCTION: LEMBALL 0x0045de70
CResMovie* CResMovie::Load(unsigned int p_resourceId)
{
	CResMovie* res = (CResMovie*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResMovie*) (new CResMovie(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ded0
CResMovie::CResMovie() : CResBaseList((ResListHeader*) g_pPreloadedResourceTypes), m_movieEntries(0), m_fontEntries(0)
{
	Initialise();
	m_initialized = 0;
}

// FUNCTION: LEMBALL 0x0045df20
CResMovie::CResMovie(unsigned long p_resourceId)
	: CResBaseList((ResListHeader*) g_pPreloadedResourceTypes), m_movieEntries(0), m_fontEntries(0)
{
	DoLoad(p_resourceId);
	m_initialized = 0;
}

// FUNCTION: LEMBALL 0x0045df70
CResMovie::~CResMovie()
{
	if (m_movieEntries != 0) {
		delete[] m_movieEntries;
	}
	if (m_fontEntries != 0) {
		delete[] m_fontEntries;
	}
}

// FUNCTION: LEMBALL 0x0045dfa0
void CResMovie::AllocateResources(unsigned int p_count)
{
	m_movieEntries = new CResString[p_count];
	m_fontEntries = new CResInt[p_count];
}

// FUNCTION: LEMBALL 0x0045e060
bool CResMovie::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	int direct = (unsigned int) m_movieEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this) >= 1;
	if (direct == 0) {
		if (!m_fontEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this)) {
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0045e0c0
bool CResMovie::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	int direct = (unsigned int) m_movieEntries[p_index].Direct(*p_cursor, this) >= 1;
	if (direct == 0) {
		if (m_fontEntries[p_index].Direct(*p_cursor, this) == 0) {
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0045e120
void CResMovie::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_movieEntries[p_index].UnLoadExtData(p_force);
	m_fontEntries[p_index].UnLoadExtData(p_force);
}
