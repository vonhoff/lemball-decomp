#ifndef LEMBALL_VISOS_FOUNDATION_LOCALDEBUGOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_LOCALDEBUGOSTREAM_H

#include "VsDebugStreambuf.h"
#include "VsOStream.h"

class LocalDebugOStream : public VsDebugStreambuf, public VsOStream {
public:
	LocalDebugOStream(char* p_buffer, int p_size) : VsIOs(this), VsDebugStreambuf(p_buffer, p_size, 0), VsOStream(this)
	{
	}
};

#endif
