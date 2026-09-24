#include "CResFONT.h"

#include "CFontTable.h"
#include "CMogRes.h"
#include "ResourceTypeList.h"

// FUNCTION: LEMBALL 0x0045d7b0
CResFONT::CResFONT(unsigned long p_resourceId) : CResBaseLIST((ResListHeader*) g_pResourceTypes)
{
	m_animationEntries = 0;
	m_fontEntries = 0;
	m_fontTable = 0;
	DoLoad(p_resourceId);
	m_initialized = 1;
}

// FUNCTION: LEMBALL 0x0045d810
CResFONT::~CResFONT()
{
	if (m_animationEntries != 0) {
		delete[] m_animationEntries;
	}
	if (m_fontEntries != 0) {
		delete[] m_fontEntries;
	}
	if (m_fontTable != 0) {
		delete m_fontTable;
	}
}

// FUNCTION: LEMBALL 0x0045d850
CResFONT* CResFONT::Load(unsigned int p_resourceId)
{
	CResFONT* res = (CResFONT*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (CResFONT*) (new CResFONT(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045d8b0
void CResFONT::AllocateResources(unsigned int p_count)
{
	m_fontEntries = new CResINT[p_count];
	m_animationEntries = new CResZRLE[p_count];
}

// FUNCTION: LEMBALL 0x0045d970
unsigned int CResFONT::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialized != 0) {
		count = 1;
	}
	if (m_fontEntries->m_initialized != 0) {
		count++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0045d990
bool CResFONT::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	int direct = (unsigned int) m_fontEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this) >= 1;
	if (direct == 0) {
		if (m_animationEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this) == 0) {
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0045d9f0
bool CResFONT::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	int direct = (unsigned int) m_fontEntries[p_index].Direct(*p_cursor, this) >= 1;
	if (direct == 0) {
		if (m_animationEntries[p_index].Direct(*p_cursor, this) == 0) {
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0045da50
void CResFONT::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_fontEntries[p_index].UnLoadExtData(p_force);
	m_animationEntries[p_index].UnLoadExtData(p_force);
}

// FUNCTION: LEMBALL 0x0045da90
void CResFONT::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadVramData(p_force);
}

// FUNCTION: LEMBALL 0x0045dab0
bool CResFONT::ForceLoadVram(unsigned int p_index)
{
	if (!m_animationEntries[p_index].GetfVramLoaded()) {
		return m_animationEntries[p_index].GetfVramLoaded();
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045daf0
void CResFONT::OnLoad()
{
	if (m_fontTable == 0) {
		m_fontTable = new CFontTable(this);
	}
}

// FUNCTION: LEMBALL 0x0045db20
CResZRLE* CResFONT::ASCIItoZRLE(unsigned int p_ascii)
{
	return m_fontTable->GetZRLE(p_ascii);
}
