#ifndef LEMBALL_VIEWS_TARGET_TARGETOBJECTCLIPGRID_H
#define LEMBALL_VIEWS_TARGET_TARGETOBJECTCLIPGRID_H

#include "../../Common.h"

// SIZE 0x1a4
struct TargetObjectClipGrid {
public:
	// No functions mapped yet.

	unsigned int m_cellCount;                  // 0x00
	short m_width;                             // 0x04
	short m_height;                            // 0x06
	short m_cellWidth;                         // 0x08
	short m_cellHeight;                        // 0x0a
	TargetObjectClipCell* m_cells;             // 0x0c
	unsigned int m_touchedCount;               // 0x10
	TargetObjectClipCell* m_touchedCells[100]; // 0x14
};

#endif
