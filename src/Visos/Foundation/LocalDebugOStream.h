#ifndef LEMBALL_VISOS_FOUNDATION_LOCALDEBUGOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_LOCALDEBUGOSTREAM_H

#include "VsDebugStreambuf.h"
#include "VsOStream.h"

// Original x86 uses the first base address to initialize both virtual stream paths.
#pragma warning(disable : 4355)
// SIZE 0x16c
class LocalDebugOStream : public VsDebugStreambuf, public virtual VsOStream {
public:
	LocalDebugOStream(char* p_buffer, int p_size) : VsIOs(this), VsDebugStreambuf(p_buffer, p_size, 0), VsOStream(this)
	{
	}
};
#pragma warning(default : 4355)

#endif
