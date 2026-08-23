#ifndef LEMBALL_VISOS_RESOURCES_RESOURCETYPELIST_H
#define LEMBALL_VISOS_RESOURCES_RESOURCETYPELIST_H

#include "../../Common.h"

// SIZE 0x10
struct ResourceTypeList {
	int m_currentIndex;        // 0x00
	int m_capacity;            // 0x04
	unsigned int* m_typeCodes; // 0x08
	int m_count;               // 0x0c
};

extern ResourceTypeList* g_pCompressedResourceTypes;
extern ResourceTypeList* g_pResourceTypes;
extern ResourceTypeList* g_pPreloadedResourceTypes;
#endif
