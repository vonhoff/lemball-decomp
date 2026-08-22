#ifndef LEMBALL_VISOS_NETWORK_CONNECT_H
#define LEMBALL_VISOS_NETWORK_CONNECT_H

#include "../../Common.h"
#include "ReadSocket.h"  // complete type
#include "WriteSocket.h" // complete type

// SIZE 0x15c
// VTABLE: LEMBALL 0x00499140 ReadSocket virtual base
// VTABLE: LEMBALL 0x00499168 WriteSocket virtual base
// VTABLE: LEMBALL 0x00499198 BaseCommonSocket virtual base
// VTABLE: LEMBALL 0x004991a8 primary
class Connect : public virtual ReadSocket, public virtual WriteSocket {
public:
	Connect();
	NetworkMessage* ReceiveAcknowledgement();
	bool CheckConnectTime();
	char Send(NetworkMessage& p_arg0);
	void Closed(unsigned char p_arg0);
	void ConnectSetup();
	void FirstReceive();
	void InitConnect(const char* p_arg0, NetworkAddress* p_arg1, short p_arg2);
	void Kill();
	void PostRead(NetworkEvents p_arg0, BasePacket* p_arg1);
	void Process();
	void SetConnectTime();
	void Stop();
	~Connect();

private:
	char* m_name;                       // 0x08
	NetworkAddress* m_address;          // 0x0c
	class Connect* m_nextConnect;       // 0x10
	class Connect* m_previousConnect;   // 0x14
	unsigned int m_newPortRequestCount; // 0x18
	unsigned int m_killRequested;       // 0x20
	unsigned int m_established;         // 0x24
	unsigned int m_connectTime;         // 0x28
};

extern Connect* g_pActiveConnection;
#endif
