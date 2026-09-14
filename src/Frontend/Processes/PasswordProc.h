#ifndef LEMBALL_FRONTEND_PROCESSES_PASSWORDPROC_H
#define LEMBALL_FRONTEND_PROCESSES_PASSWORDPROC_H

#include "../../Common.h"
#include "../Base/BaseFrontendProcess.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x004930d8 BaseQueueHandler
// VTABLE: LEMBALL 0x004930e8 BaseProcess
class PasswordProc : public BaseFrontendProcess {
public:
	PasswordProc(Game* p_game) : BaseFrontendProcess(p_game) {}
	virtual ~PasswordProc(); // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00407fd0
// PasswordProc::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408040
// PasswordProc::`vector deleting destructor'

#endif
