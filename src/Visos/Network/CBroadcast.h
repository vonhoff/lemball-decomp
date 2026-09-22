#ifndef LEMBALL_VISOS_NETWORK_CBROADCAST_H
#define LEMBALL_VISOS_NETWORK_CBROADCAST_H

#include "CRwSocket.h" // complete type
#include "Visos/Network/CBaseCommonSocket.h"

class CBroadcastMessage;
class CNetworkAddress;

// SIZE 0x128
// VTABLE: LEMBALL 0x00499080 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x004990a8 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x004990d8 CRwSocket
// VTABLE: LEMBALL 0x004990e8 CBroadcast
class CBroadcast : public virtual CRwSocket {
public:
	CBroadcast();
	virtual short FindPort(const unsigned char* p_peerPortUsage); // vtable+0x00
	virtual void ResetPort(short p_port);                         // vtable+0x04
	virtual void GetSpecificAddr(const char* p_address) = 0;      // vtable+0x08
	virtual bool Start(const char* p_networkName) = 0;            // vtable+0x0c
	virtual void Process();                                       // vtable+0x10
	virtual void StartListen() = 0;                               // vtable+0x14
	virtual void StopListen() = 0;                                // vtable+0x18
	void AddToMessage(CBroadcastMessage& p_message);
	void Closed(int p_notifyPeer);
	void Initialise(const char* p_networkName);
	void PostRead(NetworkEvents p_event, CBasePacket* p_packet);
	void Run();
	void Send(CNetworkAddress* p_address, CBroadcastMessage& p_message);
	void SendFailedInit(NetworkErrors p_error);
	void SetSpecificAddr(const char* p_address);
	void Stop();
	void Suspend();
	~CBroadcast();

	friend class CBaseNetwork;
	friend class CNetworkManager;
	friend class CTCPIPBroadcast;

protected:
	unsigned int m_runEnabled;          // 0x08
	unsigned int m_addressMode;         // 0x0c
	unsigned char* m_connectionData;    // 0x10
	unsigned int m_listenEnabled;       // 0x14
	unsigned int m_lastBroadcastTime;   // 0x18
	CNetworkAddress* m_specificAddress; // 0x1c
};

// SYNTHETIC: LEMBALL 0x00462bb0 SYMBOL
// ??_ECBroadcast@@$4PPPPPPPM@A@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462bc0
// CBroadcast::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462c10 SYMBOL
// ??_ECBroadcast@@$4PPPPPPPM@DA@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462c20 SYMBOL
// ?SendAcknowledgement@CRwSocket@@$4PPPPPPPM@PPPPPOPI@AEXXZ

// SYNTHETIC: LEMBALL 0x00462c40 SYMBOL
// ?ReceiveAcknowledgement@CRwSocket@@$4PPPPPPPM@PPPPPOPI@AEPAVCNetworkMessage@@XZ

// SYNTHETIC: LEMBALL 0x00462c60 SYMBOL
// ??_ECBroadcast@@$4PPPPPPPM@KI@AEPAXI@Z

#endif
