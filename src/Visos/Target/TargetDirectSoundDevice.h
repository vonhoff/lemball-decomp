#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x0049acd8
class TargetDirectSoundDevice : public BaseSoundDevice {
public:
	TargetDirectSoundDevice(int p_channelCount, int p_flags);
	virtual ~TargetDirectSoundDevice();
	virtual int Dummy28();

private:
	undefined m_platformState[0x50]; // 0x04
};

#endif
