#include "WriteSocket.h"

#include "../Messaging/WriteCBuff.h"
#include "../Messaging/WriteNcBuff.h"
#include "TcpIpNetwork.h"

#include <new.h>

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);

// 68K 0x1020bc70 __ct__12CWriteSocketFv
// FUNCTION: LEMBALL 0x0045fce0
WriteSocket::WriteSocket()
{
	BasePacketHeader* header;

	m_lastSendTime = timeGetTime() - 1000;
	header = (BasePacketHeader*) operator new(sizeof(BasePacketHeader));
	m_packetHeader = header;
	header->m_magic = 0x56533039;
	m_nonCriticalBuffer = 0;
	m_criticalBuffer = 0;
	m_unknown50 = 0;
	m_segmentedMessage = 0;
	m_segmentIndex = -1;
}

// 68K 0x1020bd2c __dt__12CWriteSocketFv
// FUNCTION: LEMBALL 0x0045fd80
WriteSocket::~WriteSocket()
{
	operator delete(m_destinationAddress);
	m_destinationAddress = 0;
	operator delete(m_packetHeader);
	DeleteNcBuffers();
	DeleteCBuffers();
}

// 68K 0x1020bdd8 _SetDestAddr__12CWriteSocketFP15CNetworkAddress
// STUB: LEMBALL 0x0045fde0
void WriteSocket::SetDestAddr(NetworkAddress* p_arg0)
{
}

// 68K 0x1020be2a DeleteNCBuffers__12CWriteSocketFv
// FUNCTION: LEMBALL 0x0045fdf0
void WriteSocket::DeleteNcBuffers()
{
	WriteNcBuff* buffer;

	buffer = m_nonCriticalBuffer;
	if (buffer != 0) {
		buffer->BasePacketBuff::~BasePacketBuff();
		operator delete(buffer);
	}
}

// 68K 0x1020be88 DeleteCBuffers__12CWriteSocketFv
// FUNCTION: LEMBALL 0x0045fe10
void WriteSocket::DeleteCBuffers()
{
	if (m_criticalBuffer != 0) {
		m_criticalBuffer->~WriteCBuff();
		operator delete(m_criticalBuffer);
		m_criticalBuffer = 0;
	}
}

// 68K 0x1020bf10 SetNCBuffers__12CWriteSocketFUlUli
// FUNCTION: LEMBALL 0x0045fe50
void WriteSocket::SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
{
	(void) p_arg0;
	(void) p_arg1;
	(void) p_arg2;
	DeleteNcBuffers();
}

// 68K 0x1020bf46 SetCBuffers__12CWriteSocketFii
// FUNCTION: LEMBALL 0x0045fe60
void WriteSocket::SetCBuffers(int p_arg0, int p_arg1)
{
	void* storage;

	(void) p_arg1;
	DeleteCBuffers();
	storage = operator new(sizeof(WriteCBuff));
	if (storage != 0) {
		m_criticalBuffer = new (storage) WriteCBuff(p_arg0, (unsigned short) g_networkPacketSize);
		return;
	}
	m_criticalBuffer = 0;
}

// 68K 0x1020bfac SendCritical__12CWriteSocketFR15CNetworkMessage
// STUB: LEMBALL 0x0045fea0
bool WriteSocket::SendCritical(NetworkMessage& p_arg0)
{
	return 0;
}

// 68K 0x1020c082 ResendCritical__12CWriteSocketFP12CWritePacket
// STUB: LEMBALL 0x0045ff20
char WriteSocket::ResendCritical(WritePacket* p_arg0)
{
	return 0;
}

// 68K 0x1020c112 SendNCMS__12CWriteSocketFR15CNetworkMessage
// STUB: LEMBALL 0x0045ff70
bool WriteSocket::SendNcms(NetworkMessage& p_arg0)
{
	return 0;
}

// 68K 0x1020c292 Send__12CWriteSocketFR15CNetworkMessage
// STUB: LEMBALL 0x004600d0
bool WriteSocket::Send(NetworkMessage& p_arg0)
{
	return 0;
}

// 68K 0x1020c3b0 SendAcknowledgement__12CWriteSocketFv
// STUB: LEMBALL 0x004601b0
void WriteSocket::SendAcknowledgement()
{
}

// 68K 0x1020c404 ReceiveAcknowledgement__12CWriteSocketFv
// STUB: LEMBALL 0x004601e0
NetworkMessage* WriteSocket::ReceiveAcknowledgement()
{
	return 0;
}

// 68K 0x1020c4cc Closed__12CWriteSocketFUc
// STUB: LEMBALL 0x00460260
void WriteSocket::Closed(unsigned char p_arg0)
{
}

// 68K 0x1020c50a Process__12CWriteSocketFv
// STUB: LEMBALL 0x00460280
void WriteSocket::Process()
{
}

// 68K 0x101074e2 SetPort__12CWriteSocketFs
// FUNCTION: LEMBALL 0x00462970
void WriteSocket::SetPort(short p_arg0)
{
	m_port = (unsigned short) p_arg0;
}
