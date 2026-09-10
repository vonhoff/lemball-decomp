#ifndef LEMBALL_VISOS_FOUNDATION_ARENABASE_H
#define LEMBALL_VISOS_FOUNDATION_ARENABASE_H

#include "../../Common.h"

// SIZE 0x08
// VTABLE: LEMBALL 0x00498908
class ArenaBase {
public:
	virtual VsOStream& StreamOut(VsOStream& p_stream) = 0; // vtable+0x00

protected:
	unsigned int m_signature; // 0x04
};

#endif
