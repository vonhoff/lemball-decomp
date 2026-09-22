#ifndef LEMBALL_FRONTEND_PROCESSES_CSUCCFAIL_H
#define LEMBALL_FRONTEND_PROCESSES_CSUCCFAIL_H

#include "../Base/CBaseFrontendProcess.h" // complete type

class CGame;
// SIZE 0x2c
// VTABLE: LEMBALL 0x004983c8 CBaseQueueHandler
// VTABLE: LEMBALL 0x004983d8 CBaseProcess
class CSuccFail : public CBaseFrontendProcess {
public:
	CSuccFail(CGame* p_arg0, unsigned int p_arg1);

private:
	unsigned int m_variant; // 0x28
};

// SYNTHETIC: LEMBALL 0x004511c0
// CSuccFail::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004511f0
// CSuccFail::`vector deleting destructor'

#endif
