#include "CResBase.h"

#include "../Foundation/CVsRange.h"
#include "CMogRes.h"
#include "CResBaseList.h"

#include <string.h>

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
	m_error = 1;
}

// FUNCTION: LEMBALL 0x0045cf70
bool CResBase::Direct(unsigned char*& p_cursor, CResBaseList* p_list)
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
	return 0;
}

// FUNCTION: LEMBALL 0x0045cfb0
bool CResBase::Direct(unsigned char*& p_headerCursor, unsigned char*& p_dataCursor, CResBaseList* p_list)
{
	unsigned int* entry;

	m_externalList = p_list;
	entry = (unsigned int*) p_headerCursor;
	if (m_chunkType != entry[0]) {
		m_error = 1;
		return 1;
	}
	m_dataSize = entry[1];
	m_name = (char*) &entry[2];
	SetHeader();
	p_headerCursor = (unsigned char*) &entry[2] + m_headerSkip;
	m_data = p_dataCursor;
	p_dataCursor += m_dataSize;
	++m_referenceCount;
	++m_directUseCount;
	m_age = 0;
	m_loaded = 1;
	m_age = 0;
	OnLoad();
	return 0;
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
	m_name = 0;
	m_data = 0;
	m_externalList = 0;
	m_headerSkip = 0;
	m_chunkType = 0;
	m_resourceId = 0;
	m_error = 0;
	SetType();
	g_pActiveMogRes->AgeResources();
	m_age = 0;
}

// FUNCTION: LEMBALL 0x0045d0a0
void CResBase::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size)
{
	if (p_size != 0) {
		if (m_data == 0) {
			m_data = g_pActiveMogRes->AllocateMainMem(p_size);
			memcpy(m_data, p_source, p_size);
		}
	}
	if (m_name != 0) {
		m_loaded = 1;
		OnLoad();
	}
}

// FUNCTION: LEMBALL 0x0045d100
void CResBase::LoadData()
{
	CVsRange range;

	if (m_loaded == 0) {
		if (GetfVramLoaded() == 0) {
			if (m_externalList == 0) {
				if (m_dataSize != 0) {
					range.m_offset = m_fileOffset;
					range.m_size = m_dataSize;
					if (g_pActiveMogRes->Load(range, m_data, this) != 0) {
						OnRead(m_data, &m_data, m_dataSize);
					}
				}
				else {
					m_data = 0;
					OnRead(0, &m_data, m_dataSize);
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
			m_data = 0;
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
		m_data = 0;
		--m_directUseCount;
	}
}

// FUNCTION: LEMBALL 0x0045d250
CResBase* CResBase::CheckError()
{
	switch (m_error) {
	case 1:
		g_pActiveMogRes->Remove(this);
		delete this;
		return 0;
	case 2:
		return 0;
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
	return 0;
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
