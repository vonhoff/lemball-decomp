#ifndef LEMBALL_VISOS_FOUNDATION_CHANGELISTITEM_H
#define LEMBALL_VISOS_FOUNDATION_CHANGELISTITEM_H

// SIZE 0x0c
class ChangeListItem {
public:
	ChangeListItem();

	short width;           // 0x00
	short height;          // 0x02
	short x;               // 0x04
	short y;               // 0x06
	unsigned int drawMark; // 0x08
};

#endif
