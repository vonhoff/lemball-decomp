#ifndef LEMBALL_VISOS_TARGET_IDIRECTSOUND_H
#define LEMBALL_VISOS_TARGET_IDIRECTSOUND_H

class IDirectSound {
public:
	virtual long __stdcall QueryInterface(const void* p_interfaceId, void** p_object) = 0;
	virtual unsigned long __stdcall AddRef() = 0;
	virtual unsigned long __stdcall Release() = 0;
};

#endif
