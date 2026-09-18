#ifndef LEMBALL_VISOS_TARGET_TARGETPLANARDISPLAYDIBDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETPLANARDISPLAYDIBDRIVER_H

#include "TargetDisplayDibDriver.h" // complete type

class TargetDrawingContext;
class VsRect;
struct VsPoint;
struct VsSize;
// SIZE 0x30
// VTABLE: LEMBALL 0x00498840
class TargetPlanarDisplayDibDriver : public TargetDisplayDibDriver {
public:
	TargetPlanarDisplayDibDriver(const VsSize& p_size) : TargetDisplayDibDriver(p_size) {}
	virtual int BitBltContexts(TargetDrawingContext* p_destination,
							   VsRect* p_rect,
							   TargetDrawingContext* p_source,
							   VsPoint* p_position);
	virtual int StretchBltContexts(TargetDrawingContext* p_destination,
								   VsRect* p_rect,
								   TargetDrawingContext* p_source,
								   VsRect* p_sourceRect);
	void SetPlaneWriteMask(unsigned char p_mask);
	void ExtractPlaneBytes(unsigned char* p_destination, unsigned char* p_source, int p_count);
};

// SYNTHETIC: LEMBALL 0x004583c0
// TargetPlanarDisplayDibDriver::`scalar deleting destructor'

#endif
