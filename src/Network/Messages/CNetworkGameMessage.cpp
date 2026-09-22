#include "CNetworkGameMessage.h"

// FUNCTION: LEMBALL 0x004523e0
CNetworkGameMessage::CNetworkGameMessage() : CNetworkMessage(5)
{
	m_gameName[0] = '\0';
	m_valid = 0;
	m_payloadCapacity += 0x1e;
	m_headerEnabled = 1;
}

#include "../../Visos/Foundation/CString.h"
#include "Visos/Messaging/CNetworkMessage.h"

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
