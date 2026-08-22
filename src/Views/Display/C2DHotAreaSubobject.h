#ifndef LEMBALL_SCAFFOLD_VIEWS_DISPLAY_C2DHOTAREASUBOBJECT_H
#define LEMBALL_SCAFFOLD_VIEWS_DISPLAY_C2DHOTAREASUBOBJECT_H

#include "../../Common.h"
#include "../../Visos/Foundation/VsPoint.h" // complete type

// SIZE 0x2414
class C2DHotAreaSubobject {
public:
	// No functions mapped yet.

private:
	unsigned int m_mouseDown; // 0x178
	Map* m_map;               // 0x900
	int m_viewOriginX;        // 0x904
	int m_viewOriginY;        // 0x908
	VsPoint m_cursorPoint;    // 0x928
	Main2DDisplay* m_display; // 0x964
	unsigned int m_paused;    // 0xa68
};

#endif
