#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDEFFECT_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDEFFECT_H

#include "../../Common.h"

// SIZE 0x1c
class TargetDirectSoundEffect {
private:
	unsigned int m_prepared; // 0x00
	undefined4 m_unknown04;  // 0x04
	void** m_buffers;        // 0x08
	undefined4 m_unknown0c;  // 0x0c
	int m_bufferCount;       // 0x10
	undefined4 m_unknown14;  // 0x14
	unsigned int m_handle;   // 0x18
};

#endif
