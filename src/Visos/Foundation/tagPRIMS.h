#ifndef LEMBALL_VISOS_FOUNDATION_TAGPRIMS_H
#define LEMBALL_VISOS_FOUNDATION_TAGPRIMS_H

#include "Visos/Graphics/Primitives/CBigBitmap.h"
#include "Visos/Graphics/Primitives/CClipRect.h"
#include "Visos/Graphics/Primitives/CCopyToBackBuff.h"
#include "Visos/Graphics/Primitives/CDrawingMark.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"

// SIZE 0x260
class tagPRIMS {
public:
	tagPRIMS();
	~tagPRIMS();

	CCopyToBackBuff m_bitmap;   // 0x00
	CBigBitmap m_primitive;     // 0x10
	CBigBitmap m_records[10];   // 0x34
	CClipRect m_rects[2];       // 0x19c
	CDrawingMark m_drawingMark; // 0x1bc
	CSolidRect m_lines[10];     // 0x1c0
};

#endif
