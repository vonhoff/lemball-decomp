#ifndef LEMBALL_VISOS_FOUNDATION_PRIMS_H
#define LEMBALL_VISOS_FOUNDATION_PRIMS_H

#include "../../Common.h"
#include "../../Frontend/Resources/FrontendPrimitiveRecord.h" // complete type
#include "../Graphics/DrawingMark.h"                          // complete type
#include "../Graphics/Line.h"                                 // complete type
#include "../Graphics/SolidRect.h"                            // complete type

// SIZE 0x260
struct Prims {
public:
	Prims();
	~Prims();

	unsigned char m_header[0x34];          // 0x00
	FrontendPrimitiveRecord m_records[10]; // 0x34
	SolidRect m_rects[2];                  // 0x19c
	DrawingMark m_drawingMark;             // 0x1bc
	Line m_lines[10];                      // 0x1c0
};

#endif
