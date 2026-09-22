#ifndef LEMBALL_VISOS_TARGET_TARGETPLANARDISPLAYDIBDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETPLANARDISPLAYDIBDRIVER_H

#include "TargetDisplayDibDriver.h" // complete type

struct CVsSize;
// SIZE 0x30
// VTABLE: LEMBALL 0x00498840
class TargetPlanarDisplayDibDriver : public TargetDisplayDibDriver {
public:
	TargetPlanarDisplayDibDriver(const CVsSize& p_size) : TargetDisplayDibDriver(p_size) {}
	virtual int BitBltContexts(TargetDrawingContext* p_destination,
							   CVsRect* p_rect,
							   TargetDrawingContext* p_source,
							   CVsPoint* p_position);
	virtual int StretchBltContexts(TargetDrawingContext* p_destination,
								   CVsRect* p_rect,
								   TargetDrawingContext* p_source,
								   CVsRect* p_sourceRect);
	void SetPlaneWriteMask(unsigned char p_mask);
	void ExtractPlaneBytes(unsigned char* p_destination, unsigned char* p_source, int p_count);
};

// SYNTHETIC: LEMBALL 0x004583c0
// TargetPlanarDisplayDibDriver::`scalar deleting destructor'

#endif
