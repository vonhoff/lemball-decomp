#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWDRIVER_H

#include "TargetGraphicsDriver.h" // complete type

struct VsSize;
struct IDirectDrawPalette;
struct IDirectDraw;
struct IDirectDrawSurface;

// SIZE 0x4a8
// VTABLE: LEMBALL 0x004987e8
class TargetDirectDrawDriver : public TargetGraphicsDriver {
public:
	TargetDirectDrawDriver(VsSize* p_size, int p_fullScreen);
	virtual ~TargetDirectDrawDriver();
	virtual TargetDrawingContext* CreateDrawingContext();
	virtual int DestroyDrawingContext(TargetDrawingContext* p_drawingContext);
	virtual bool InitializeBitmapInfo(void* p_bitmapInfo);
	virtual TargetDibContext* CreateDIBContext(TargetDrawingContext* p_drawingContext, void* p_bitmapInfo);
	virtual int DestroyDIBContext(TargetDibContext* p_dibContext);
	virtual unsigned int UpdateDIBColourTable(TargetDrawingContext* p_drawingContext,
											  unsigned int p_startIndex,
											  unsigned int p_entryCount,
											  void* p_colours);
	virtual TargetDibContext* SelectDIBContext(TargetDrawingContext* p_drawingContext, TargetDibContext* p_dibContext);
	virtual TargetDibContext* RestoreDIBContext(TargetDrawingContext* p_drawingContext, TargetDibContext* p_dibContext);
	virtual bool CreatePalette(void* p_paletteDescription);
	virtual bool HasPalette();
	virtual int BitBltContexts(TargetDrawingContext* p_destination,
							   VsRect* p_destinationRect,
							   TargetDrawingContext* p_source,
							   VsPoint* p_sourcePosition);
	virtual int StretchBltContexts(TargetDrawingContext* p_destination,
								   VsRect* p_destinationRect,
								   TargetDrawingContext* p_source,
								   VsRect* p_sourceRect);

private:
	IDirectDraw* m_directDraw;              // 0x1c
	IDirectDrawSurface* m_primarySurface;   // 0x20
	void* m_surface24;                      // 0x24
	void* m_surface28;                      // 0x28
	void* m_surface2c;                      // 0x2c
	IDirectDrawPalette* m_paletteInterface; // 0x30
	char m_driverState[0x6c];               // 0x34
	void* m_contextSurfaces[257];           // 0xa0
	int m_nextContextIndex;                 // 0x4a4
};

// SYNTHETIC: LEMBALL 0x00458360
// TargetDirectDrawDriver::`scalar deleting destructor'

#endif
