#ifndef LEMBALL_CONTROL_LEVEL_LOADBLOCKHEADER_H
#define LEMBALL_CONTROL_LEVEL_LOADBLOCKHEADER_H

#include "../../Common.h"

// SIZE 0x08
struct LoadBlockHeader {
public:
	// No functions mapped yet.

	unsigned int m_type; // 0x00
	unsigned int m_size; // 0x04
};

#endif
