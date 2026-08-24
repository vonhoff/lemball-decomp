#include "BaseNetwork.h"

#include "../Messaging/MessFailedConnect.h"
#include "../Messaging/MessGoConnect.h"
#include "../Messaging/MessOkConnect.h"
#include "../Messaging/MessReqConnect.h"
#include "../Messaging/MessReqNewPort.h"
#include "../Messaging/NetworkMessage.h"
#include "Connect.h"
#include "NetworkAddress.h"

// 68K 0x1020e61c __ct__12CBaseNetworkFv
// STUB: LEMBALL 0x004619f0
BaseNetwork::BaseNetwork()
{
}

// 68K 0x1020e6fc Initialise__12CBaseNetworkFPCci
// STUB: LEMBALL 0x00461aa0
bool BaseNetwork::Initialise(const char* p_arg0, int p_arg1)
{
	return 0;
}

// 68K 0x1020e886 DoInitialise__12CBaseNetworkFv
// STUB: LEMBALL 0x00461bd0
bool BaseNetwork::DoInitialise()
{
	// STRING: LEMBALL 0x004a1ee8 "Request Connect"
	// STRING: LEMBALL 0x004a1ef8 "Request New Port"
	// STRING: LEMBALL 0x004a1f0c "Authorise Connect"
	// STRING: LEMBALL 0x004a1f20 "Go Ahead Connect"
	// STRING: LEMBALL 0x004a1f34 "Failed Connect"
	return 0;
}

// 68K 0x1020ea84 __dt__12CBaseNetworkFv
// STUB: LEMBALL 0x00461db0
BaseNetwork::~BaseNetwork()
{
}

// 68K 0x1020eb50 ShutDown__12CBaseNetworkFv
// STUB: LEMBALL 0x00461e10
void BaseNetwork::ShutDown()
{
}

// 68K 0x1020eda0 Delete__12CBaseNetworkFP8CConnect
// STUB: LEMBALL 0x00461fc0
void BaseNetwork::Delete(Connect* p_arg0)
{
}

// 68K 0x1020ee72 NewConnect__12CBaseNetworkFv
// STUB: LEMBALL 0x00462040
Connect* BaseNetwork::NewConnect()
{
	return 0;
}

// 68K 0x1020ef72 Exists__12CBaseNetworkFP8CConnect
// FUNCTION: LEMBALL 0x00462130
bool BaseNetwork::Exists(Connect* p_arg0)
{
	Connect* peer;

	peer = m_firstConnect;
	while (peer != 0) {
		if (p_arg0 == peer) {
			if (peer->CheckConnectTime() == 0) {
				return 0;
			}
			if (peer->m_killRequested == 0) {
				peer->SetConnectTime();
				return 1;
			}
			return 0;
		}
		peer = peer->m_nextConnect;
	}
	return 0;
}

// 68K 0x1020efec FindConnection__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x00462180
bool BaseNetwork::FindConnection(NetworkAddress* p_arg0)
{
	return 0;
}

// 68K 0x1020f06e KillUnBornConnection__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x004621c0
bool BaseNetwork::KillUnBornConnection(NetworkAddress* p_arg0)
{
	return 0;
}

// 68K 0x1020f0d2 CtoSRequestConnect__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x004621e0
void BaseNetwork::CtoSRequestConnect(NetworkAddress* p_arg0)
{
}

// 68K 0x1020f1cc CtoSRequestNewPort__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x00462280
void BaseNetwork::CtoSRequestNewPort(NetworkAddress* p_arg0)
{
	// STRING: LEMBALL 0x004a1f44 "To many new-port requests"
}

// 68K 0x1020f2fc StoCOKConnect__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x00462340
void BaseNetwork::StoCokConnect(NetworkAddress* p_arg0)
{
}

// 68K 0x1020f47c StoCFAILEDConnect__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x00462460
void BaseNetwork::StoCfailedConnect(NetworkAddress* p_arg0)
{
}

// 68K 0x1020f4d2 CtoSGOConnect__12CBaseNetworkFP15CNetworkAddress
// STUB: LEMBALL 0x00462480
void BaseNetwork::CtoSgoConnect(NetworkAddress* p_arg0)
{
}

// 68K 0x1020f53c Establish__12CBaseNetworkFP15CNetworkAddressPUc
// FUNCTION: LEMBALL 0x004624a0
void BaseNetwork::Establish(NetworkAddress* p_arg0, unsigned char* p_arg1)
{
	(*(void(__fastcall**)(NetworkAddress*)) * (void***) p_arg0)(p_arg0);
	if (g_pMessReqConnect->Set(p_arg1) != 0) {
		CtoSRequestConnect(p_arg0);
		return;
	}
	if (g_pMessReqNewPort->Set(p_arg1) != 0) {
		CtoSRequestNewPort(p_arg0);
		return;
	}
	if (g_pMessOKConnect->Set(p_arg1) != 0) {
		StoCokConnect(p_arg0);
		return;
	}
	if (g_pMessGOConnect->Set(p_arg1) != 0) {
		CtoSgoConnect(p_arg0);
		return;
	}
	if (g_pMessFAILEDConnect->Set(p_arg1) != 0) {
		StoCfailedConnect(p_arg0);
	}
}

// 68K 0x1020f632 SetNCBuffers__12CBaseNetworkFUlUli
// FUNCTION: LEMBALL 0x00462550
void BaseNetwork::SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
{
	m_nonCriticalPacketCount = p_arg0;
	m_nonCriticalSequenceWindow = p_arg1;
	m_nonCriticalSubpacketCount = p_arg2;
}

// 68K 0x1020f676 SetCBuffers__12CBaseNetworkFii
// FUNCTION: LEMBALL 0x00462570
void BaseNetwork::SetCBuffers(int p_arg0, int p_arg1)
{
	m_criticalPacketCount = p_arg0;
	m_criticalSubpacketCount = p_arg1;
}

// 68K 0x1020f6b0 AttachMessageQueue__12CBaseNetworkFP17CBaseQueueHandler
// STUB: LEMBALL 0x00462590
void BaseNetwork::AttachMessageQueue(BaseQueueHandler* p_arg0)
{
}

// 68K 0x1020f710 DetachMessageQueue__12CBaseNetworkFv
// STUB: LEMBALL 0x004625b0
void BaseNetwork::DetachMessageQueue()
{
}

// 68K 0x1020f768 Process__12CBaseNetworkFv
// STUB: LEMBALL 0x004625e0
void BaseNetwork::Process()
{
}

// 68K 0x1020f866 SendAll__12CBaseNetworkFR15CNetworkMessage
// FUNCTION: LEMBALL 0x00462720
bool BaseNetwork::SendAll(NetworkMessage& p_arg0)
{
	Connect* peer;
	int activeCount;
	int sendBlocked;

	activeCount = 0;
	peer = m_firstConnect;
	sendBlocked = 0;

	while (1) {
		if (peer == 0) {
			if (sendBlocked == 0 && 0 < activeCount) {
				return 1;
			}
			return 0;
		}

		if (peer->m_killRequested == 0) {
			++activeCount;
			if (sendBlocked == 0) {
				if (peer->Send(p_arg0) != 0) {
					sendBlocked = 0;
					peer = peer->m_nextConnect;
					continue;
				}
			}
			sendBlocked = 1;
		}

		peer = peer->m_nextConnect;
	}
}

// 68K 0x1020f8f0 ProcessMsg__12CBaseNetworkFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004627b0
int BaseNetwork::ProcessMsg(Message* p_arg0)
{
	unsigned int type;
	NetworkMessage* stream;
	Connect* peer;

	type = 0;
	type = p_arg0->type;
	if (type == 0xb) {
		if (p_arg0->code == 1) {
			stream = (NetworkMessage*) p_arg0->payload;
			peer = (Connect*) p_arg0->source;
			peer->Send(*stream);
			stream->CloseDataStream();
			return 1;
		}
	}
	else if (type == 0xc) {
		if (p_arg0->code == 1) {
			stream = (NetworkMessage*) p_arg0->payload;
			SendAll(*stream);
			stream->CloseDataStream();
			return 1;
		}
	}
	return 0;
}

// 68K 0x1010f03e BeforeDestroyConnections__12CBaseNetworkFv
// STUB: LEMBALL 0x00462aa0
void BaseNetwork::BeforeDestroyConnections()
{
}

// 68K 0x1010f074 AfterDestroyConnections__12CBaseNetworkFv
// STUB: LEMBALL 0x00462ab0
void BaseNetwork::AfterDestroyConnections()
{
}

// 68K 0x10107a8e WaitProcess__12CBaseNetworkFv
// STUB: LEMBALL 0x00462ac0
void BaseNetwork::WaitProcess()
{
}

// GLOBAL: LEMBALL 0x004a1e18
BaseNetwork* g_pBaseNetwork = 0;
