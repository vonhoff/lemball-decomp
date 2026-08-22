#ifndef LEMBALL_VIEWS_TARGET_TARGETSPRITEGROUNDLOOKUP_H
#define LEMBALL_VIEWS_TARGET_TARGETSPRITEGROUNDLOOKUP_H

#include "../../Common.h"

// SIZE 0x18
struct TargetSpriteGroundLookup {
public:
	// No functions mapped yet.

	unsigned char m_header[8]; // 0x00
	short m_width;             // 0x08
	short m_height;            // 0x0a
	unsigned int m_reserved;   // 0x0c
	unsigned char* m_maskA;    // 0x10
	unsigned char* m_maskB;    // 0x14
};

#endif
