#ifndef LEMBALL_VISOS_RESOURCES_RESOURCETYPELIST_H
#define LEMBALL_VISOS_RESOURCES_RESOURCETYPELIST_H

#include "ResourceChunkTypes.h"

// SIZE 0x10
struct ResourceTypeList {
	ResourceTypeList(int p_capacity)
	{
		m_capacity = p_capacity;
		m_currentIndex = -1;
		m_count = 0;
		m_typeCodes = new CHUNKID[m_capacity];
	}
	~ResourceTypeList() { delete[] m_typeCodes; }

	int m_currentIndex;   // 0x00
	int m_capacity;       // 0x04
	CHUNKID* m_typeCodes; // 0x08
	int m_count;          // 0x0c
};

extern ResourceTypeList* g_pCompressedResourceTypes;
extern ResourceTypeList* g_pResourceTypes;
extern ResourceTypeList* g_pPreloadedResourceTypes;
#endif
