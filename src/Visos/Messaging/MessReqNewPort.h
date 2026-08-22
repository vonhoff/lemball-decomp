#ifndef LEMBALL_VISOS_MESSAGING_MESSREQNEWPORT_H
#define LEMBALL_VISOS_MESSAGING_MESSREQNEWPORT_H

#include "../../Common.h"
#include "MessReqConnect.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00498ed0
class MessReqNewPort : public MessReqConnect {
public:
	MessReqNewPort(const char* p_arg0);
	void AddData();
	void GetData();
	~MessReqNewPort();

private:
	unsigned int m_connectionId; // 0x3c
};

extern MessReqNewPort* g_pMessReqNewPort;
#endif
