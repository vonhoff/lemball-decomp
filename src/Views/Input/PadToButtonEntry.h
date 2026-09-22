#ifndef LEMBALL_VIEWS_INPUT_PADTOBUTTONENTRY_H
#define LEMBALL_VIEWS_INPUT_PADTOBUTTONENTRY_H

class CPVButton;
// SIZE 0x08
struct PadToButtonEntry {
	CPVButton* m_button;    // 0x00
	unsigned int m_padCode; // 0x04
};

#endif
