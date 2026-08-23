#ifndef LEMBALL_AI_BASE_STATEENTRY_H
#define LEMBALL_AI_BASE_STATEENTRY_H

#include "../../Common.h"

// SIZE 0x10
struct StateEntry {
	void* m_predicate;      // 0x00
	void* m_actionFunction; // 0x04
	eAction m_nextAction;   // 0x08
	int m_actionArgument;   // 0x0c
};

#endif
