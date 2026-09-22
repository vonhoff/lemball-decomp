#ifndef LEMBALL_VISOS_FOUNDATION_CMAINOPTIONS1_H
#define LEMBALL_VISOS_FOUNDATION_CMAINOPTIONS1_H

#include "../../Frontend/Base/CBaseFrontendProcess.h" // complete type

class CGame;

// SIZE 0x28
// VTABLE: LEMBALL 0x00493068 CBaseQueueHandler
// VTABLE: LEMBALL 0x00493078 CBaseProcess
class CMainOptions1 : public CBaseFrontendProcess {
public:
	CMainOptions1(CGame* p_game) : CBaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407f70
// CMainOptions1::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408060
// CMainOptions1::`vector deleting destructor'

#endif
