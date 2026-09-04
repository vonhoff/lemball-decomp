#ifndef LEMBALL_VISOS_TARGET_TARGETGDIDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETGDIDRIVER_H

#include "TargetGraphicsDriver.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498738
class TargetGDIDriver : public TargetGraphicsDriver {
public:
	TargetGDIDriver();
	virtual ~TargetGDIDriver();                                                // vtable+0x00
	virtual TargetDrawingContext* CreateDrawingContext();                      // vtable+0x04
	virtual int DestroyDrawingContext(TargetDrawingContext* p_drawingContext); // vtable+0x08
	virtual bool InitializeBitmapInfo(void* p_bitmapInfo);                     // vtable+0x0c
	virtual TargetDibContext* CreateDIBContext(TargetDrawingContext* p_drawingContext,
											   void* p_bitmapInfo); // vtable+0x10
	virtual int DestroyDIBContext(TargetDibContext* p_dibContext);  // vtable+0x14
	virtual void UpdateDIBColourTable(TargetDrawingContext* p_drawingContext,
									  unsigned int p_startIndex,
									  unsigned int p_entryCount,
									  void* p_colours); // vtable+0x18
	virtual void StretchBltContexts(TargetDrawingContext* p_destination,
									VsRect* p_destinationRect,
									TargetDrawingContext* p_source,
									VsRect* p_sourceRect); // vtable+0x1c
	virtual void BitBltContexts(TargetDrawingContext* p_destination,
								VsRect* p_destinationRect,
								TargetDrawingContext* p_source,
								VsPoint* p_sourcePosition); // vtable+0x20
	virtual TargetDibContext* SelectDIBContext(TargetDrawingContext* p_drawingContext,
											   TargetDibContext* p_dibContext); // vtable+0x24
	virtual TargetDibContext* RestoreDIBContext(TargetDrawingContext* p_drawingContext,
												TargetDibContext* p_dibContext); // vtable+0x28
};

// SYNTHETIC: LEMBALL 0x00458230
// TargetGDIDriver::`scalar deleting destructor'

#endif
