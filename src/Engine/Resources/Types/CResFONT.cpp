#include "CResFONT.h"

#include "Engine/Resources/Types/CFontTable.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/ResourceChunkTypes.h"
#include "Engine/Resources/ResourceTypeList.h"

#include <stddef.h>

#include "Engine/Text/CText.h"

// FUNCTION: LEMBALL 0x0045d760
CResFONT::CResFONT() : CResBaseLIST((ResListHeader*) g_pResourceTypes)
{
	m_animationEntries = NULL;
	m_fontEntries = NULL;
	m_fontTable = NULL;
	Initialise();
	m_initialised = 1;
}

// FUNCTION: LEMBALL 0x0045d7b0
CResFONT::CResFONT(unsigned long p_resourceId) : CResBaseLIST((ResListHeader*) g_pResourceTypes)
{
	m_animationEntries = NULL;
	m_fontEntries = NULL;
	m_fontTable = NULL;
	DoLoad(p_resourceId);
	m_initialised = 1;
}

// FUNCTION: LEMBALL 0x0045d810
CResFONT::~CResFONT()
{
	if (m_animationEntries != NULL) {
		delete[] m_animationEntries;
	}
	if (m_fontEntries != NULL) {
		delete[] m_fontEntries;
	}
	if (m_fontTable != NULL) {
		delete m_fontTable;
	}
}

// FUNCTION: LEMBALL 0x0045d850
CResFONT* CResFONT::Load(unsigned int p_resourceId)
{
	CResFONT* res = (CResFONT*) g_pActiveMogRes->Find(p_resourceId);
	if (res == NULL) {
		return (CResFONT*) (new CResFONT(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != RESOURCE_CHUNK_LIST) {
		res->UnLoad();
		return NULL;
	}
	return res;
}

// FUNCTION: LEMBALL 0x0045d8b0
void CResFONT::AllocateResources(unsigned long p_count)
{
	m_fontEntries = new CResINT[p_count];
	m_animationEntries = new CResZRLE[p_count];
}

// FUNCTION: LEMBALL 0x0045d970
unsigned int CResFONT::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialised != 0) {
		count = 1;
	}
	if (m_fontEntries->m_initialised != 0) {
		count++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x0045d990
bool CResFONT::DirectResources(unsigned long p_index, unsigned char*& p_headerCursor, unsigned char*& p_dataCursor)
{
	bool failed = m_fontEntries[p_index].Direct(p_headerCursor, p_dataCursor, this) != 0;
	if (!failed) {
		if (m_animationEntries[p_index].Direct(p_headerCursor, p_dataCursor, this) == 0) {
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045d9f0
bool CResFONT::DirectResources(unsigned long p_index, unsigned char*& p_cursor)
{
	bool failed = m_fontEntries[p_index].Direct(p_cursor, this) != 0;
	if (!failed) {
		if (m_animationEntries[p_index].Direct(p_cursor, this) == 0) {
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0045da50
void CResFONT::UnLoadResources(unsigned long p_index, unsigned int p_force)
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
	if (m_fontTable == NULL) {
		m_fontTable = new CFontTable(this);
	}
}

// FUNCTION: LEMBALL 0x0045db20
CResZRLE* CResFONT::ASCIItoZRLE(unsigned int p_ascii)
{
	return m_fontTable->GetZRLE(p_ascii);
}

// FUNCTION: LEMBALL 0x0045db30
CVSSize CResFONT::GetSize(const char* p_text, unsigned int p_flags)
{
	int textIndex = 0;
	CVSSize sizeValue;
	CVSSize& size = sizeValue;
	size.m_height = 0;
	size.m_width = 0;
	if (p_text[0] != '\0') {
		do {
			CResZRLE* glyph = ASCIItoZRLE(p_text[textIndex]);
			if (glyph == NULL) {
				glyph = ASCIItoZRLE('I');
				if (glyph == NULL) {
					glyph = m_animationEntries;
				}
			}
			short* glyphDimensions = &glyph->m_width;
			short* glyphOrigin = &glyph->m_x;
			if ((p_flags & TEXT_ADVANCE_HORIZONTAL_MASK) != 0) {
				size.m_width += *glyphDimensions + 1;
			}
			else {
				if (glyphOrigin[0] + *glyphDimensions > size.m_width) {
					size.m_width = glyphOrigin[0] + *glyphDimensions;
				}
			}
			if ((p_flags & TEXT_ADVANCE_VERTICAL_MASK) != 0) {
				size.m_height += glyphDimensions[1] + 1;
			}
			else {
				if (glyphOrigin[1] + glyphDimensions[1] > size.m_height) {
					size.m_height = glyphDimensions[1] + glyphOrigin[1];
				}
			}
			textIndex++;
		} while (p_text[textIndex] != '\0');
	}
	if ((p_flags & TEXT_ADVANCE_HORIZONTAL_MASK) != 0) {
		size.m_width--;
	}
	if ((p_flags & TEXT_ADVANCE_VERTICAL_MASK) != 0) {
		size.m_height--;
	}
	return sizeValue;
}
