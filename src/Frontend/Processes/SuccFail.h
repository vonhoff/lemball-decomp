#ifndef LEMBALL_FRONTEND_PROCESSES_SUCCFAIL_H
#define LEMBALL_FRONTEND_PROCESSES_SUCCFAIL_H

#include "../../Common.h"
#include "../Base/BaseFrontendProcess.h" // complete type

// SIZE 0x2c
// VTABLE: LEMBALL 0x004983c8 BaseQueueHandler
// VTABLE: LEMBALL 0x004983d8 BaseProcess
class SuccFail : public BaseFrontendProcess {
public:
	SuccFail(Game* p_arg0, unsigned int p_arg1);
	virtual ~SuccFail(); // vtable+0x00

private:
	unsigned int m_variant; // 0x28
};

// SYNTHETIC: LEMBALL 0x004511c0
// SuccFail::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004511f0
// SuccFail::`vector deleting destructor'

#endif
