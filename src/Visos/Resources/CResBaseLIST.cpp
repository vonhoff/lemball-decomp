#include "CResBaseLIST.h"

#include <string.h>

#pragma intrinsic(memcpy)

#include "../Foundation/CVsRange.h"
#include "CMogRes.h"

#define kListHeaderUnset 0xffffffff
#define kHeaderDataOffset 0x4c

// FUNCTION: LEMBALL 0x0045d290
void CResBaseLIST::SetHeader()
{
	ResListHeader* header = (ResListHeader*) m_name;
	m_totalSize = header->m_totalSize;
	m_headerSize = header->m_headerSize;
	m_vramEntryCount = -1;
	m_bodySize = header->m_bodySize;
}

// FUNCTION: LEMBALL 0x0045d2b0
void CResBaseLIST::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size)
{
	if ((int) this - (int) p_data == -kHeaderDataOffset) {
		if (m_headerData == 0) {
			m_headerData = g_pActiveMogRes->AllocateMainMem(p_size);
			memcpy(m_headerData, p_source, p_size);
		}
		m_headerLoaded = 1;
	}
	else {
		if (m_data == 0) {
			m_data = g_pActiveMogRes->AllocateMainMem(p_size);
			memcpy(m_data, p_source, p_size);
		}
		m_bodyLoaded = 1;
	}
	if (m_loaded == 0 && m_bodyLoaded != 0 && m_headerLoaded != 0) {
		unsigned int count;
		unsigned int directed;
		unsigned char* dataCursor;
		count = m_totalSize / m_listHeader->m_headerSize;
		directed = 0;
		dataCursor = m_data;
		if (m_vramReady == 0) {
			unsigned char* headerCursor = m_headerData;
			m_vramEntryCount = GetnVramEntries() * count;
			if (m_vramEntryCount == 0) {
				m_vramEntryCount = -1;
			}
			for (unsigned int i = 0; i < count; i++) {
				if (DirectResources(i, &headerCursor, &dataCursor) != 0 || directed != 0) {
					directed = 1;
				}
				else {
					directed = 0;
				}
			}
			g_pActiveMogRes->DeallocateMem(m_headerData, 1);
			m_headerData = 0;
			m_vramReady = 1;
		}
		else {
			for (unsigned int i = 0; i < count; i++) {
				if (DirectResources(i, &dataCursor) != 0 || directed != 0) {
					directed = 1;
				}
				else {
					directed = 0;
				}
			}
		}
		m_loaded = 1;
		OnLoad();
		m_listHeader->m_totalSize = kListHeaderUnset;
	}
}

// FUNCTION: LEMBALL 0x0045d430
void CResBaseLIST::LoadData()
{
	if (m_loaded == 0) {
		if (!GetfVramLoaded()) {
			unsigned int headerTotal = m_listHeader->m_totalSize;
			unsigned int count;
			if (headerTotal != kListHeaderUnset && m_totalSize != headerTotal) {
				return;
			}
			count = m_totalSize / m_listHeader->m_headerSize;
			if (m_vramReady == 0) {
				AllocateResources(count);
				CVsRange headerRange;
				headerRange.m_offset = m_fileOffset;
				headerRange.m_size = m_headerSize;
				if (g_pActiveMogRes->Load(headerRange, m_headerData, this)) {
					OnRead(m_headerData, &m_headerData, m_headerSize);
				}
			}
			CVsRange bodyRange;
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
		if (m_totalSize / m_listHeader->m_headerSize != 0) {
			do {
				if (!ForceLoadVram(i)) {
					return 0;
				}
				i++;
			} while (i < m_totalSize / m_listHeader->m_headerSize);
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
	m_loaded = 0;
	g_pActiveMogRes->DeallocateMem(m_data, 1);
	m_data = 0;
unload_entries:
	unsigned int i = 0;
	if (m_totalSize / m_listHeader->m_headerSize != 0) {
		do {
			UnLoadResources(i, p_force);
			i++;
		} while (i < m_totalSize / m_listHeader->m_headerSize);
	}
	OnUnLoad();
}

// FUNCTION: LEMBALL 0x0045d5c0
void CResBaseLIST::UnLoadVramData(unsigned int p_force)
{
	if (GetfAnyVramLoaded()) {
		for (unsigned int i = 0; i < m_totalSize / m_listHeader->m_headerSize; i++) {
			UnLoadVramData(i, p_force);
		}
	}
}

// FUNCTION: LEMBALL 0x0045e680
void CResBaseLIST::SetType()
{
	m_chunkType = 0x4c495354;
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
void CResBaseLIST::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
}

// FUNCTION: LEMBALL 0x0045e6f0
bool CResBaseLIST::ForceLoadVram(unsigned int p_index)
{
	return 0;
}
