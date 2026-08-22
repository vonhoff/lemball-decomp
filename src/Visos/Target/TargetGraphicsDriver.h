#ifndef LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETGRAPHICSDRIVER_H
#define LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETGRAPHICSDRIVER_H

#include "../../Common.h"
#include "../Foundation/VsSize.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498700 abstract/base driver
// VTABLE: LEMBALL 0x00498738 Gdi DIB driver
// VTABLE: LEMBALL 0x004987b0 DisplayDib driver
// VTABLE: LEMBALL 0x004987e8 DirectDraw driver
// VTABLE: LEMBALL 0x00498840 DirectDraw-derived driver
class TargetGraphicsDriver {
public:
	// No functions mapped yet.

private:
	void* m_driverModule;         // 0x04
	void* m_palette;              // 0x08
	unsigned int m_ready;         // 0x0c
	void* m_window;               // 0x10
	VsSize m_screenSize;          // 0x14
	PvGdiBitmap* m_currentBitmap; // 0x18
};

// Confirmed class-scoped globals.
extern TargetGraphicsDriver* g_pTargetGraphicsDriver;
#endif
