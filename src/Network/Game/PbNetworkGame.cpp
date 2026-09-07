#include "PbNetworkGame.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Control/Game/GameTime.h"

// 68K 0x10a00c52 __ct__14CPBNetworkGameFP3CAI
// STUB: LEMBALL 0x00452fe0
PbNetworkGame::PbNetworkGame(Ai* p_arg0)
{
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
// STUB: LEMBALL 0x00453070
void PbNetworkGame::GetData()
{
}

// 68K 0x10118234 __dt__14CPBNetworkGameFv
PbNetworkGame::~PbNetworkGame()
{
}
