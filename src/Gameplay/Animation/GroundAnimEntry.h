#ifndef LEMBALL_AI_OBJECTS_GROUNDANIMENTRY_H
#define LEMBALL_AI_OBJECTS_GROUNDANIMENTRY_H

#include "Map/CGround.h"
#include "Gameplay/Geometry/tCoord3d.h"

class CMap;
// SIZE 0x18
struct GroundAnimEntry {
	tCoord3d m_coordinate;             // 0x00
	unsigned short m_alignmentPadding; // 0x06
	CGround* m_mapCell;                // 0x08
	short m_currentFrame;              // 0x0c
	short m_startFrame;                // 0x0e
	short m_endFrame;                  // 0x10
	short m_direction;                 // 0x12
	unsigned int m_active;             // 0x14
};

#endif
