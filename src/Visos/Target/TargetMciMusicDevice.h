#ifndef LEMBALL_VISOS_TARGET_TARGETMCIMUSICDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETMCIMUSICDEVICE_H

#include "../../Common.h"
#include "../Sound/PvMusicDevice.h" // complete type

#define WIN32_LEAN_AND_MEAN
// clang-format off: mmsystem.h requires the Win32 types declared by windows.h.
#include <windows.h>
#include <mmsystem.h>
// clang-format on

// SIZE 0x34
// VTABLE: LEMBALL 0x0049ad78
class MciMusicDevice : public PvMusicDevice {
public:
	MciMusicDevice();
	virtual ~MciMusicDevice();
	virtual void Prepare(unsigned long p_handle, unsigned long p_resourceId);
	virtual void Free(unsigned long p_handle);
	virtual void Play(unsigned long p_handle);
	virtual void Stop(unsigned long p_handle);
	virtual void Pause(unsigned long p_handle);
	virtual void Resume(unsigned long p_handle);
	virtual int IsAvailable();
	virtual char* GetInfo();

private:
	unsigned int m_preparedHandle; // 0x18
	unsigned int m_available;      // 0x1c
	MCIDEVICEID m_deviceId;        // 0x20
	unsigned int m_playing;        // 0x24
	unsigned int m_paused;         // 0x28
	DWORD m_pausePosition;         // 0x2c
	HWND m_notifyWindow;           // 0x30
};

// SYNTHETIC: LEMBALL 0x0047f590
// MciMusicDevice::`scalar deleting destructor'

#endif
