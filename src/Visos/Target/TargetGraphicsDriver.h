#ifndef LEMBALL_VISOS_TARGET_TARGETGRAPHICSDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETGRAPHICSDRIVER_H

#include "../../Common.h"
#include "../Foundation/VsPoint.h" // complete type
#include "../Foundation/VsRect.h"  // complete type
#include "../Foundation/VsSize.h"  // complete type
#include "TargetDibContext.h"      // complete type
#include "TargetDrawingContext.h"  // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498700
// The TargetGDIDriver construction table is at 0x00498738.
// Subobject VTABLE: 0x004987b0 DisplayDib driver
// Subobject VTABLE: 0x004987e8 DirectDraw driver
// Subobject VTABLE: 0x00498840 DirectDraw-derived driver
class TargetGraphicsDriver {
public:
	TargetGraphicsDriver()
	{
		m_screenSize.m_height = 0;
		m_driverModule = 0;
		m_screenSize.m_width = 0;
		m_palette = 0;
		m_ready = 0;
		m_window = 0;
	}
	virtual ~TargetGraphicsDriver();                                               // vtable+0x00
	virtual TargetDrawingContext* CreateDrawingContext() = 0;                      // vtable+0x04
	virtual int DestroyDrawingContext(TargetDrawingContext* p_drawingContext) = 0; // vtable+0x08
	virtual bool InitializeBitmapInfo(void* p_bitmapInfo) = 0;                     // vtable+0x0c
	virtual TargetDibContext* CreateDIBContext(TargetDrawingContext* p_drawingContext,
											   void* p_bitmapInfo) = 0; // vtable+0x10
	virtual int DestroyDIBContext(TargetDibContext* p_dibContext) = 0;  // vtable+0x14
	virtual void UpdateDIBColourTable(TargetDrawingContext* p_drawingContext,
									  unsigned int p_startIndex,
									  unsigned int p_entryCount,
									  void* p_colours) = 0; // vtable+0x18
	virtual void StretchBltContexts(TargetDrawingContext* p_destination,
									VsRect* p_destinationRect,
									TargetDrawingContext* p_source,
									VsRect* p_sourceRect) = 0; // vtable+0x1c
	virtual void BitBltContexts(TargetDrawingContext* p_destination,
								VsRect* p_destinationRect,
								TargetDrawingContext* p_source,
								VsPoint* p_sourcePosition) = 0; // vtable+0x20
	virtual TargetDibContext* SelectDIBContext(TargetDrawingContext* p_drawingContext,
											   TargetDibContext* p_dibContext) = 0; // vtable+0x24
	virtual TargetDibContext* RestoreDIBContext(TargetDrawingContext* p_drawingContext,
												TargetDibContext* p_dibContext) = 0; // vtable+0x28
	virtual bool CreatePalette(void* p_paletteDescription);                          // vtable+0x2c
	virtual bool RealizePalette(TargetDrawingContext* p_drawingContext);             // vtable+0x30
	virtual bool HasPalette();                                                       // vtable+0x34
	bool BlitWrappedBitmap(TargetDrawingContext* p_destination,
						   VsRect* p_destinationRect,
						   TargetDrawingContext* p_source,
						   VsRect* p_sourceRect,
						   PvGdiBitmap* p_bitmap);

	friend class Wnd;
	friend class Main2DDisplay;
	friend struct TargetGraphicsSystemState;
	friend class GWnd;
	friend class Surface;
	friend bool TargetPumpEvents();

protected:
	void* m_driverModule;         // 0x04
	void* m_palette;              // 0x08
	unsigned int m_ready;         // 0x0c
	void* m_window;               // 0x10
	VsSize m_screenSize;          // 0x14
	PvGdiBitmap* m_currentBitmap; // 0x18
};

extern TargetGraphicsDriver* g_pTargetGraphicsDriver;
extern unsigned int g_dwFullScreenGdi;
extern void* g_apCResRasterConstructionVtable[15];
extern TargetGraphicsSystemState* g_pTargetGraphicsSystem;
extern unsigned int g_dwWinGDrawColourTable[256];
void TargetBuildSurfaceColourTable(unsigned int* p_entries,
								   ResPalette* p_palette,
								   void* p_unused,
								   unsigned int* p_fallbackEntries);
long __stdcall TargetWinGDrawCodec_DriverProc(unsigned int p_driverId,
											  void* p_driverHandle,
											  unsigned int p_message,
											  long p_param1,
											  long p_param2);
// SYNTHETIC: LEMBALL 0x00458210
// TargetGraphicsDriver::`scalar deleting destructor'

#endif
