#ifndef LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESOURCETYPELIST_H
#define LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESOURCETYPELIST_H

#include "../../Common.h"

// SIZE 0x10
struct ResourceTypeList {
public:
	// No functions mapped yet.

	int m_currentIndex;        // 0x00
	int m_capacity;            // 0x04
	unsigned int* m_typeCodes; // 0x08
	int m_count;               // 0x0c
};

#endif
