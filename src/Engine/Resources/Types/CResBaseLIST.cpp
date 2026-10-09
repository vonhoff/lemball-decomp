#include "CResBaseLIST.h"

#include "Engine/Resources/ResourceChunkTypes.h"

#include <string.h>

#pragma intrinsic(memcpy)

#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/Archive/CVSRange.h"

enum {
	VRAM_ENTRIES_NOT_TRACKED = -1
};

#define RESOURCE_LIST_HEADER_UNSET 0xffffffff

// FUNCTION: LEMBALL 0x0045d290
void CResBaseLIST::SetHeader()
{
	ResListHeader* header = (ResListHeader*) m_name;
	m_totalSize = header->m_totalSize;
	m_headerSize = header->m_headerSize;
	m_bodySize = header->m_bodySize;
	m_vramEntryCount = VRAM_ENTRIES_NOT_TRACKED;
}

// FUNCTION: LEMBALL 0x0045d2b0
void CResBaseLIST::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned long p_size)
{
	if (p_data == &m_headerData) {
		if (m_headerData == NULL) {
			m_headerData = g_pActiveMogRes->AllocateMainMem(p_size);
			memcpy(m_headerData, p_source, p_size);
		}
		m_headerLoaded = true;
	}
	else {
		if (m_data == NULL) {
			m_data = g_pActiveMogRes->AllocateMainMem(p_size);
			memcpy(m_data, p_source, p_size);
		}
		m_bodyLoaded = true;
	}
	if (m_loaded == 0 && m_bodyLoaded != 0 && m_headerLoaded != 0) {
		unsigned int count;
		unsigned int directed;
		unsigned char* dataCursor;
		count = m_totalSize / m_listHeader->m_capacity;
		directed = 0;
		dataCursor = m_data;
		if (m_vramReady == 0) {
			unsigned char* headerCursor = m_headerData;
			m_vramEntryCount = GetnVramEntries() * count;
			if (m_vramEntryCount == 0) {
				m_vramEntryCount = VRAM_ENTRIES_NOT_TRACKED;
			}
			for (unsigned int i = 0; i < count; i++) {
				if (DirectResources(i, headerCursor, dataCursor) != 0 || directed != 0) {
					directed = 1;
				}
				else {
					directed = 0;
				}
			}
			g_pActiveMogRes->DeallocateMem(m_headerData, 1);
			m_headerData = NULL;
			m_vramReady = true;
		}
		else {
			for (unsigned int i = 0; i < count; i++) {
				if (DirectResources(i, dataCursor) != 0 || directed != 0) {
					directed = 1;
				}
				else {
					directed = 0;
				}
			}
		}
		m_loaded = true;
		OnLoad();
		m_listHeader->m_currentIndex = RESOURCE_LIST_HEADER_UNSET;
	}
}

// FUNCTION: LEMBALL 0x0045d430
void CResBaseLIST::LoadData()
{
	if (m_loaded == 0) {
		if (!GetfVramLoaded()) {
			unsigned int headerTotal = m_listHeader->m_currentIndex;
			unsigned int count;
			if (headerTotal != RESOURCE_LIST_HEADER_UNSET && m_totalSize != headerTotal) {
				return;
			}
			count = m_totalSize / m_listHeader->m_capacity;
			if (m_vramReady == 0) {
				AllocateResources(count);
				CVSRange headerRange;
				headerRange.m_offset = m_fileOffset;
				headerRange.m_size = m_headerSize;
				if (g_pActiveMogRes->Load(headerRange, m_headerData, this)) {
					OnRead(m_headerData, &m_headerData, m_headerSize);
				}
			}
			CVSRange bodyRange;
			bodyRange.m_offset = m_fileOffset + m_headerSize;
			bodyRange.m_size = m_bodySize;
			if (g_pActiveMogRes->Load(bodyRange, m_data, this)) {
				OnRead(m_data, &m_data, m_headerSize);
			}
		}
	}
	m_age = 0;
}

// FUNCTION: LEMBALL 0x0045d4f0
bool CResBaseLIST::ForceLoadVram()
{
	if (!GetfVramLoaded()) {
		unsigned int i = 0;
		if (m_totalSize / m_listHeader->m_capacity != 0) {
			do {
				if (!ForceLoadVram(i)) {
					return false;
				}
				i++;
			} while (i < m_totalSize / m_listHeader->m_capacity);
		}
	}
	return GetfVramLoaded();
}

// FUNCTION: LEMBALL 0x0045d540
void CResBaseLIST::UnLoadData(unsigned int p_force)
{
	if (m_loaded == 0) {
		if (!GetfAnyVramLoaded()) {
			return;
		}
		if (m_loaded == 0) {
			goto unload_entries;
		}
	}
	m_loaded = false;
	g_pActiveMogRes->DeallocateMem(m_data, 1);
	m_data = NULL;
unload_entries:
	unsigned int i = 0;
	if (m_totalSize / m_listHeader->m_capacity != 0) {
		do {
			UnLoadResources(i, p_force);
			i++;
		} while (i < m_totalSize / m_listHeader->m_capacity);
	}
	OnUnLoad();
}

// FUNCTION: LEMBALL 0x0045d5c0
void CResBaseLIST::UnLoadVramData(unsigned int p_force)
{
	if (GetfAnyVramLoaded()) {
		for (unsigned int i = 0; i < m_totalSize / m_listHeader->m_capacity; i++) {
			UnLoadVramData(i, p_force);
		}
	}
}

// FUNCTION: LEMBALL 0x0045e680
void CResBaseLIST::SetType()
{
	m_chunkType = RESOURCE_CHUNK_LIST;
	m_headerSkip = 0xc;
}

// FUNCTION: LEMBALL 0x0045e690
unsigned int CResBaseLIST::GetSizeUsed()
{
	return m_bodySize;
}

// FUNCTION: LEMBALL 0x0045e6a0
bool CResBaseLIST::GetfVramLoaded()
{
	return !(m_vramLoadedCount - m_vramEntryCount);
}

// FUNCTION: LEMBALL 0x0045e6b0
bool CResBaseLIST::GetfAnyVramLoaded()
{
	return m_vramLoadedCount >= 1;
}

// FUNCTION: LEMBALL 0x0045e6c0
bool CResBaseLIST::GetfVramSwappable()
{
	return m_vramSwappable >= 1;
}

// FUNCTION: LEMBALL 0x0045e6d0
unsigned int CResBaseLIST::GetnVramEntries()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0045e6e0
void CResBaseLIST::UnLoadVramData(unsigned long p_index, unsigned int p_force)
{
}

// FUNCTION: LEMBALL 0x0045e6f0
bool CResBaseLIST::ForceLoadVram(unsigned int p_index)
{
	return false;
}
