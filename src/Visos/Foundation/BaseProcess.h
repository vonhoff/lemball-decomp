#ifndef LEMBALL_VISOS_FOUNDATION_BASEPROCESS_H
#define LEMBALL_VISOS_FOUNDATION_BASEPROCESS_H

#include "../../Common.h"

// SIZE 0x0c
// VTABLE: LEMBALL 0x004930a0
class BaseProcess {
public:
	virtual ~BaseProcess();        // vtable+0x00
	virtual void Process() = 0; // vtable+0x04

	friend class Game;
	friend class BaseFrontendProcess;
	friend class TargetAbout;

private:
	int m_returnState;  // 0x04
	int m_processState; // 0x08
};

// SYNTHETIC: LEMBALL 0x00407ef0
// CProcess::`scalar deleting destructor'


#endif
