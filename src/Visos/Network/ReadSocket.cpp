#include "ReadSocket.h"

#include "../Messaging/ReadNcBuff.h"
#include "../Messaging/ReadNcmsBuff.h"

// 68K 0x1020b4d4 __ct__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045f820
ReadSocket::ReadSocket()
{
	m_nonCriticalBuffer = 0;
	m_nonCriticalMultiBuffer = 0;
	m_criticalBuffer = 0;
	m_criticalMultiBuffer = 0;
	m_packetHeader = 0;
}

// 68K 0x1020b56e __dt__11CReadSocketFv
// STUB: LEMBALL 0x0045f8a0
ReadSocket::~ReadSocket()
{
}

// 68K 0x1020b5fe DeleteNCBuffers__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045f8d0
void ReadSocket::DeleteNcBuffers()
{
	ReadNcBuff* nc;
	ReadNcmsBuff* ncms;

	nc = m_nonCriticalBuffer;
	if (nc != 0) {
		nc->BasePacketBuff::~BasePacketBuff();
		operator delete(nc);
	}
	ncms = m_nonCriticalMultiBuffer;
	if (ncms != 0) {
		delete ncms;
	}
}

// 68K 0x1020b682 DeleteCBuffers__11CReadSocketFv
// STUB: LEMBALL 0x0045f900
void ReadSocket::DeleteCBuffers()
{
}

// 68K 0x1020b704 SetNCBuffers__11CReadSocketFUlUli
// STUB: LEMBALL 0x0045f930
void ReadSocket::SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
{
}

// 68K 0x1020b7a8 SetCBuffers__11CReadSocketFii
// STUB: LEMBALL 0x0045f9b0
void ReadSocket::SetCBuffers(int p_arg0, int p_arg1)
{
}

// 68K 0x1020b85c ProcessPacket__11CReadSocketFv
// STUB: LEMBALL 0x0045fa40
bool ReadSocket::ProcessPacket()
{
	return 0;
}

// 68K 0x1020ba78 Process__11CReadSocketFv
// STUB: LEMBALL 0x0045fbb0
void ReadSocket::Process()
{
}

// 68K 0x1020bb26 IsChanged__11CReadSocketFR15CNetworkMessage
// STUB: LEMBALL 0x0045fc30
bool ReadSocket::IsChanged(NetworkMessage& p_arg0)
{
	return 0;
}

// 68K 0x1020bb80 GetLatest__11CReadSocketFR15CNetworkMessage
// STUB: LEMBALL 0x0045fc50
void ReadSocket::GetLatest(NetworkMessage& p_arg0)
{
}

// 68K 0x1020bc08 UnUseAllNC__11CReadSocketFv
// STUB: LEMBALL 0x0045fcc0
void ReadSocket::UnUseAllNc()
{
}

// 68K 0x1020bc3c UnUseAllC__11CReadSocketFv
// STUB: LEMBALL 0x0045fcd0
void ReadSocket::UnUseAllC()
{
}

// 68K 0x10107764 FirstReceive__11CReadSocketFv
// STUB: LEMBALL 0x00462920
void ReadSocket::FirstReceive()
{
}

// 68K 0x1010778c PostRead__11CReadSocketF13NetworkEventsP11CBasePacket
// STUB: LEMBALL 0x00462930
void ReadSocket::PostRead(NetworkEvents p_arg0, BasePacket* p_arg1)
{
}
