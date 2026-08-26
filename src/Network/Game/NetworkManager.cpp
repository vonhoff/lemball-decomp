#include "NetworkManager.h"

// 68K 0x10a00346 __ct__15CNetworkManagerFPCc
// STUB: LEMBALL 0x00452550
NetworkManager::NetworkManager(const char* p_arg0)
{
}

// 68K 0x10a005e8 Start__15CNetworkManagerFv
// STUB: LEMBALL 0x00452740
bool NetworkManager::Start()
{
	return 0;
}

// 68K 0x10a0065e StartBroadcast__15CNetworkManagerFPCc
// STUB: LEMBALL 0x00452780
void NetworkManager::StartBroadcast(const char* p_address)
{
}

// 68K 0x10a006d8 Stop__15CNetworkManagerFv
// STUB: LEMBALL 0x004527c0
void NetworkManager::Stop()
{
}

// 68K 0x10a007a6 ProcessMsg__15CNetworkManagerFP10tagMESSAGE
// STUB: LEMBALL 0x00452850
int NetworkManager::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10a00996 Broadcast__15CNetworkManagerFPCc
// STUB: LEMBALL 0x00452a40
void NetworkManager::Broadcast(const char* p_address)
{
}

// 68K 0x10a00a42 Kill__15CNetworkManagerFv
// STUB: LEMBALL 0x00452ab0
void NetworkManager::Kill()
{
}

// 68K 0x10a00a70 GameProcess__15CNetworkManagerFv
// STUB: LEMBALL 0x00452ac0
void NetworkManager::GameProcess()
{
}

// 68K 0x10a00b6a Process__15CNetworkManagerFv
// STUB: LEMBALL 0x00452b80
void NetworkManager::Process()
{
}

// 68K 0x10a00b92 GetGameMessage__15CNetworkManagerFP8CConnect
// STUB: LEMBALL 0x00452b90
NetworkGameMessage* NetworkManager::GetGameMessage(Connect* p_connection)
{
	return 0;
}

// 68K 0x10a00bfa GetnGame__15CNetworkManagerFP8CConnect
// STUB: LEMBALL 0x00452bf0
int NetworkManager::GetnGame(Connect* p_connection)
{
	return 0;
}

// 68K 0x10a00500 __dt__15CNetworkManagerFv
// SYNTHETIC: LEMBALL 0x00452f20
// NetworkManager::`scalar deleting destructor'
NetworkManager::~NetworkManager()
{
}

// GLOBAL: LEMBALL 0x004a0120
NetworkManager* g_pNetworkManager = 0;
