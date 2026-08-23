#include "ResBaseList.h"

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
// STUB: LEMBALL 0x0045d2b0
void ResBaseList::OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size)
{
}

// 68K 0x1020345a LoadData__12CResBaseLISTFv
// STUB: LEMBALL 0x0045d430
void ResBaseList::LoadData()
{
}

// 68K 0x10203586 ForceLoadVram__12CResBaseLISTFv
// STUB: LEMBALL 0x0045d4f0
bool ResBaseList::ForceLoadVram()
{
	return 0;
}

// 68K 0x10203616 UnLoadData__12CResBaseLISTFUc
// STUB: LEMBALL 0x0045d540
void ResBaseList::UnLoadData(unsigned char p_force)
{
}

// 68K 0x102036cc UnLoadVramData__12CResBaseLISTFUc
// STUB: LEMBALL 0x0045d5c0
void ResBaseList::UnLoadVramData(unsigned char p_force)
{
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
	return m_vramLoadedCount - m_vramEntryCount < 1;
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
// STUB: LEMBALL 0x0045e6d0
unsigned int ResBaseList::GetnVramEntries()
{
	return 0;
}

// 68K 0x10115f0c UnLoadVramData__12CResBaseLISTFUlUc
// STUB: LEMBALL 0x0045e6e0
void ResBaseList::UnLoadVramData(unsigned int p_index, unsigned char p_force)
{
}

// 68K 0x10115ea0 ForceLoadVram__12CResBaseLISTFUl
// STUB: LEMBALL 0x0045e6f0
bool ResBaseList::ForceLoadVram(unsigned int p_index)
{
	return 0;
}

// 68K 0x10100944 __dt__12CResBaseLISTFv
// SYNTHETIC: LEMBALL 0x0045e700
// ResBaseList::`scalar deleting destructor'
ResBaseList::~ResBaseList()
{
}
