#ifndef LEMBALL_VISOS_NETWORK_CCONNECT_H
#define LEMBALL_VISOS_NETWORK_CCONNECT_H

#include "CRwSocket.h" // complete type
#include "Visos/Network/CBaseCommonSocket.h"

class CNetworkAddress;
class CNetworkMessage;

// SIZE 0x15c
// VTABLE: LEMBALL 0x00499140 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x00499168 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x00499198 CRwSocket
// VTABLE: LEMBALL 0x004991a8 CConnect
class CConnect : public virtual CRwSocket {
public:
	CConnect();
	virtual bool Start(const char* p_localName, const char* p_remoteName) = 0; // vtable+0x00
	virtual void Process();                                                    // vtable+0x04
	virtual void InitSocket() = 0;                                             // vtable+0x08
	virtual void Listen(CNetworkAddress* p_address) = 0;                       // vtable+0x0c
	virtual void Connect() = 0;                                                // vtable+0x10
	virtual void ConnectSetup();                                               // vtable+0x14
	CNetworkMessage* ReceiveAcknowledgement();
	bool CheckConnectTime();
	bool Send(CNetworkMessage& p_message);
	void Closed(int p_notifyPeer);
	void FirstReceive();
	void InitConnect(const char* p_peerName, CNetworkAddress* p_address, short p_port);
	void Kill();
	void PostRead(NetworkEvents p_event, CBasePacket* p_packet);
	void SetConnectTime();
	void Stop();
	~CConnect();

	friend class CBaseNetwork;
	friend class CBulletManager;
	friend class CNetworkOptionsDrawer;
	friend class CNetworkOptionsProc;
	friend class CLevelLoader;
	friend class CAI;
	friend class CLaser;
	friend class CLift;
	friend class CMover;
	friend class CPaintGun;
	friend class CFileConnect;
	friend class CTCPIPConnect;

private:
	char* m_name;                 // 0x08
	CNetworkAddress* m_address;   // 0x0c
	CConnect* m_nextConnect;      // 0x10
	CConnect* m_previousConnect;  // 0x14
	int m_newPortRequestCount;    // 0x18
	unsigned int m_isHost;        // 0x1c
	unsigned int m_killRequested; // 0x20
	unsigned int m_established;   // 0x24
	unsigned int m_connectTime;   // 0x28
};

extern CConnect* g_pActiveConnection;

// SYNTHETIC: LEMBALL 0x00462da0 SYMBOL
// ??_ECConnect@@$4PPPPPPPM@A@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462db0
// CConnect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462e00 SYMBOL
// ??_ECConnect@@$4PPPPPPPM@DA@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462e20 SYMBOL
// ??_ECConnect@@$4PPPPPPPM@KI@AEPAXI@Z

#endif
