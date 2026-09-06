#ifndef LEMBALL_VISOS_SOUND_PVMUSICDEVICE_H
#define LEMBALL_VISOS_SOUND_PVMUSICDEVICE_H

#include "../../Common.h"
#include "../Foundation/String.h" // complete type

// SIZE 0x18
// VTABLE: LEMBALL 0x0049adb0
class PvMusicDevice {
public:
	// 68K 0x101023b4 __dt__14CPVMusicDeviceFv
	virtual ~PvMusicDevice() {}                                                   // vtable+0x00
	virtual void Initialise(unsigned long p_resourceId, unsigned long p_flags);   // vtable+0x04
	virtual void Prepare(unsigned long p_handle, unsigned long p_resourceId) = 0; // vtable+0x08
	virtual void Free(unsigned long p_handle) = 0;                                // vtable+0x0c
	virtual void Play(unsigned long p_handle) = 0;                                // vtable+0x10
	virtual void Stop(unsigned long p_handle) = 0;                                // vtable+0x14
	virtual void Pause(unsigned long p_handle) = 0;                               // vtable+0x18
	virtual void Resume(unsigned long p_handle) = 0;                              // vtable+0x1c
	virtual void SetVolume(unsigned char p_volume);                               // vtable+0x20
	virtual unsigned char GetVolume();                                            // vtable+0x24
	virtual void Process();                                                       // vtable+0x28
	virtual int IsAvailable() = 0;                                                // vtable+0x2c
	virtual char* GetInfo();                                                      // vtable+0x30

	friend class SoundManager;
	friend class MciMusicDevice;

private:
	unsigned char m_volume;        // 0x04
	unsigned int m_usePathPrefix;  // 0x08
	unsigned int m_useCdDirectory; // 0x0c
	String m_path;                 // 0x10
};

// SYNTHETIC: LEMBALL 0x0047f560
// PvMusicDevice::`scalar deleting destructor'

#endif
