#ifndef LEMBALL_VISOS_FOUNDATION_CDEBUGOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_CDEBUGOSTREAM_H

#include "CVSDebugStreambuf.h"
#include "CVSOStream.h"

#pragma warning(disable : 4355)
// SIZE 0x16c
// VTABLE: LEMBALL 0x00493020
class CDebugOStream : public virtual CVSOStream {
public:
	CDebugOStream(char* p_buffer, int p_size) : CVSIOs(&m_buffer), CVSOStream(&m_buffer), m_buffer(p_buffer, p_size, 0)
	{
	}
	virtual ~CDebugOStream() {}

private:
	CVSDebugStreambuf m_buffer; // 0x04
};
#pragma warning(default : 4355)

// SYNTHETIC: LEMBALL 0x00407e80
// CDebugOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00493000
// CDebugOStream::`vbtable'{for `CVSOStream'}

// GLOBAL: LEMBALL 0x00493010
// CDebugOStream::`vbtable'{for `CDebugOStream'}

#endif
