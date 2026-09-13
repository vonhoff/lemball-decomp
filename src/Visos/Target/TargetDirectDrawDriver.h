#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWDRIVER_H

#include "../../Common.h"
#include "TargetGraphicsDriver.h" // complete type

struct IDirectDrawPalette;

// SIZE 0x4a8
// VTABLE: LEMBALL 0x004987e8
class TargetDirectDrawDriver : public TargetGraphicsDriver {
public:
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

private:
	void* m_directDraw;                     // 0x1c
	void* m_primarySurface;                 // 0x20
	void* m_surface24;                      // 0x24
	void* m_surface28;                      // 0x28
	void* m_surface2c;                      // 0x2c
	IDirectDrawPalette* m_paletteInterface; // 0x30
	undefined m_driverState[0x6c];          // 0x34
	void* m_contextSurfaces[257];           // 0xa0
	int m_nextContextIndex;                 // 0x4a4
};

#endif
