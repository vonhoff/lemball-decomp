#ifndef LEMBALL_PLATFORM_DIRECTX_DIRECTSOUND_H
#define LEMBALL_PLATFORM_DIRECTX_DIRECTSOUND_H

struct tWAVEFORMATEX;

#define DSBCAPS_PRIMARYBUFFER 0x00000001
#define DSBCAPS_STATIC 0x00000002
#define DSBCAPS_CTRLFREQUENCY 0x00000020
#define DSBCAPS_CTRLPAN 0x00000040
#define DSBCAPS_CTRLVOLUME 0x00000080

// SIZE 0x14
struct DSBUFFERDESC {
	unsigned long dwSize;
	unsigned long dwFlags;
	unsigned long dwBufferBytes;
	unsigned long dwReserved;
	tWAVEFORMATEX* lpwfxFormat;
};

class IDirectSoundBuffer;

#define DSSCL_PRIORITY 0x00000002

class IDirectSound {
public:
	virtual long __stdcall QueryInterface(const void* p_interfaceId, void** p_object) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall CreateSoundBuffer(const DSBUFFERDESC* p_description,
											 IDirectSoundBuffer** p_buffer,
											 void* p_outer) = 0;
	virtual long __stdcall GetCaps(void* p_caps) = 0;
	virtual long __stdcall DuplicateSoundBuffer(IDirectSoundBuffer* p_original, IDirectSoundBuffer** p_duplicate) = 0;
	virtual long __stdcall SetCooperativeLevel(void* p_window, unsigned long p_level) = 0;
};

class IDirectSoundBuffer {
public:
	virtual long __stdcall QueryInterface(const void* p_interfaceId, void** p_object) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
	virtual long __stdcall GetCaps(void* p_caps) = 0;
	virtual long __stdcall GetCurrentPosition(unsigned long* p_playCursor, unsigned long* p_writeCursor) = 0;
	virtual long __stdcall GetFormat(tWAVEFORMATEX* p_format, unsigned long p_size, unsigned long* p_written) = 0;
	virtual long __stdcall GetVolume(long* p_volume) = 0;
	virtual long __stdcall GetPan(long* p_pan) = 0;
	virtual long __stdcall GetFrequency(unsigned long* p_frequency) = 0;
	virtual long __stdcall GetStatus(unsigned long* p_status) = 0;
	virtual long __stdcall Initialize(IDirectSound* p_directSound, const DSBUFFERDESC* p_description) = 0;
	virtual long __stdcall Lock(unsigned long p_offset,
								unsigned long p_size,
								void** p_audio1,
								unsigned long* p_audioBytes1,
								void** p_audio2,
								unsigned long* p_audioBytes2,
								unsigned long p_flags) = 0;
	virtual long __stdcall Play(unsigned long p_reserved1, unsigned long p_priority, unsigned long p_flags) = 0;
	virtual long __stdcall SetCurrentPosition(unsigned long p_position) = 0;
	virtual long __stdcall SetFormat(const tWAVEFORMATEX* p_format) = 0;
	virtual long __stdcall SetVolume(long p_volume) = 0;
	virtual long __stdcall SetPan(long p_pan) = 0;
	virtual long __stdcall SetFrequency(unsigned long p_frequency) = 0;
	virtual long __stdcall Stop() = 0;
	virtual long __stdcall Unlock(void* p_audio1,
								  unsigned long p_audioBytes1,
								  void* p_audio2,
								  unsigned long p_audioBytes2) = 0;
};

#endif
