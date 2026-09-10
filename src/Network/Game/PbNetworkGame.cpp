#include "PbNetworkGame.h"

#include "../../AI/Navigation/Ai.h"
#include "../../AI/Objects/PlayerLemming.h"
#include "../../Control/Game/GameTime.h"

// 68K 0x10a00c52 __ct__14CPBNetworkGameFP3CAI
// FUNCTION: LEMBALL 0x00452fe0
PbNetworkGame::PbNetworkGame(Ai* p_arg0) : NetworkMessage(3), m_ai(p_arg0), m_networkLemmings(p_arg0->m_networkLemmings)
{
	m_payloadCapacity += p_arg0->m_payloadCapacity + 8;
	m_headerEnabled = 0;
}

// 68K 0x10a00cd2 AddData__14CPBNetworkGameFv
// FUNCTION: LEMBALL 0x00453030
void PbNetworkGame::AddData()
{
	NetworkMessage::Add((unsigned short) 0x2d);
	NetworkMessage::Add(g_dwSimulationTimestamp);
	Ai* ai = m_ai;
	ai->CopyDataStream(m_writeCursor, 0);
	m_writeCursor += ai->m_writeCursor - ai->m_buffer;
	NetworkMessage::Add((unsigned short) 0x2f);
}

// 68K 0x10a00d52 GetData__14CPBNetworkGameFv
// FUNCTION: LEMBALL 0x00453070
void PbNetworkGame::GetData()
{
	int marker = NetworkMessage::GetWord();
	while (marker != 0x2f) {
		switch (marker) {
		case 0x2c: {
			unsigned char playerIndex = NetworkMessage::GetByte();
			PlayerLemming* player = m_networkLemmings[playerIndex + 4];
			NetworkMessage* message = (NetworkMessage*) ((unsigned char*) player + sizeof(GlobalGameObject));
			unsigned char* readCursor = m_readCursor;
			if (message->Set(readCursor)) {
				m_readCursor = message->m_readCursor;
			}
			break;
		}
		case 0x2d:
			SetRemoteGameTimeReal(NetworkMessage::GetDword());
			break;
		}
		marker = NetworkMessage::GetWord();
	}
}

// 68K 0x10118234 __dt__14CPBNetworkGameFv
PbNetworkGame::~PbNetworkGame()
{
}
