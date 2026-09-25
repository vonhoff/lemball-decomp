#ifndef LEMBALL_VISOS_ANIMATION_CINTROANIM_H
#define LEMBALL_VISOS_ANIMATION_CINTROANIM_H

#include "../../Frontend/Base/CBaseFrontendProcess.h"

class CGame;

// SIZE 0x28
// VTABLE: LEMBALL 0x00493040 CBaseQueueHandler
// VTABLE: LEMBALL 0x00493050 CBaseProcess
class CIntroAnim : public CBaseFrontendProcess {
public:
	CIntroAnim(CGame* p_game) : CBaseFrontendProcess(p_game) {}
};

// SYNTHETIC: LEMBALL 0x00407f40
// CIntroAnim::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00408070
// CIntroAnim::`vector deleting destructor'

#endif
