#ifndef LEMBALL_VISOS_FOUNDATION_PRIMS_H
#define LEMBALL_VISOS_FOUNDATION_PRIMS_H

#include "../Graphics/BitmapRes.h"    // complete type
#include "../Graphics/CBitmap.h"      // complete type
#include "../Graphics/CDrawingMark.h" // complete type
#include "../Graphics/CLine.h"        // complete type
#include "../Graphics/CSolidRect.h"   // complete type

// SIZE 0x260
class Prims {
public:
	Prims();
	~Prims();

	CBitmap m_bitmap;           // 0x00
	BitmapRes m_primitive;      // 0x10
	BitmapRes m_records[10];    // 0x34
	CSolidRect m_rects[2];      // 0x19c
	CDrawingMark m_drawingMark; // 0x1bc
	CLine m_lines[10];          // 0x1c0
};

#endif
