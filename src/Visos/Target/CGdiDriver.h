#ifndef LEMBALL_VISOS_TARGET_CGDIDRIVER_H
#define LEMBALL_VISOS_TARGET_CGDIDRIVER_H

#include "CGraphicsDriver.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498738
class CGdiDriver : public CGraphicsDriver {
public:
	CGdiDriver();
	virtual CDrawingContext* CreateDrawingContext();                      // vtable+0x04
	virtual int DestroyDrawingContext(CDrawingContext* p_drawingContext); // vtable+0x08
	virtual bool InitializeBitmapInfo(void* p_bitmapInfo);                // vtable+0x0c
	virtual CDibContext* CreateDibContext(CDrawingContext* p_drawingContext,
										  void* p_bitmapInfo); // vtable+0x10
	virtual int DestroyDibContext(CDibContext* p_dibContext);  // vtable+0x14
	virtual unsigned int UpdateDibColourTable(CDrawingContext* p_drawingContext,
											  unsigned int p_startIndex,
											  unsigned int p_entryCount,
											  void* p_colours); // vtable+0x18
	virtual int StretchBltContexts(CDrawingContext* p_destination,
								   CVsRect* p_destinationRect,
								   CDrawingContext* p_source,
								   CVsRect* p_sourceRect); // vtable+0x1c
	virtual int BitBltContexts(CDrawingContext* p_destination,
							   CVsRect* p_destinationRect,
							   CDrawingContext* p_source,
							   CVsPoint* p_sourcePosition); // vtable+0x20
	virtual CDibContext* SelectDibContext(CDrawingContext* p_drawingContext,
										  CDibContext* p_dibContext); // vtable+0x24
	virtual CDibContext* RestoreDibContext(CDrawingContext* p_drawingContext,
										   CDibContext* p_dibContext); // vtable+0x28
};

// SYNTHETIC: LEMBALL 0x00458230
// CGdiDriver::`scalar deleting destructor'

#endif
