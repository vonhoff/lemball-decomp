#include "CPBNetworkGame.h"

#include "../../AI/Navigation/CAI.h"
#include "../../Control/Game/GameTime.h"
#include "AI/Objects/CPlayerLemming.h"
#include "Visos/Messaging/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x00452fe0
CPBNetworkGame::CPBNetworkGame(CAI* p_ai) : CNetworkMessage(3), m_ai(p_ai), m_networkLemmings(p_ai->m_networkLemmings)
{
	m_payloadCapacity += p_ai->m_payloadCapacity + 8;
	m_headerEnabled = 0;
}

// FUNCTION: LEMBALL 0x00453030
void CPBNetworkGame::AddData()
{
	CNetworkMessage::Add((unsigned short) 0x2d);
	CNetworkMessage::Add(g_dwSimulationTimestamp);
	CAI& ai = *m_ai;
	ai.CopyDataStream(m_writeCursor, 0);
	m_writeCursor += ai.m_writeCursor - ai.m_buffer;
	CNetworkMessage::Add((unsigned short) 0x2f);
}

// FUNCTION: LEMBALL 0x00453070
void CPBNetworkGame::GetData()
{
	int marker = CNetworkMessage::GetWORD();
	while (marker != 0x2f) {
		switch (marker) {
		case 0x2c: {
			unsigned char playerIndex = CNetworkMessage::GetBYTE();
			CPlayerLemming& player = *m_networkLemmings[playerIndex + 4];
			unsigned char* readCursor = m_readCursor;
			if (player.Set(readCursor)) {
				m_readCursor = player.m_readCursor;
			}
			break;
		}
		case 0x2d:
			SetRemoteGameTimeReal(CNetworkMessage::GetDWORD());
			break;
		}
		marker = CNetworkMessage::GetWORD();
	}
}
