#include "CResMOVIE.h"

#include "CMogRes.h"
#include "ResourceTypeList.h"
#include "Visos/Resources/CResBaseLIST.h"
#include "Visos/Resources/CResINT.h"
#include "Visos/Resources/CResSTRING.h"

// FUNCTION: LEMBALL 0x0045de70
CResMOVIE* CResMOVIE::Load(unsigned int p_resourceId)
{
	CResMOVIE* res = (CResMOVIE*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResMOVIE*) (new CResMOVIE(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ded0
CResMOVIE::CResMOVIE() : CResBaseLIST((ResListHeader*) g_pPreloadedResourceTypes), m_movieEntries(0), m_fontEntries(0)
{
	Initialise();
	m_initialized = 0;
}

// FUNCTION: LEMBALL 0x0045df20
CResMOVIE::CResMOVIE(unsigned long p_resourceId)
	: CResBaseLIST((ResListHeader*) g_pPreloadedResourceTypes), m_movieEntries(0), m_fontEntries(0)
{
	DoLoad(p_resourceId);
	m_initialized = 0;
}

// FUNCTION: LEMBALL 0x0045df70
CResMOVIE::~CResMOVIE()
{
	if (m_movieEntries != 0) {
		delete[] m_movieEntries;
	}
	if (m_fontEntries != 0) {
		delete[] m_fontEntries;
	}
}

// FUNCTION: LEMBALL 0x0045dfa0
void CResMOVIE::AllocateResources(unsigned int p_count)
{
	m_movieEntries = new CResSTRING[p_count];
	m_fontEntries = new CResINT[p_count];
}

// FUNCTION: LEMBALL 0x0045e060
bool CResMOVIE::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
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
bool CResMOVIE::DirectResources(unsigned int p_index, unsigned char** p_cursor)
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
void CResMOVIE::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_movieEntries[p_index].UnLoadExtData(p_force);
	m_fontEntries[p_index].UnLoadExtData(p_force);
}
