#ifndef LEMBALL_VISOS_ANIMATION_INTROANIM_H
#define LEMBALL_VISOS_ANIMATION_INTROANIM_H

#include "../../Common.h"
#include "../../Frontend/Base/BaseFrontendProcess.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00493050
class IntroAnim : public BaseFrontendProcess {
public:
	IntroAnim(Game* p_game) : BaseFrontendProcess(p_game) {}
	virtual ~IntroAnim(); // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00407f40
// IntroAnim::`scalar deleting destructor'


#endif

