#include "Multiplayer/CPBNetworkGame.h"

#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Messages/GameMessageIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x00452fe0
CPBNetworkGame::CPBNetworkGame(CAI* p_ai)
	: CNetworkMessage(NETWORK_MESSAGE_GAMEPLAY_STATE), m_ai(p_ai), m_networkLemmings(p_ai->m_networkLemmings)
{
	enum {
		GAMEPLAY_STREAM_FIXED_PAYLOAD_BYTES = 8
	};
	m_payloadCapacity += p_ai->m_payloadCapacity + GAMEPLAY_STREAM_FIXED_PAYLOAD_BYTES;
	m_headerEnabled = 0;
}

// FUNCTION: LEMBALL 0x00453030
void CPBNetworkGame::AddData()
{
	CNetworkMessage::Add((unsigned short) MESSAGE_SIMULATION_TIME);
	CNetworkMessage::Add(g_dwSimulationTimestamp);
	CAI& ai = *m_ai;
	ai.CopyDataStream(m_writeCursor, 0);
	m_writeCursor += ai.m_writeCursor - ai.m_buffer;
	CNetworkMessage::Add((unsigned short) MESSAGE_GAME_STREAM_END);
}

// FUNCTION: LEMBALL 0x00453070
void CPBNetworkGame::GetData()
{
	enum {
		NETWORK_LEMMING_SLOTS_PER_PLAYER = 4
	};
	int marker = CNetworkMessage::GetWORD();
	while (marker != MESSAGE_GAME_STREAM_END) {
		switch (marker) {
		case MESSAGE_PLAYER_LEMMING_STATE: {
			unsigned char playerIndex = CNetworkMessage::GetBYTE();
			CPlayerLemming& player = *m_networkLemmings[playerIndex + NETWORK_LEMMING_SLOTS_PER_PLAYER];
			unsigned char* readCursor = m_readCursor;
			if (player.Set(readCursor)) {
				m_readCursor = player.m_readCursor;
			}
			break;
		}
		case MESSAGE_SIMULATION_TIME:
			SetRemoteGameTimeReal(CNetworkMessage::GetDWORD());
			break;
		}
		marker = CNetworkMessage::GetWORD();
	}
}
