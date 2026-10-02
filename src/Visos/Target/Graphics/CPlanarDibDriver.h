#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CPLANARDIBDRIVER_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CPLANARDIBDRIVER_H

#include "CDisplayDibDriver.h"

struct CVSSize;
// SIZE 0x30
// VTABLE: LEMBALL 0x00498840
class CPlanarDibDriver : public CDisplayDibDriver {
public:
	CPlanarDibDriver(const CVSSize& p_size) : CDisplayDibDriver(p_size) {}
	virtual int BitBltContexts(CDrawingContext* p_destination,
							   CVSRect* p_rect,
							   CDrawingContext* p_source,
							   CVSPoint* p_position);
	virtual int StretchBltContexts(CDrawingContext* p_destination,
								   CVSRect* p_rect,
								   CDrawingContext* p_source,
								   CVSRect* p_sourceRect);
	void SetPlaneWriteMask(unsigned char p_mask);
	void ExtractPlaneBytes(unsigned char* p_destination, unsigned char* p_source, int p_count);
};

// SYNTHETIC: LEMBALL 0x004583c0
// CPlanarDibDriver::`scalar deleting destructor'

#endif
