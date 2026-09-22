#ifndef LEMBALL_FRONTEND_PROCESSES_CPASSWORDPROC_H
#define LEMBALL_FRONTEND_PROCESSES_CPASSWORDPROC_H

#include "../Base/CBaseFrontendProcess.h" // complete type

class CGame;
// SIZE 0x28
// VTABLE: LEMBALL 0x004930d8 CBaseQueueHandler
// VTABLE: LEMBALL 0x004930e8 CBaseProcess
class CPasswordProc : public CBaseFrontendProcess {
public:
	CPasswordProc(CGame* p_game) : CBaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407fd0
// CPasswordProc::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408040
// CPasswordProc::`vector deleting destructor'

#endif
