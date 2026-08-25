#ifndef LEMBALL_VISOS_TARGET_TARGETGRAPHICSDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETGRAPHICSDRIVER_H

#include "../../Common.h"
#include "../Foundation/VsSize.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498700 abstract/base driver
// VTABLE: LEMBALL 0x00498738 Gdi DIB driver
// Subobject VTABLE: 0x004987b0 DisplayDib driver
// Subobject VTABLE: 0x004987e8 DirectDraw driver
// Subobject VTABLE: 0x00498840 DirectDraw-derived driver
class TargetGraphicsDriver {
public:
	virtual ~TargetGraphicsDriver(); // vtable+0x00

private:
	void* m_driverModule;         // 0x04
	void* m_palette;              // 0x08
	unsigned int m_ready;         // 0x0c
	void* m_window;               // 0x10
	VsSize m_screenSize;          // 0x14
	PvGdiBitmap* m_currentBitmap; // 0x18
};

extern TargetGraphicsDriver* g_pTargetGraphicsDriver;
extern void* g_apCResRasterConstructionVtable[15];
extern TargetGraphicsSystemState* g_pTargetGraphicsSystem;
extern unsigned int g_dwWinGDrawColourTable[256];
#endif
