#ifndef LEMBALL_VISOS_FOUNDATION_MAINOPTIONS2_H
#define LEMBALL_VISOS_FOUNDATION_MAINOPTIONS2_H

#include "../../Frontend/Base/BaseFrontendProcess.h" // complete type

class Game;

// SIZE 0x28
// VTABLE: LEMBALL 0x004930b0 BaseQueueHandler
// VTABLE: LEMBALL 0x004930c0 BaseProcess
class MainOptions2 : public BaseFrontendProcess {
public:
	MainOptions2(Game* p_game) : BaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407fa0
// MainOptions2::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408050
// MainOptions2::`vector deleting destructor'

#endif
