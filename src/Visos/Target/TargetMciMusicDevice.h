#ifndef LEMBALL_VISOS_TARGET_TARGETMCIMUSICDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETMCIMUSICDEVICE_H

#include "../../Common.h"
#include "../Sound/PvMusicDevice.h" // complete type

// SIZE 0x34
// VTABLE: LEMBALL 0x0049ad78
class MciMusicDevice : public PvMusicDevice {
public:
	MciMusicDevice();
	virtual ~MciMusicDevice();
	virtual void Dummy08(unsigned long p_allocated, unsigned long p_resourceId);
	virtual void Dummy0c(unsigned long p_handle);
	virtual void Dummy10(unsigned long p_handle);
	virtual void Dummy14(unsigned long p_handle);
	virtual void Dummy18(unsigned long p_handle);
	virtual void Dummy1c(unsigned long p_handle);
	virtual int Dummy2c();
	virtual char* GetInfo();

private:
	unsigned int m_preparedHandle; // 0x18
	unsigned int m_available;      // 0x1c
	unsigned int m_deviceId;       // 0x20
	unsigned int m_playing;        // 0x24
	unsigned int m_paused;         // 0x28
	unsigned int m_pausePosition;  // 0x2c
	void* m_notifyWindow;          // 0x30
};

#endif
