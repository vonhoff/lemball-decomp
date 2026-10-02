#ifndef LEMBALL_AI_OBJECTS_LIFTENDPOINTRECORD_H
#define LEMBALL_AI_OBJECTS_LIFTENDPOINTRECORD_H

#include "../Base/tCoord3d.h"

// SIZE 0x0c
struct LiftEndpointRecord {
	tCoord3d m_start; // 0x00
	tCoord3d m_end;   // 0x06
};

#endif
