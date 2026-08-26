#include "ResBase.h"

#include <string.h>

#include "../Foundation/VsRange.h"
#include "MogRes.h"
#include "ResBaseList.h"

// 68K 0x102029ce DoLoad__8CResBaseFUl
// FUNCTION: LEMBALL 0x0045cf20
void ResBase::DoLoad(unsigned int p_resourceId)
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

// 68K 0x10202a4a Direct__8CResBaseFRPUcP12CResBaseLIST
// FUNCTION: LEMBALL 0x0045cf70
bool ResBase::Direct(unsigned char*& p_cursor, ResBaseList* p_list)
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

// 68K 0x10202adc Direct__8CResBaseFRPUcRPUcP12CResBaseLIST
// FUNCTION: LEMBALL 0x0045cfb0
bool ResBase::Direct(unsigned char*& p_headerCursor, unsigned char*& p_dataCursor, ResBaseList* p_list)
{
	unsigned int* entry;

	m_externalList = p_list;
	entry = (unsigned int*) p_headerCursor;
	if (m_chunkType != entry[0]) {
		m_error = 1;
		return 1;
	}
	m_dataSize = entry[1];
	m_name = (char*) (entry + 2);
	SetHeader();
	p_headerCursor = (unsigned char*) entry + m_headerSkip + 8;
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

// 68K 0x10202bb6 __dt__8CResBaseFv
// STUB: LEMBALL 0x0045d040
ResBase::~ResBase()
{
}

// 68K 0x10202bf8 Initialise__8CResBaseFv
// FUNCTION: LEMBALL 0x0045d050
void ResBase::Initialise()
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

// 68K 0x10202f26 OnRead__8CResBaseFPUcPPUcUl
// FUNCTION: LEMBALL 0x0045d0a0
void ResBase::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size)
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

// 68K 0x10202fae LoadData__8CResBaseFv
// STUB: LEMBALL 0x0045d100
void ResBase::LoadData()
{
	ResBaseList* list;
	unsigned int size;
	VsRange range;

	if (m_loaded == 0) {
		if (GetfVramLoaded() == 0) {
			list = m_externalList;
			if (list == 0) {
				size = m_dataSize;
				if (size != 0) {
					range.m_offset = m_fileOffset;
					range.m_size = size;
					if (g_pActiveMogRes->Load(range, m_data, this) != 0) {
						OnRead(m_data, &m_data, size);
					}
				}
				else {
					m_data = 0;
					OnRead(0, &m_data, size);
				}
			}
			else {
				list->LoadData();
			}
		}
	}
	m_age = 0;
}

// 68K 0x1020308a UnLoad__8CResBaseFv
// FUNCTION: LEMBALL 0x0045d180
void ResBase::UnLoad()
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

// 68K 0x10203108 UnLoadData__8CResBaseFUc
// STUB: LEMBALL 0x0045d1c0
void ResBase::UnLoadData(unsigned int p_force)
{
	unsigned int size;

	if (m_loaded != 0 && m_resourceId != 0 && (size = m_dataSize) != 0) {
		g_pActiveMogRes->DeallocateMem(m_data, 1);
		m_data = 0;
	}
	UnLoadVramData(p_force);
	if (m_loaded != 0) {
		m_loaded = 0;
		OnUnLoad();
	}
}

// 68K 0x10203194 UnLoadExtData__8CResBaseFUc
// FUNCTION: LEMBALL 0x0045d220
void ResBase::UnLoadExtData(unsigned int p_force)
{
	UnLoadVramData(p_force);
	if (m_loaded != 0) {
		m_loaded = 0;
		m_data = 0;
		--m_directUseCount;
	}
}

// 68K 0x102031e8 CheckError__8CResBaseFv
// FUNCTION: LEMBALL 0x0045d250
ResBase* ResBase::CheckError()
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

// 68K 0x10101148 GetfVramLoaded__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e5b0
bool ResBase::GetfVramLoaded()
{
	return m_vramLoaded;
}

// 68K 0x10101176 GetfVramSwappable__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e5c0
bool ResBase::GetfVramSwappable()
{
	return m_vramSwappable;
}

// 68K 0x101011a8 GetfAnyVramLoaded__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e5d0
bool ResBase::GetfAnyVramLoaded()
{
	return m_vramLoaded;
}

// 68K 0x101011da ForceLoadVram__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e5e0
bool ResBase::ForceLoadVram()
{
	return 0;
}

// 68K 0x10101202 UnLoadVramData__8CResBaseFUc
// FUNCTION: LEMBALL 0x0045e5f0
void ResBase::UnLoadVramData(unsigned int p_force)
{
}

// 68K 0x1010122a GetData__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e600
unsigned char* ResBase::GetData()
{
	return m_data;
}

// 68K 0x10101252 OnLoad__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e610
void ResBase::OnLoad()
{
}

// 68K 0x10101270 OnUnLoad__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e620
void ResBase::OnUnLoad()
{
}

// 68K 0x10101126 SetHeader__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e630
void ResBase::SetHeader()
{
}

// 68K 0x10100ec8 SetType__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e640
void ResBase::SetType()
{
	m_chunkType = 0;
}

// 68K 0x10101290 GetSizeUsed__8CResBaseFv
// FUNCTION: LEMBALL 0x0045e650
unsigned int ResBase::GetSizeUsed()
{
	return m_dataSize;
}
