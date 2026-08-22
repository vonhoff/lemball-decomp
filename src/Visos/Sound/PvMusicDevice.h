#ifndef LEMBALL_VISOS_SOUND_PVMUSICDEVICE_H
#define LEMBALL_VISOS_SOUND_PVMUSICDEVICE_H

#include "../../Common.h"
#include "../Foundation/String.h" // complete type

// SIZE 0x18
// VTABLE: LEMBALL 0x0049adb0
class PvMusicDevice {
public:
	virtual char* GetInfo();                                                    // vtable+0x30
	virtual unsigned char GetVolume();                                          // vtable+0x24
	virtual void Initialise(unsigned long p_resourceId, unsigned long p_flags); // vtable+0x04
	virtual void Process();                                                     // vtable+0x28
	virtual void SetVolume(unsigned char p_volume);                             // vtable+0x20
	virtual ~PvMusicDevice();                                                   // vtable+0x00

private:
	unsigned char m_volume;    // 0x04
	unsigned int m_reserved08; // 0x08
	unsigned int m_reserved0c; // 0x0c
	String m_path;             // 0x10
};

#endif
