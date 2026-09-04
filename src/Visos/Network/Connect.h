#ifndef LEMBALL_VISOS_NETWORK_CONNECT_H
#define LEMBALL_VISOS_NETWORK_CONNECT_H

#include "../../Common.h"
#include "RwSocket.h" // complete type

// SIZE 0x15c
// VTABLE: LEMBALL 0x00499140 ReadSocket virtual base
// VTABLE: LEMBALL 0x00499168 WriteSocket virtual base
// VTABLE: LEMBALL 0x00499198 BaseCommonSocket virtual base
// VTABLE: LEMBALL 0x004991a8 primary
class CConnect : public virtual RwSocket {
public:
	CConnect();
	virtual bool Start(const char* p_localName, const char* p_remoteName) = 0; // vtable+0x00
	virtual void Process();                                                    // vtable+0x04
	virtual void InitSocket() = 0;                                             // vtable+0x08
	virtual void Listen(NetworkAddress* p_address) = 0;                        // vtable+0x0c
	virtual void Connect() = 0;                                                // vtable+0x10
	virtual void ConnectSetup();                                               // vtable+0x14
	NetworkMessage* ReceiveAcknowledgement();
	bool CheckConnectTime();
	bool Send(NetworkMessage& p_arg0);
	void Closed(unsigned char p_arg0);
	void FirstReceive();
	void InitConnect(const char* p_arg0, NetworkAddress* p_arg1, short p_arg2);
	void Kill();
	void PostRead(NetworkEvents p_arg0, BasePacket* p_arg1);
	void SetConnectTime();
	void Stop();
	~CConnect();

	friend class BaseNetwork;
	friend class BulletManager;
	friend class NetworkOptionsDrawer;
	friend class NetworkOptionsProc;
	friend class LevelLoader;
	friend class TcpIpConnect;

private:
	char* m_name;                 // 0x08
	NetworkAddress* m_address;    // 0x0c
	CConnect* m_nextConnect;      // 0x10
	CConnect* m_previousConnect;  // 0x14
	int m_newPortRequestCount;    // 0x18
	unsigned int m_isHost;        // 0x1c
	unsigned int m_killRequested; // 0x20
	unsigned int m_established;   // 0x24
	unsigned int m_connectTime;   // 0x28
};

typedef CConnect Connect;

extern Connect* g_pActiveConnection;
#endif
