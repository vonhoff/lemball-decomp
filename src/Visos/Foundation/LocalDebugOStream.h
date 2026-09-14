#ifndef LEMBALL_VISOS_FOUNDATION_LOCALDEBUGOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_LOCALDEBUGOSTREAM_H

#include "VsDebugStreambuf.h"
#include "VsOStream.h"

// Original cleanup uses a vbptr at +0 and the buffer member at +4.
#pragma warning(disable : 4355)
// SIZE 0x16c
// VTABLE: LEMBALL 0x00493020
class LocalDebugOStream : public virtual VsOStream {
public:
	LocalDebugOStream(char* p_buffer, int p_size)
		: VsIOs(&m_buffer), VsOStream(&m_buffer), m_buffer(p_buffer, p_size, 0)
	{
	}
	virtual ~LocalDebugOStream() {}

private:
	VsDebugStreambuf m_buffer; // 0x04
};
#pragma warning(default : 4355)

// SYNTHETIC: LEMBALL 0x00407e80
// LocalDebugOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00493000
// LocalDebugOStream::`vbtable'{for `VsOStream'}

// GLOBAL: LEMBALL 0x00493010
// LocalDebugOStream::`vbtable'{for `LocalDebugOStream'}

#endif
