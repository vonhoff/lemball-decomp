#ifndef LEMBALL_CONTROL_SUPPORT_TAGPRE_INIT_H
#define LEMBALL_CONTROL_SUPPORT_TAGPRE_INIT_H

#include "Engine/Memory/SmallMemoryConstants.h"

// SIZE 0x1c
struct tagPRE_INIT {
	unsigned int m_flags;         // 0x00
	unsigned int m_memoryBudget;  // 0x04
	int m_startBucket;            // 0x08
	int m_capabilityCount;        // 0x0c
	unsigned int* m_capabilities; // 0x10
	int m_shift;                  // 0x14
	void* m_icon;                 // 0x18
};

extern unsigned int g_anPreInitCapabilities[SMALL_MEMORY_BUCKET_COUNT];
extern tagPRE_INIT g_preInitActive;
extern tagPRE_INIT g_preInit;
#endif
