#ifndef LEMBALL_VISOS_NETWORK_CBASECOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_CBASECOMMONSOCKET_H

#include "NetworkConstants.h"

// SIZE 0x2c
// VTABLE: LEMBALL 0x00498f30
class CBaseCommonSocket {
public:
	CBaseCommonSocket();
	virtual int SysCloseSocket() = 0;      // vtable+0x00
	virtual ~CBaseCommonSocket();          // vtable+0x04
	virtual void SocketError() = 0;        // vtable+0x08
	virtual void Closed(int p_notifyPeer); // vtable+0x0c
	void CloseSocket();
	void SocketError(NetworkErrors p_error);

	friend class CWriteSocket;
	friend class CConnect;
	friend class CBroadcast;
	friend class CReadSocket;
	friend class CFileReadSocket;
	friend class CFileWriteSocket;
	friend class CFileBroadcast;
	friend class CFileConnect;
	friend class CTCPIPReadSocket;
	friend class CTCPIPWriteSocket;
	friend class CTCPIPBroadcast;
	friend class CTCPIPConnect;
	friend class CTCPIPCommonSocket;
	friend class CBaseNetwork;

private:
	NetworkErrors m_lastError;      // 0x04
	unsigned int m_socketHandle;    // 0x08
	bool m_isOpen;                  // 0x0c
	bool m_readReady;               // 0x10
	bool m_writeReady;              // 0x14
	bool m_closePending;            // 0x18
	bool m_eventPending;            // 0x1c
	short m_port;                   // 0x20
	bool m_socketFlags;             // 0x24
	unsigned char* m_platformState; // 0x28
};

// SYNTHETIC: LEMBALL 0x004628e0
// CBaseCommonSocket::`scalar deleting destructor'

#endif
