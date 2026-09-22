#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CGRAPHICSDRIVER_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CGRAPHICSDRIVER_H

#include "../../Foundation/CVsPoint.h" // complete type
#include "../../Foundation/CVsRect.h"  // complete type
#include "../../Foundation/CVsSize.h"  // complete type
#include "CDibContext.h"               // complete type
#include "CDrawingContext.h"           // complete type

class CPVGDIBitmap;
class CResPALETTE;
struct CGraphicsState;

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498700
// The CGdiDriver construction table is at 0x00498738.
// Subobject VTABLE: 0x004987b0 DisplayDib driver
// Subobject VTABLE: 0x004987e8 DirectDraw driver
// Subobject VTABLE: 0x00498840 DirectDraw-derived driver
class CGraphicsDriver {
public:
	CGraphicsDriver()
	{
		m_screenSize.m_height = 0;
		m_driverModule = 0;
		m_screenSize.m_width = 0;
		m_palette = 0;
		m_ready = 0;
		m_window = 0;
	}
	virtual ~CGraphicsDriver();                                               // vtable+0x00
	virtual CDrawingContext* CreateDrawingContext() = 0;                      // vtable+0x04
	virtual int DestroyDrawingContext(CDrawingContext* p_drawingContext) = 0; // vtable+0x08
	virtual bool InitializeBitmapInfo(void* p_bitmapInfo) = 0;                // vtable+0x0c
	virtual CDibContext* CreateDibContext(CDrawingContext* p_drawingContext,
										  void* p_bitmapInfo) = 0; // vtable+0x10
	virtual int DestroyDibContext(CDibContext* p_dibContext) = 0;  // vtable+0x14
	virtual unsigned int UpdateDibColourTable(CDrawingContext* p_drawingContext,
											  unsigned int p_startIndex,
											  unsigned int p_entryCount,
											  void* p_colours) = 0; // vtable+0x18
	virtual int StretchBltContexts(CDrawingContext* p_destination,
								   CVsRect* p_destinationRect,
								   CDrawingContext* p_source,
								   CVsRect* p_sourceRect) = 0; // vtable+0x1c
	virtual int BitBltContexts(CDrawingContext* p_destination,
							   CVsRect* p_destinationRect,
							   CDrawingContext* p_source,
							   CVsPoint* p_sourcePosition) = 0; // vtable+0x20
	virtual CDibContext* SelectDibContext(CDrawingContext* p_drawingContext,
										  CDibContext* p_dibContext) = 0; // vtable+0x24
	virtual CDibContext* RestoreDibContext(CDrawingContext* p_drawingContext,
										   CDibContext* p_dibContext) = 0; // vtable+0x28
	virtual bool CreatePalette(void* p_paletteDescription);                // vtable+0x2c
	virtual bool RealizePalette(CDrawingContext* p_drawingContext);        // vtable+0x30
	virtual bool HasPalette();                                             // vtable+0x34
	bool BlitWrappedBitmap(CDrawingContext* p_destination,
						   CVsRect* p_destinationRect,
						   CDrawingContext* p_source,
						   CVsRect* p_sourceRect,
						   CPVGDIBitmap* p_bitmap);

	friend class CWnd;
	friend class CMain2DDisplay;
	friend struct CGraphicsState;
	friend class CGWnd;
	friend class CSurface;
	friend bool PumpEvents();

protected:
	void* m_driverModule;          // 0x04
	void* m_palette;               // 0x08
	unsigned int m_ready;          // 0x0c
	void* m_window;                // 0x10
	CVsSize m_screenSize;          // 0x14
	CPVGDIBitmap* m_currentBitmap; // 0x18
};

extern CGraphicsDriver* g_pTargetGraphicsDriver;
extern unsigned int g_dwFullScreenGdi;
extern void* g_apCResRasterConstructionVtable[15];
extern CGraphicsState* g_pTargetGraphicsSystem;
extern unsigned int g_dwWinGDrawColourTable[256];
void BuildSurfaceColourTable(unsigned int* p_entries,
							 CResPALETTE* p_palette,
							 void* p_unused,
							 unsigned int* p_fallbackEntries);
#include "WinGDraw.h"
// SYNTHETIC: LEMBALL 0x00458210
// CGraphicsDriver::`scalar deleting destructor'

#endif
