#include "CResBase.h"

#include "CResBaseLIST.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/Archive/CVSRange.h"

#include <string.h>

// SIZE 0x08
struct ResourceEntryHeader {
	unsigned int m_chunkType;
	unsigned int m_dataSize;
};

enum eResourceCheckDisposition {
	RESOURCE_CHECK_DISCARD_INVALID_RESOURCE = 1,
	RESOURCE_CHECK_PRESERVE_RESOURCE = 2
};

// FUNCTION: LEMBALL 0x0045cf20
void CResBase::DoLoad(unsigned int p_resourceId)
{
	Initialise();
	if (g_pActiveMogRes->Load(p_resourceId, this, 1) != 0) {
		m_resourceId = p_resourceId;
		SetHeader();
		LoadData();
		m_referenceCount = m_referenceCount + 1;
		return;
	}
	m_error = RESOURCE_CHECK_DISCARD_INVALID_RESOURCE;
}

// FUNCTION: LEMBALL 0x0045cf70
bool CResBase::Direct(unsigned char*& p_cursor, CResBaseLIST* p_list)
{
	m_externalList = p_list;
	m_data = p_cursor;
	p_cursor += m_dataSize;
	++m_referenceCount;
	++m_directUseCount;
	m_age = 0;
	m_loaded = 1;
	m_age = 0;
	OnLoad();
	return false;
}

// FUNCTION: LEMBALL 0x0045cfb0
bool CResBase::Direct(unsigned char*& p_headerCursor, unsigned char*& p_dataCursor, CResBaseLIST* p_list)
{
	ResourceEntryHeader* entry;

	m_externalList = p_list;
	entry = (ResourceEntryHeader*) p_headerCursor;
	if (m_chunkType != entry->m_chunkType) {
		m_error = RESOURCE_CHECK_DISCARD_INVALID_RESOURCE;
		return true;
	}
	m_dataSize = entry->m_dataSize;
	m_name = (char*) (entry + 1);
	SetHeader();
	p_headerCursor = (unsigned char*) (entry + 1) + m_headerSkip;
	m_data = p_dataCursor;
	p_dataCursor += m_dataSize;
	++m_referenceCount;
	++m_directUseCount;
	m_age = 0;
	m_loaded = 1;
	m_age = 0;
	OnLoad();
	return false;
}

// FUNCTION: LEMBALL 0x0045d040
CResBase::~CResBase()
{
}

// FUNCTION: LEMBALL 0x0045d050
void CResBase::Initialise()
{
	m_directUseCount = 0;
	m_referenceCount = 0;
	m_vramLoaded = 0;
	m_loaded = 0;
	m_dataSize = 0;
	m_fileOffset = 0;
	m_name = NULL;
	m_data = NULL;
	m_externalList = NULL;
	m_headerSkip = 0;
	m_chunkType = 0;
	m_resourceId = 0;
	m_error = 0;
	SetType();
	g_pActiveMogRes->AgeResources();
	m_age = 0;
}

// FUNCTION: LEMBALL 0x0045d0a0
void CResBase::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned long p_size)
{
	if (p_size != 0) {
		if (m_data == NULL) {
			m_data = g_pActiveMogRes->AllocateMainMem(p_size);
			memcpy(m_data, p_source, p_size);
		}
	}
	if (m_name != NULL) {
		m_loaded = 1;
		OnLoad();
	}
}

// FUNCTION: LEMBALL 0x0045d100
void CResBase::LoadData()
{
	CVSRange range;

	if (m_loaded == 0) {
		if (GetfVramLoaded() == 0) {
			if (m_externalList == NULL) {
				if (m_dataSize != 0) {
					range.m_offset = m_fileOffset;
					range.m_size = m_dataSize;
					if (g_pActiveMogRes->Load(range, m_data, this) != 0) {
						OnRead(m_data, &m_data, m_dataSize);
					}
				}
				else {
					m_data = NULL;
					OnRead(NULL, &m_data, m_dataSize);
				}
			}
			else {
				m_externalList->LoadData();
			}
		}
	}
	m_age = 0;
}

// FUNCTION: LEMBALL 0x0045d180
void CResBase::UnLoad()
{
	if (--m_referenceCount == 0) {
		UnLoadData(1);
		if (g_pActiveMogRes->m_skipCleanup != 0) {
			if (m_resourceId != 0) {
				g_pActiveMogRes->Remove(this);
			}
			delete this;
		}
	}
}

// FUNCTION: LEMBALL 0x0045d1c0
void CResBase::UnLoadData(unsigned int p_force)
{
	unsigned int size;

	if (m_loaded != 0) {
		size = m_dataSize;
		if (m_resourceId != 0 && size != 0) {
			g_pActiveMogRes->DeallocateMem(m_data, 1);
			m_data = NULL;
		}
	}
	UnLoadVramData(p_force);
	if (m_loaded != 0) {
		m_loaded = 0;
		OnUnLoad();
	}
}

// FUNCTION: LEMBALL 0x0045d220
void CResBase::UnLoadExtData(unsigned int p_force)
{
	UnLoadVramData(p_force);
	if (m_loaded != 0) {
		m_loaded = 0;
		m_data = NULL;
		--m_directUseCount;
	}
}

// FUNCTION: LEMBALL 0x0045d250
CResBase* CResBase::CheckError()
{
	switch (m_error) {
	case RESOURCE_CHECK_DISCARD_INVALID_RESOURCE:
		g_pActiveMogRes->Remove(this);
		delete this;
		return NULL;
	case RESOURCE_CHECK_PRESERVE_RESOURCE:
		return NULL;
	default:
		return this;
	}
}

// FUNCTION: LEMBALL 0x0045e5b0
bool CResBase::GetfVramLoaded()
{
	return m_vramLoaded;
}

// FUNCTION: LEMBALL 0x0045e5c0
bool CResBase::GetfVramSwappable()
{
	return m_vramSwappable;
}

// FUNCTION: LEMBALL 0x0045e5d0
bool CResBase::GetfAnyVramLoaded()
{
	return m_vramLoaded;
}

// FUNCTION: LEMBALL 0x0045e5e0
bool CResBase::ForceLoadVram()
{
	return false;
}

// FUNCTION: LEMBALL 0x0045e5f0
void CResBase::UnLoadVramData(unsigned int p_force)
{
}

// FUNCTION: LEMBALL 0x0045e600
unsigned char* CResBase::GetData()
{
	return m_data;
}

// FUNCTION: LEMBALL 0x0045e610
void CResBase::OnLoad()
{
}

// FUNCTION: LEMBALL 0x0045e620
void CResBase::OnUnLoad()
{
}

// FUNCTION: LEMBALL 0x0045e630
void CResBase::SetHeader()
{
}

// FUNCTION: LEMBALL 0x0045e640
void CResBase::SetType()
{
	m_chunkType = 0;
}

// FUNCTION: LEMBALL 0x0045e650
unsigned int CResBase::GetSizeUsed()
{
	return m_dataSize;
}
