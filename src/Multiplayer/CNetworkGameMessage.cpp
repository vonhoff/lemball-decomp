#include "Multiplayer/CNetworkGameMessage.h"

#include "Multiplayer/CGameFlaggedMessage.h"

// FUNCTION: LEMBALL 0x004523e0
CNetworkGameMessage::CNetworkGameMessage() : CNetworkMessage(GAME_MESSAGE_GAME_INFO)
{
	m_gameName[0] = '\0';
	m_valid = 0;
	m_payloadCapacity += sizeof(m_gameName) + sizeof(m_peerName);
	m_headerEnabled = 1;
}

#include "Engine/Strings/CString.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"

#include <string.h>

extern char* g_szBroadcastPeerName;
extern char* g_szGameName;

// FUNCTION: LEMBALL 0x00452420
void CNetworkGameMessage::AddData()
{
	char peerName[21];
	Add(g_szGameName);
	char* broadcastPeerName = g_szBroadcastPeerName;
	strncpy(peerName, broadcastPeerName, 20);
	peerName[20] = '\0';
	CString peerString(peerName);
	peerString.lower();
	Add(peerString);
}

// FUNCTION: LEMBALL 0x00452490
void CNetworkGameMessage::GetData()
{
	GetCopy(m_gameName);
	GetCopy(m_peerName);
	m_valid = 1;
}
