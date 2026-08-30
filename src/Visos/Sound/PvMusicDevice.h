#ifndef LEMBALL_VISOS_SOUND_PVMUSICDEVICE_H
#define LEMBALL_VISOS_SOUND_PVMUSICDEVICE_H

#include "../../Common.h"
#include "../Foundation/String.h" // complete type

// SIZE 0x18
// VTABLE: LEMBALL 0x0049adb0
class PvMusicDevice {
public:
	virtual ~PvMusicDevice();                                                    // vtable+0x00
	virtual void Initialise(unsigned long p_resourceId, unsigned long p_flags);  // vtable+0x04
	virtual void Dummy08(unsigned long p_allocated, unsigned long p_resourceId); // vtable+0x08
	virtual void Dummy0c(unsigned long p_handle);                                // vtable+0x0c
	virtual void Dummy10(unsigned long p_handle);                                // vtable+0x10
	virtual void Dummy14(unsigned long p_handle);                                // vtable+0x14
	virtual void Dummy18(unsigned long p_handle);                                // vtable+0x18
	virtual void Dummy1c(unsigned long p_handle);                                // vtable+0x1c
	virtual void SetVolume(unsigned char p_volume);                              // vtable+0x20
	virtual unsigned char GetVolume();                                           // vtable+0x24
	virtual void Process();                                                      // vtable+0x28
	virtual int Dummy2c();                                                       // vtable+0x2c
	virtual char* GetInfo();                                                     // vtable+0x30

	friend class SoundManager;
	friend class MciMusicDevice;

private:
	unsigned char m_volume;    // 0x04
	unsigned int m_reserved08; // 0x08
	unsigned int m_reserved0c; // 0x0c
	String m_path;             // 0x10
};

// SYNTHETIC: LEMBALL 0x0047f560
// PvMusicDevice::`scalar deleting destructor'

#endif
