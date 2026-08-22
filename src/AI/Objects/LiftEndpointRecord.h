#ifndef LEMBALL_AI_OBJECTS_LIFTENDPOINTRECORD_H
#define LEMBALL_AI_OBJECTS_LIFTENDPOINTRECORD_H

#include "../../Common.h"
#include "../Base/Coord3d.h" // complete type

// SIZE 0x0c
struct LiftEndpointRecord {
public:
	// No functions mapped yet.

	Coord3d m_start; // 0x00
	Coord3d m_end;   // 0x06
};

#endif
