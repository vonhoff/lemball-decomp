#include "CResMOVIE.h"

#include "CResBaseLIST.h"
#include "CResINT.h"
#include "CResSTRING.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"
#include "Engine/Resources/ResourceTypeList.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045de70
CResMOVIE* CResMOVIE::Load(unsigned long p_resourceId)
{
	CResMOVIE* res = (CResMOVIE*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResMOVIE*) (new CResMOVIE(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_LIST) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045ded0
CResMOVIE::CResMOVIE() : CResBaseLIST(g_pPreloadedResourceTypes)
{
	m_movieEntries = NULL;
	m_fontEntries = NULL;
	Initialise();
	m_initialised = false;
}

// FUNCTION: LEMBALL 0x0045df20
CResMOVIE::CResMOVIE(unsigned long p_resourceId) : CResBaseLIST(g_pPreloadedResourceTypes)
{
	m_movieEntries = NULL;
	m_fontEntries = NULL;
	DoLoad(p_resourceId);
	m_initialised = false;
}

// FUNCTION: LEMBALL 0x0045df70
CResMOVIE::~CResMOVIE()
{
	if (m_movieEntries != NULL) {
		delete[] m_movieEntries;
	}
	if (m_fontEntries != NULL) {
		delete[] m_fontEntries;
	}
}

// FUNCTION: LEMBALL 0x0045dfa0
void CResMOVIE::AllocateResources(unsigned long p_count)
{
	m_movieEntries = new CResSTRING[p_count];
	m_fontEntries = new CResINT[p_count];
}

// FUNCTION: LEMBALL 0x0045e060
bool CResMOVIE::DirectResources(unsigned long p_index, unsigned char*& p_headerCursor, unsigned char*& p_dataCursor)
{
	bool failed = m_movieEntries[p_index].Direct(p_headerCursor, p_dataCursor, this) != 0;
	if (!failed) {
		if (!m_fontEntries[p_index].Direct(p_headerCursor, p_dataCursor, this)) {
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045e0c0
bool CResMOVIE::DirectResources(unsigned long p_index, unsigned char*& p_cursor)
{
	bool failed = m_movieEntries[p_index].Direct(p_cursor, this) != 0;
	if (!failed) {
		if (m_fontEntries[p_index].Direct(p_cursor, this) == 0) {
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045e120
void CResMOVIE::UnLoadResources(unsigned long p_index, unsigned int p_force)
{
	m_movieEntries[p_index].UnLoadExtData(p_force);
	m_fontEntries[p_index].UnLoadExtData(p_force);
}
