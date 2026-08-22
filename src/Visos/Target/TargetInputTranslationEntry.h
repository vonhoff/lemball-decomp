#ifndef LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETINPUTTRANSLATIONENTRY_H
#define LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETINPUTTRANSLATIONENTRY_H

#include "../../Common.h"

// SIZE 0x08
struct TargetInputTranslationEntry {
public:
	// No functions mapped yet.

	unsigned int m_platformCode; // 0x00
	unsigned int m_inputCode;    // 0x04
};

#endif
