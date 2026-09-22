#ifndef LEMBALL_VISOS_NETWORK_CBASECOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_CBASECOMMONSOCKET_H

enum NetworkErrors {
	NETWORK_ERROR_NONE = 0,
	NETWORK_ERROR_1 = 1,
	NETWORK_ERROR_2 = 2,
	NETWORK_ERROR_3 = 3,
	NETWORK_ERROR_4 = 4,
	NETWORK_ERROR_5 = 5
};

enum NetworkEvents {
	NETWORK_EVENT_5 = 5,
	NETWORK_EVENT_7 = 7,
	NETWORK_EVENT_8 = 8,
	NETWORK_EVENT_9 = 9,
	NETWORK_EVENT_10 = 10,
	NETWORK_EVENT_13 = 13
};

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
	void SocketError(NetworkErrors p_arg0);

	friend class CWriteSocket;
	friend class CConnect;
	friend class CBroadcast;
	friend class CReadSocket;
	friend class CFileReadSocket;
	friend class CFileWriteSocket;
	friend class CFileBroadcast;
	friend class CFileConnect;
	friend class CTcpIpReadSocket;
	friend class CTcpIpWriteSocket;
	friend class CTcpIpBroadcast;
	friend class CTcpIpConnect;
	friend class CTcpIpCommonSocket;
	friend class CBaseNetwork;

private:
	NetworkErrors m_lastError;   // 0x04
	int m_socketHandle;          // 0x08
	unsigned int m_isOpen;       // 0x0c
	unsigned int m_readReady;    // 0x10
	unsigned int m_writeReady;   // 0x14
	unsigned int m_closePending; // 0x18
	unsigned int m_eventPending; // 0x1c
	short m_port;                // 0x20
	unsigned int m_socketFlags;  // 0x24
	void* m_platformState;       // 0x28
};

// SYNTHETIC: LEMBALL 0x004628e0
// CBaseCommonSocket::`scalar deleting destructor'

#endif
