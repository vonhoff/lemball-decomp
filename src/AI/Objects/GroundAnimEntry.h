#ifndef LEMBALL_AI_OBJECTS_GROUNDANIMENTRY_H
#define LEMBALL_AI_OBJECTS_GROUNDANIMENTRY_H

#include "../../Common.h"
#include "../Base/Coord3d.h" // complete type

// SIZE 0x18
struct GroundAnimEntry {
	Coord3d m_coordinate;          // 0x00
	undefined2 m_alignmentPadding; // 0x06
	void* m_mapCell;               // 0x08
	unsigned short m_currentFrame; // 0x0c
	unsigned short m_startFrame;   // 0x0e
	unsigned short m_endFrame;     // 0x10
	short m_direction;             // 0x12
	unsigned int m_active;         // 0x14
};

#endif
