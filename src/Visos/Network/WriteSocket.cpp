#include "WriteSocket.h"

#include "../Foundation/VsTime.h"
#include "../Messaging/AckMessage.h"
#include "../Messaging/PulseMessage.h"
#include "../Messaging/WriteCBuff.h"
#include "../Messaging/WriteNcBuff.h"
#include "../Messaging/WritePacket.h"
#include "BaseNetwork.h"
#include "NetworkAddress.h"
#include "TcpIpNetwork.h"

#include <new.h>

extern "C" unsigned long __stdcall timeGetTime(void);

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
	m_destinationAddress = (NetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
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
// FUNCTION: LEMBALL 0x0045fde0
void WriteSocket::_SetDestAddr(NetworkAddress* p_arg0)
{
	*m_destinationAddress = *p_arg0;
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
	WriteCBuff* critical;
	WriteCBuff* unknown;

	critical = m_criticalBuffer;
	if (critical != 0) {
		critical->BasePacketBuff::~BasePacketBuff();
		operator delete(critical);
	}
	unknown = m_unknown50;
	if (unknown != 0) {
		unknown->BasePacketBuff::~BasePacketBuff();
		operator delete(unknown);
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
// FUNCTION: LEMBALL 0x0045fea0
bool WriteSocket::SendCritical(NetworkMessage& p_arg0)
{
	unsigned char* data;
	int size;
	WritePacket* packet;

	m_packetHeader->m_critical = 1;
	data = p_arg0.m_buffer;
	size = p_arg0.m_writeCursor - data;
	CopyDataStream(data, 0);
	packet = m_criticalBuffer->StorePacket(m_packetHeader->m_packetSequence, data, size, &p_arg0);
	if (packet == 0) {
		return false;
	}
	if (SendPacket(data, size) != 0) {
		packet->m_lastSendTime = timeGetTime();
		packet->m_available = 0;
		packet->m_retryCount++;
	}
	else {
		packet->m_lastSendTime = timeGetTime() - 1000;
		packet->m_available = 0;
	}
	return true;
}

// 68K 0x1020c082 ResendCritical__12CWriteSocketFP12CWritePacket
// FUNCTION: LEMBALL 0x0045ff20
bool WriteSocket::ResendCritical(WritePacket* p_arg0)
{
	int size;
	unsigned char* data;
	bool sent;

	data = p_arg0->m_data;
	Set(data);
	size = ((BasePacketHeader*) data)->m_packetSize;
	Set(data);
	sent = SendPacket(data, size);
	if (sent != 0) {
		p_arg0->m_lastSendTime = timeGetTime();
		p_arg0->m_retryCount++;
		p_arg0->m_available = 0;
	}
	return sent;
}

// 68K 0x1020c112 SendNCMS__12CWriteSocketFR15CNetworkMessage
// FUNCTION: LEMBALL 0x0045ff70
bool WriteSocket::SendNcms(NetworkMessage& p_arg0)
{
	unsigned char* data;
	int remaining;
	unsigned int dataSize;
	int segmentCount;
	int sendCount;

	if (m_segmentIndex == -1) {
		m_segmentIndex = 0;
		m_segmentSequence = (short) ++m_multiMessageSequence;
		m_segmentedMessage = &p_arg0;
	}
	dataSize = g_networkPacketSize - 0x10;
	data = p_arg0.m_buffer;
	remaining = p_arg0.m_writeCursor - data;
	sendCount = 0;
	segmentCount = (dataSize + remaining - 0x11) / dataSize;
	if (m_segmentIndex != 0) {
		dataSize *= m_segmentIndex;
		data += dataSize;
		remaining -= dataSize;
	}
	m_packetHeader->m_packetSequence = (unsigned short) m_segmentSequence;
	while (segmentCount > m_segmentIndex && sendCount++ < 9) {
		int sendSize;

		m_packetHeader->m_subpacketSequence = (unsigned short) m_segmentIndex;
		sendSize = g_networkPacketSize;
		if (remaining < (int) g_networkPacketSize) {
			sendSize = remaining;
		}
		if (m_segmentIndex == 0) {
			CopyDataStream(data, 0);
			if (SendPacket(data, sendSize) == 0) {
				break;
			}
		}
		else {
			bool sent;

			m_savedHeader.m_magic = ((BasePacketHeader*) data)->m_magic;
			m_savedHeader.m_packetSize = ((BasePacketHeader*) data)->m_packetSize;
			*(unsigned int*) &m_savedHeader.m_messageId = *(unsigned int*) &((BasePacketHeader*) data)->m_messageId;
			*(unsigned int*) &m_savedHeader.m_subpacketSequence =
				*(unsigned int*) &((BasePacketHeader*) data)->m_subpacketSequence;
			CopyDataStream(data, 0);
			sent = SendPacket(data, sendSize);
			*(BasePacketHeader*) data = m_savedHeader;
			if (sent == 0) {
				break;
			}
		}
		remaining -= sendSize;
		data += sendSize - 0x10;
		remaining += 0x10;
		m_segmentIndex++;
	}
	if (m_segmentIndex != segmentCount) {
		return false;
	}
	m_segmentIndex = -1;
	return true;
}

// 68K 0x1020c292 Send__12CWriteSocketFR15CNetworkMessage
// FUNCTION: LEMBALL 0x004600d0
bool WriteSocket::Send(NetworkMessage& p_arg0)
{
	unsigned char* data;
	bool sent;

	if (m_socketFlags == 0) {
		return false;
	}
	if (p_arg0.m_pendingSendCount == 0) {
		p_arg0.m_pendingSendCount = 1;
	}
	m_packetHeader->m_packetSize = p_arg0.m_writeCursor - p_arg0.m_buffer;
	data = p_arg0.m_buffer;
	m_packetHeader->m_messageId = (unsigned short) p_arg0.m_messageId;
	if (g_networkPacketSize < m_packetHeader->m_packetSize) {
		if (m_segmentIndex != -1) {
			p_arg0.m_pendingSendCount = 0;
			sent = false;
		}
		else {
			sent = SendNcms(p_arg0);
		}
	}
	else {
		m_packetHeader->m_subpacketSequence = 0x100;
		if (p_arg0.m_headerEnabled == 0) {
			m_packetHeader->m_packetSequence = ++m_nonCriticalSequence;
			m_packetHeader->m_critical = 0;
			CopyDataStream(data, 0);
			sent = SendPacket(data, m_packetHeader->m_packetSize);
		}
		else {
			m_packetHeader->m_packetSequence = ++m_criticalSequence;
			sent = SendCritical(p_arg0);
		}
	}
	p_arg0.m_pendingSendCount = 0;
	return sent;
}

// 68K 0x1020c3b0 SendAcknowledgement__12CWriteSocketFv
// FUNCTION: LEMBALL 0x004601b0
void WriteSocket::SendAcknowledgement()
{
	g_pAckMessage->OpenDataStream();
	Send(*g_pAckMessage);
	g_pAckMessage->CloseDataStream();
}

// 68K 0x1020c404 ReceiveAcknowledgement__12CWriteSocketFv
// FUNCTION: LEMBALL 0x004601e0
NetworkMessage* WriteSocket::ReceiveAcknowledgement()
{
	NetworkMessage* message;

	message = 0;
	g_pAckMessage->Set((unsigned char*) (g_pNetworkPacketScratch + 1));
	if (g_pAckMessage->m_subpacketSequence == 0x100) {
		WriteCBuff* buffer;
		WritePacket* packet;

		buffer = m_criticalBuffer;
		packet = (WritePacket*) buffer->m_packets[g_pAckMessage->m_packetSequence % buffer->m_packetCount];
		Set(packet->m_data);
		if (((BasePacketHeader*) packet->m_data)->m_packetSequence == g_pAckMessage->m_packetSequence &&
			packet->m_available == 0) {
			packet->m_available = 1;
			message = packet->m_message;
		}
		Set(packet->m_data);
	}
	return message;
}

// 68K 0x1020c4cc Closed__12CWriteSocketFUc
// FUNCTION: LEMBALL 0x00460260
void WriteSocket::Closed(unsigned char p_arg0)
{
	(void) p_arg0;
	if (m_segmentIndex != -1) {
		m_segmentIndex = -1;
		m_segmentedMessage->m_pendingSendCount = 0;
	}
}

// 68K 0x1020c50a Process__12CWriteSocketFv
// FUNCTION: LEMBALL 0x00460280
void WriteSocket::Process()
{
	int index;

	if (m_socketFlags == 0 || m_isOpen == 0) {
		return;
	}
	if (m_segmentIndex != -1 && SendNcms(*m_segmentedMessage) != 0) {
		m_segmentedMessage->m_pendingSendCount = 0;
	}
	index = 0;
	while (1) {
		WriteCBuff* buffer;
		WritePacket* packet;

		buffer = m_criticalBuffer;
		if (index >= buffer->m_packetCount) {
			break;
		}
		packet = (WritePacket*) buffer->m_packets[index % buffer->m_packetCount];
		if (packet->m_available == 0 && 1000 < timeGetTime() - packet->m_lastSendTime) {
			if (g_pBaseNetwork->m_unk0x60 == 0 || packet->m_retryCount != g_pBaseNetwork->m_unk0x60) {
				ResendCritical(packet);
			}
			else {
				packet->m_available = 1;
				index++;
				Closed(1);
				continue;
			}
		}
		index++;
	}
	if (m_closePending != 0 && 1000 < timeGetTime() - m_lastSendTime) {
		Send(*g_pPulseMessage);
	}
}
