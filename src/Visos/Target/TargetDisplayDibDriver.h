#ifndef LEMBALL_VISOS_TARGET_TARGETDISPLAYDIBDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETDISPLAYDIBDRIVER_H

#include "TargetGDIDriver.h" // complete type

class TargetDrawingContext;
class TargetPlanarDisplayDibDriver;
class VsRect;
struct VsPoint;
struct VsSize;
// SIZE 0x30
// VTABLE: LEMBALL 0x004987b0
class TargetDisplayDibDriver : public TargetGDIDriver {
public:
	TargetDisplayDibDriver(const VsSize& p_size);
	virtual ~TargetDisplayDibDriver();
	virtual unsigned int UpdateDIBColourTable(TargetDrawingContext* p_context,
											  unsigned int p_start,
											  unsigned int p_count,
											  void* p_colours);
	virtual int BitBltContexts(TargetDrawingContext* p_destination,
							   VsRect* p_rect,
							   TargetDrawingContext* p_source,
							   VsPoint* p_position);
	virtual int StretchBltContexts(TargetDrawingContext* p_destination,
								   VsRect* p_rect,
								   TargetDrawingContext* p_source,
								   VsRect* p_sourceRect);

private:
	friend class TargetPlanarDisplayDibDriver;
	unsigned char* m_frameBuffer;                        // 0x1c
	unsigned int m_reserved20;                           // 0x20
	void(__stdcall* m_displayDibStart)();                // 0x24
	void(__stdcall* m_displayDibEnd)();                  // 0x28
	unsigned char*(__stdcall* m_displayDibGetAddress)(); // 0x2c
};

// SYNTHETIC: LEMBALL 0x00458330
// TargetDisplayDibDriver::`scalar deleting destructor'

#endif
