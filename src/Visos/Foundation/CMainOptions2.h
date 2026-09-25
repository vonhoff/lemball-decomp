#ifndef LEMBALL_VISOS_FOUNDATION_CMAINOPTIONS2_H
#define LEMBALL_VISOS_FOUNDATION_CMAINOPTIONS2_H

#include "../../Frontend/Base/CBaseFrontendProcess.h"

class CGame;

// SIZE 0x28
// VTABLE: LEMBALL 0x004930b0 CBaseQueueHandler
// VTABLE: LEMBALL 0x004930c0 CBaseProcess
class CMainOptions2 : public CBaseFrontendProcess {
public:
	CMainOptions2(CGame* p_game) : CBaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407fa0
// CMainOptions2::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408050
// CMainOptions2::`vector deleting destructor'

#endif
