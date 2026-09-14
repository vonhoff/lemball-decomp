#ifndef LEMBALL_VISOS_ANIMATION_INTROANIM_H
#define LEMBALL_VISOS_ANIMATION_INTROANIM_H

#include "../../Common.h"
#include "../../Frontend/Base/BaseFrontendProcess.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00493040 BaseQueueHandler
// VTABLE: LEMBALL 0x00493050 BaseProcess
class IntroAnim : public BaseFrontendProcess {
public:
	IntroAnim(Game* p_game) : BaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407f40
// IntroAnim::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408070
// IntroAnim::`vector deleting destructor'

#endif
