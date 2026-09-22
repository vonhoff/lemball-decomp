#ifndef LEMBALL_VISOS_FOUNDATION_CARENABASE_H
#define LEMBALL_VISOS_FOUNDATION_CARENABASE_H

class CVsOStream;

// SIZE 0x08
// VTABLE: LEMBALL 0x00498908
class CArenaBase {
public:
	virtual CVsOStream& StreamOut(CVsOStream& p_stream) = 0; // vtable+0x00

protected:
	unsigned int m_signature; // 0x04
};

#endif
