#ifndef LEMBALL_VISOS_FOUNDATION_PROCESS_H
#define LEMBALL_VISOS_FOUNDATION_PROCESS_H

#include "../../Common.h"

// SIZE 0x0c
// VTABLE: LEMBALL 0x004930a0
class Process {
public:
	virtual ~Process(); // vtable+0x00

private:
	int m_returnState;  // 0x04
	int m_processState; // 0x08
};

#endif
