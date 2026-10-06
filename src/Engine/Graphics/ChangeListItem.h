#ifndef LEMBALL_VISOS_FOUNDATION_CHANGELISTITEM_H
#define LEMBALL_VISOS_FOUNDATION_CHANGELISTITEM_H

#include "Engine/Math/CVSRect.h"

// SIZE 0x0c
class ChangeListItem : public CVSRect {
public:
	ChangeListItem();

	unsigned int m_drawMark; // 0x08
};

#endif
