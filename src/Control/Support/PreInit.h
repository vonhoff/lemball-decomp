#ifndef LEMBALL_CONTROL_SUPPORT_PREINIT_H
#define LEMBALL_CONTROL_SUPPORT_PREINIT_H

#include "../../Common.h"
#include "PreInit.h" // complete type

// SIZE 0x1c
struct PreInit {
public:
	// No functions mapped yet.

	unsigned int m_flags;         // 0x00
	unsigned int m_memoryBudget;  // 0x04
	undefined4 m_unk0x08;         // 0x08
	int m_capabilityCount;        // 0x0c
	unsigned int* m_capabilities; // 0x10
	undefined4 m_unk0x14;         // 0x14
	void* m_icon;                 // 0x18
};

// Confirmed class-scoped globals.
extern PreInit g_preInitActive;
extern PreInit g_preInit;
#endif
