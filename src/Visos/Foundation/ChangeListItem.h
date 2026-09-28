#ifndef LEMBALL_VISOS_FOUNDATION_CHANGELISTITEM_H
#define LEMBALL_VISOS_FOUNDATION_CHANGELISTITEM_H

// SIZE 0x0c
class ChangeListItem {
public:
	ChangeListItem();

	short m_width;           // 0x00
	short m_height;          // 0x02
	short m_x;               // 0x04
	short m_y;               // 0x06
	unsigned int m_drawMark; // 0x08
};

#endif
