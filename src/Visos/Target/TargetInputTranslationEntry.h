#ifndef LEMBALL_VISOS_TARGET_TARGETINPUTTRANSLATIONENTRY_H
#define LEMBALL_VISOS_TARGET_TARGETINPUTTRANSLATIONENTRY_H

#include "../../Common.h"

// SIZE 0x08
struct TargetInputTranslationEntry {
	unsigned int m_platformCode; // 0x00
	unsigned int m_inputCode;    // 0x04
};

#endif
