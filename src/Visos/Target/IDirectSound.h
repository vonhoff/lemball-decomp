#ifndef LEMBALL_VISOS_TARGET_IDIRECTSOUND_H
#define LEMBALL_VISOS_TARGET_IDIRECTSOUND_H

class IDirectSoundBuffer;

class IDirectSound {
public:
	virtual long __stdcall QueryInterface(const void* p_interfaceId, void** p_object) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall CreateSoundBuffer(const void* p_description,
											 IDirectSoundBuffer** p_buffer,
											 void* p_outer) = 0;
	virtual long __stdcall GetCaps(void* p_caps) = 0;
	virtual long __stdcall DuplicateSoundBuffer(IDirectSoundBuffer* p_original, IDirectSoundBuffer** p_duplicate) = 0;
	virtual long __stdcall SetCooperativeLevel(void* p_window, unsigned long p_level) = 0;
};

#endif
