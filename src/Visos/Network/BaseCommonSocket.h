#ifndef LEMBALL_VISOS_NETWORK_BASECOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_BASECOMMONSOCKET_H

#include "../../Common.h"

// SIZE 0x2c
// VTABLE: LEMBALL 0x00498f30
class BaseCommonSocket {
public:
	BaseCommonSocket();
	void CloseSocket();
	void Closed(unsigned char p_arg0);
	void SocketError(NetworkErrors p_arg0);
	~BaseCommonSocket();

private:
	NetworkErrors m_lastError;   // 0x04
	int m_socketHandle;          // 0x08
	unsigned int m_isOpen;       // 0x0c
	unsigned int m_readReady;    // 0x10
	unsigned int m_writeReady;   // 0x14
	unsigned int m_closePending; // 0x18
	unsigned int m_eventPending; // 0x1c
	unsigned short m_port;       // 0x20
	unsigned int m_socketFlags;  // 0x24
	void* m_platformState;       // 0x28
};

#endif
