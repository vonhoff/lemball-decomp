#ifndef LEMBALL_SCAFFOLD_VIEWS_INPUT_PADTOBUTTONENTRY_H
#define LEMBALL_SCAFFOLD_VIEWS_INPUT_PADTOBUTTONENTRY_H

#include "../../Common.h"

// SIZE 0x08
struct PadToButtonEntry {
public:
	// No functions mapped yet.

	void* m_button;         // 0x00
	unsigned int m_padCode; // 0x04
};

#endif
