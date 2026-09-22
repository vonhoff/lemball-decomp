#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CDIRECTDRAWDRIVER_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CDIRECTDRAWDRIVER_H

#include "CGraphicsDriver.h" // complete type

struct CVsSize;
struct IDirectDrawPalette;
struct IDirectDraw;
struct IDirectDrawSurface;

// SIZE 0x4a8
// VTABLE: LEMBALL 0x004987e8
class CDirectDrawDriver : public CGraphicsDriver {
public:
	CDirectDrawDriver(CVsSize* p_size, int p_fullScreen);
	virtual ~CDirectDrawDriver();
	virtual CDrawingContext* CreateDrawingContext();
	virtual int DestroyDrawingContext(CDrawingContext* p_drawingContext);
	virtual bool InitializeBitmapInfo(void* p_bitmapInfo);
	virtual CDibContext* CreateDibContext(CDrawingContext* p_drawingContext, void* p_bitmapInfo);
	virtual int DestroyDibContext(CDibContext* p_dibContext);
	virtual unsigned int UpdateDibColourTable(CDrawingContext* p_drawingContext,
											  unsigned int p_startIndex,
											  unsigned int p_entryCount,
											  void* p_colours);
	virtual CDibContext* SelectDibContext(CDrawingContext* p_drawingContext, CDibContext* p_dibContext);
	virtual CDibContext* RestoreDibContext(CDrawingContext* p_drawingContext, CDibContext* p_dibContext);
	virtual bool CreatePalette(void* p_paletteDescription);
	virtual bool HasPalette();
	virtual int BitBltContexts(CDrawingContext* p_destination,
							   CVsRect* p_destinationRect,
							   CDrawingContext* p_source,
							   CVsPoint* p_sourcePosition);
	virtual int StretchBltContexts(CDrawingContext* p_destination,
								   CVsRect* p_destinationRect,
								   CDrawingContext* p_source,
								   CVsRect* p_sourceRect);

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
// CDirectDrawDriver::`scalar deleting destructor'

#endif
