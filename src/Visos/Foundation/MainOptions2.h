#ifndef LEMBALL_VISOS_FOUNDATION_MAINOPTIONS2_H
#define LEMBALL_VISOS_FOUNDATION_MAINOPTIONS2_H

#include "../../Common.h"
#include "../../Frontend/Base/BaseFrontendProcess.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x004930c0
class MainOptions2 : public BaseFrontendProcess {
public:
	MainOptions2(Game* p_game) : BaseFrontendProcess(p_game) {}
	virtual ~MainOptions2(); // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00407fa0
// MainOptions2::`scalar deleting destructor'

#endif
