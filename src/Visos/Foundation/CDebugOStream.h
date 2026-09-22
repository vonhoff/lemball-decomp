#ifndef LEMBALL_VISOS_FOUNDATION_CDEBUGOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_CDEBUGOSTREAM_H

#include "CVsDebugStreambuf.h"
#include "CVsOStream.h"

// Original cleanup uses a vbptr at +0 and the buffer member at +4.
#pragma warning(disable : 4355)
// SIZE 0x16c
// VTABLE: LEMBALL 0x00493020
class CDebugOStream : public virtual CVsOStream {
public:
	CDebugOStream(char* p_buffer, int p_size) : CVsIOs(&m_buffer), CVsOStream(&m_buffer), m_buffer(p_buffer, p_size, 0)
	{
	}
	virtual ~CDebugOStream() {}

private:
	CVsDebugStreambuf m_buffer; // 0x04
};
#pragma warning(default : 4355)

// SYNTHETIC: LEMBALL 0x00407e80
// CDebugOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00493000
// CDebugOStream::`vbtable'{for `CVsOStream'}

// GLOBAL: LEMBALL 0x00493010
// CDebugOStream::`vbtable'{for `CDebugOStream'}

#endif
