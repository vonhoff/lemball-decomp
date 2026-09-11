#ifndef LEMBALL_VISOS_TARGET_IDIRECTSOUNDBUFFER_H
#define LEMBALL_VISOS_TARGET_IDIRECTSOUNDBUFFER_H

class IDirectSound;

class IDirectSoundBuffer {
public:
	virtual long __stdcall QueryInterface(const void* p_interfaceId, void** p_object) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall GetCaps(void* p_caps) = 0;
	virtual long __stdcall GetCurrentPosition(unsigned long* p_playCursor, unsigned long* p_writeCursor) = 0;
	virtual long __stdcall GetFormat(void* p_format, unsigned long p_size, unsigned long* p_written) = 0;
	virtual long __stdcall GetVolume(long* p_volume) = 0;
	virtual long __stdcall GetPan(long* p_pan) = 0;
	virtual long __stdcall GetFrequency(unsigned long* p_frequency) = 0;
	virtual long __stdcall GetStatus(unsigned long* p_status) = 0;
	virtual long __stdcall Initialize(IDirectSound* p_directSound, const void* p_description) = 0;
	virtual long __stdcall Lock(unsigned long p_offset,
								unsigned long p_size,
								void** p_audio1,
								unsigned long* p_audioBytes1,
								void** p_audio2,
								unsigned long* p_audioBytes2,
								unsigned long p_flags) = 0;
	virtual long __stdcall Play(unsigned long p_reserved1, unsigned long p_priority, unsigned long p_flags) = 0;
	virtual long __stdcall SetCurrentPosition(unsigned long p_position) = 0;
	virtual long __stdcall SetFormat(const void* p_format) = 0;
	virtual long __stdcall SetVolume(long p_volume) = 0;
};

#endif
