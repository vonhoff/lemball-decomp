#ifndef LEMBALL_VISOS_MESSAGING_CMESSREQNEWPORT_H
#define LEMBALL_VISOS_MESSAGING_CMESSREQNEWPORT_H

#include "CMessReqConnect.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00498ed0
class CMessReqNewPort : public CMessReqConnect {
public:
	CMessReqNewPort(const char* p_arg0);
	void AddData();
	void GetData();

	friend class CBaseNetwork;

private:
	unsigned int m_connectionId; // 0x3c
};

extern CMessReqNewPort* g_pMessReqNewPort;

// SYNTHETIC: LEMBALL 0x00462850
// CMessReqNewPort::`scalar deleting destructor'

#endif
