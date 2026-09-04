#include "ResBaseList.h"

#include <string.h>

#pragma intrinsic(memcpy)

#include "../Foundation/VsRange.h"
#include "MogRes.h"

#define kListHeaderUnset 0xffffffff
#define kHeaderDataOffset 0x4c

// 68K 0x10203254 SetHeader__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045d290
void ResBaseList::SetHeader()
{
	ResListHeader* header = (ResListHeader*) m_name;
	m_totalSize = header->m_totalSize;
	m_headerSize = header->m_headerSize;
	m_vramEntryCount = -1;
	m_bodySize = header->m_bodySize;
}

// 68K 0x10203298 OnRead__12CResBaseLISTFPUcPPUcUl
// FUNCTION: LEMBALL 0x0045d2b0
void ResBaseList::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size)
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

// 68K 0x1020345a LoadData__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045d430
void ResBaseList::LoadData()
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
				VsRange headerRange;
				headerRange.m_offset = m_fileOffset;
				headerRange.m_size = m_headerSize;
				if (g_pActiveMogRes->Load(headerRange, m_headerData, this)) {
					OnRead(m_headerData, &m_headerData, m_headerSize);
				}
			}
			VsRange bodyRange;
			bodyRange.m_offset = m_fileOffset + m_headerSize;
			bodyRange.m_size = m_bodySize;
			if (g_pActiveMogRes->Load(bodyRange, m_data, this)) {
				OnRead(m_data, &m_data, m_headerSize);
			}
		}
	}
	m_age = 0;
}

// 68K 0x10203586 ForceLoadVram__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045d4f0
bool ResBaseList::ForceLoadVram()
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

// 68K 0x10203616 UnLoadData__12CResBaseLISTFUc
// FUNCTION: LEMBALL 0x0045d540
void ResBaseList::UnLoadData(unsigned int p_force)
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

// 68K 0x102036cc UnLoadVramData__12CResBaseLISTFUc
// FUNCTION: LEMBALL 0x0045d5c0
void ResBaseList::UnLoadVramData(unsigned int p_force)
{
	if (GetfAnyVramLoaded()) {
		unsigned int i = 0;
		unsigned int count = m_totalSize / m_listHeader->m_headerSize;
		if (count != 0) {
			do {
				UnLoadVramData(i, p_force);
				i++;
			} while (i < m_totalSize / m_listHeader->m_headerSize);
		}
	}
}

// 68K 0x1011621e SetType__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045e680
void ResBaseList::SetType()
{
	m_chunkType = 0x4c495354;
	m_headerSkip = 0xc;
}

// 68K 0x10116254 GetSizeUsed__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045e690
unsigned int ResBaseList::GetSizeUsed()
{
	return m_bodySize;
}

// 68K 0x10115e60 GetfVramLoaded__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045e6a0
bool ResBaseList::GetfVramLoaded()
{
	return !(m_vramLoadedCount - m_vramEntryCount);
}

// 68K 0x10115ece GetfAnyVramLoaded__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045e6b0
bool ResBaseList::GetfAnyVramLoaded()
{
	return m_vramLoadedCount >= 1;
}

// 68K 0x10116284 GetfVramSwappable__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045e6c0
bool ResBaseList::GetfVramSwappable()
{
	return m_vramSwappable >= 1;
}

// 68K 0x10115e30 GetnVramEntries__12CResBaseLISTFv
// FUNCTION: LEMBALL 0x0045e6d0
unsigned int ResBaseList::GetnVramEntries()
{
	return 0;
}

// 68K 0x10115f0c UnLoadVramData__12CResBaseLISTFUlUc
// FUNCTION: LEMBALL 0x0045e6e0
void ResBaseList::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
}

// 68K 0x10115ea0 ForceLoadVram__12CResBaseLISTFUl
// FUNCTION: LEMBALL 0x0045e6f0
bool ResBaseList::ForceLoadVram(unsigned int p_index)
{
	return 0;
}
