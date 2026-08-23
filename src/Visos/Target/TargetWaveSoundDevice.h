#ifndef LEMBALL_VISOS_TARGET_TARGETWAVESOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETWAVESOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

// SIZE 0xb8
// VTABLE: LEMBALL 0x0049ac38
class TargetWaveSoundDevice : public BaseSoundDevice {
private:
	undefined m_platformState[0xb4]; // 0x04
};

#endif
