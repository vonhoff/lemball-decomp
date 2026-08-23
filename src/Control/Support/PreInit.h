#ifndef LEMBALL_CONTROL_SUPPORT_PREINIT_H
#define LEMBALL_CONTROL_SUPPORT_PREINIT_H

#include "../../Common.h"
#include "PreInit.h" // complete type

// SIZE 0x1c
struct PreInit {
	unsigned int m_flags;         // 0x00
	unsigned int m_memoryBudget;  // 0x04
	int m_startBucket;            // 0x08
	int m_capabilityCount;        // 0x0c
	unsigned int* m_capabilities; // 0x10
	int m_shift;                  // 0x14
	void* m_icon;                 // 0x18
};

extern unsigned int g_anPreInitCapabilities[7];
extern PreInit g_preInitActive;
extern PreInit g_preInit;
#endif
