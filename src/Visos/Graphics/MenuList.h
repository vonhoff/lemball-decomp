#ifndef LEMBALL_VISOS_GRAPHICS_MENULIST_H
#define LEMBALL_VISOS_GRAPHICS_MENULIST_H

// SIZE 0x18
struct MenuList {
	char* m_name;    // 0x00
	int m_commandId; // 0x04
	int m_action;    // 0x08
	int m_enabled;   // 0x0c
	int m_checked;   // 0x10
	int m_unk14;     // 0x14
};

#endif
