#ifndef LEMBALL_VISOS_MESSAGING_CTRANSPORTOBJECTMESS_H
#define LEMBALL_VISOS_MESSAGING_CTRANSPORTOBJECTMESS_H

#include "../../AI/Messages/CGameObjectMess.h" // complete type

// SIZE 0x34
// VTABLE: LEMBALL 0x00493fe8
class CTransportObjectMess : public CGameObjectMess {
public:
	inline CTransportObjectMess() : CGameObjectMess(0) {}
	virtual void GetData(); // vtable+0x08

private:
	unsigned short m_transportObjectId; // 0x30
};

// SYNTHETIC: LEMBALL 0x00417d50
// CTransportObjectMess::`scalar deleting destructor'

#endif
