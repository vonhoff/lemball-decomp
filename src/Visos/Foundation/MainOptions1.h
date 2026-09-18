#ifndef LEMBALL_VISOS_FOUNDATION_MAINOPTIONS1_H
#define LEMBALL_VISOS_FOUNDATION_MAINOPTIONS1_H

#include "../../Frontend/Base/BaseFrontendProcess.h" // complete type

class Game;

// SIZE 0x28
// VTABLE: LEMBALL 0x00493068 BaseQueueHandler
// VTABLE: LEMBALL 0x00493078 BaseProcess
class MainOptions1 : public BaseFrontendProcess {
public:
	MainOptions1(Game* p_game) : BaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407f70
// MainOptions1::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408060
// MainOptions1::`vector deleting destructor'

#endif
