#ifndef LEMBALL_VISOS_TARGET_CDISPLAYDIBDRIVER_H
#define LEMBALL_VISOS_TARGET_CDISPLAYDIBDRIVER_H

#include "CGdiDriver.h" // complete type

struct CVsSize;
// SIZE 0x30
// VTABLE: LEMBALL 0x004987b0
class CDisplayDibDriver : public CGdiDriver {
public:
	CDisplayDibDriver(const CVsSize& p_size);
	virtual ~CDisplayDibDriver();
	virtual unsigned int UpdateDibColourTable(CDrawingContext* p_context,
											  unsigned int p_start,
											  unsigned int p_count,
											  void* p_colours);
	virtual int BitBltContexts(CDrawingContext* p_destination,
							   CVsRect* p_rect,
							   CDrawingContext* p_source,
							   CVsPoint* p_position);
	virtual int StretchBltContexts(CDrawingContext* p_destination,
								   CVsRect* p_rect,
								   CDrawingContext* p_source,
								   CVsRect* p_sourceRect);

private:
	friend class CPlanarDibDriver;
	unsigned char* m_frameBuffer;                        // 0x1c
	unsigned int m_reserved20;                           // 0x20
	void(__stdcall* m_displayDibStart)();                // 0x24
	void(__stdcall* m_displayDibEnd)();                  // 0x28
	unsigned char*(__stdcall* m_displayDibGetAddress)(); // 0x2c
};

// SYNTHETIC: LEMBALL 0x00458330
// CDisplayDibDriver::`scalar deleting destructor'

#endif
