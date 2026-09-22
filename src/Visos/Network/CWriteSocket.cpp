#include "CWriteSocket.h"

#include "../Messaging/CAckMessage.h"
#include "../Messaging/CPulseMessage.h"
#include "../Messaging/CWriteCBuff.h"
#include "../Messaging/CWritePacket.h"
#include "../Messaging/WriteNcBuff.h"
#include "CBaseNetwork.h"
#include "CNetworkAddress.h"
#include "CTcpIpNetwork.h"
#include "Visos/Messaging/BasePacketHeader.h"
#include "Visos/Messaging/CBasePacketBuff.h"
#include "Visos/Messaging/CNetworkMessage.h"

#include <new.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x0045fce0
CWriteSocket::CWriteSocket()
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
	m_destinationAddress = (CNetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
}

// FUNCTION: LEMBALL 0x0045fd80
CWriteSocket::~CWriteSocket()
{
	operator delete(m_destinationAddress);
	m_destinationAddress = 0;
	operator delete(m_packetHeader);
	DeleteNcBuffers();
	DeleteCBuffers();
}

// FUNCTION: LEMBALL 0x0045fde0
void CWriteSocket::InternalSetDestAddr(CNetworkAddress* p_address)
{
	*m_destinationAddress = *p_address;
}

// FUNCTION: LEMBALL 0x0045fdf0
void CWriteSocket::DeleteNcBuffers()
{
	WriteNcBuff* buffer;

	buffer = m_nonCriticalBuffer;
	if (buffer != 0) {
		buffer->CBasePacketBuff::~CBasePacketBuff();
		operator delete(buffer);
	}
}

// FUNCTION: LEMBALL 0x0045fe10
void CWriteSocket::DeleteCBuffers()
{
	CWriteCBuff* critical;
	CWriteCBuff* unknown;

	critical = m_criticalBuffer;
	if (critical != 0) {
		critical->CBasePacketBuff::~CBasePacketBuff();
		operator delete(critical);
	}
	unknown = m_unknown50;
	if (unknown != 0) {
		unknown->CBasePacketBuff::~CBasePacketBuff();
		operator delete(unknown);
	}
}

// FUNCTION: LEMBALL 0x0045fe50
void CWriteSocket::SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
{
	(void) p_arg0;
	(void) p_arg1;
	(void) p_arg2;
	DeleteNcBuffers();
}

// FUNCTION: LEMBALL 0x0045fe60
void CWriteSocket::SetCBuffers(int p_arg0, int p_arg1)
{
	void* storage;

	(void) p_arg1;
	DeleteCBuffers();
	storage = operator new(sizeof(CWriteCBuff));
	if (storage != 0) {
		m_criticalBuffer = new (storage) CWriteCBuff(p_arg0, (unsigned short) g_networkPacketSize);
		return;
	}
	m_criticalBuffer = 0;
}

// FUNCTION: LEMBALL 0x0045fea0
bool CWriteSocket::SendCritical(CNetworkMessage& p_arg0)
{
	unsigned char* data;
	int size;
	CWritePacket* packet;

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

// FUNCTION: LEMBALL 0x0045ff20
bool CWriteSocket::ResendCritical(CWritePacket* p_packet)
{
	int size;
	unsigned char* data;
	bool sent;

	data = p_packet->m_data;
	Set(data);
	size = ((BasePacketHeader*) data)->m_packetSize;
	Set(data);
	sent = SendPacket(data, size);
	if (sent != 0) {
		p_packet->m_lastSendTime = timeGetTime();
		p_packet->m_retryCount++;
		p_packet->m_available = 0;
	}
	return sent;
}

// FUNCTION: LEMBALL 0x0045ff70
bool CWriteSocket::SendNcms(CNetworkMessage& p_arg0)
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

// FUNCTION: LEMBALL 0x004600d0
bool CWriteSocket::Send(CNetworkMessage& p_arg0)
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

// FUNCTION: LEMBALL 0x004601b0
void CWriteSocket::SendAcknowledgement()
{
	g_pAckMessage->OpenDataStream();
	Send(*g_pAckMessage);
	g_pAckMessage->CloseDataStream();
}

// FUNCTION: LEMBALL 0x004601e0
CNetworkMessage* CWriteSocket::ReceiveAcknowledgement()
{
	CNetworkMessage* message;

	message = 0;
	g_pAckMessage->Set((unsigned char*) (g_pNetworkPacketScratch + 1));
	if (g_pAckMessage->m_subpacketSequence == 0x100) {
		CWriteCBuff* buffer;
		CWritePacket* packet;

		buffer = m_criticalBuffer;
		packet = (CWritePacket*) buffer->m_packets[g_pAckMessage->m_packetSequence % buffer->m_packetCount];
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

// FUNCTION: LEMBALL 0x00460260
void CWriteSocket::Closed(int p_notifyPeer)
{
	(void) p_notifyPeer;
	if (m_segmentIndex != -1) {
		m_segmentIndex = -1;
		m_segmentedMessage->m_pendingSendCount = 0;
	}
}

// FUNCTION: LEMBALL 0x00460280
void CWriteSocket::Process()
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
		CWriteCBuff* buffer;
		CWritePacket* packet;

		buffer = m_criticalBuffer;
		if (index >= buffer->m_packetCount) {
			break;
		}
		packet = (CWritePacket*) buffer->m_packets[index % buffer->m_packetCount];
		if (packet->m_available == 0 && 1000 < timeGetTime() - packet->m_lastSendTime) {
			if (g_pBaseNetwork->m_criticalRetryLimit == 0 ||
				packet->m_retryCount != g_pBaseNetwork->m_criticalRetryLimit) {
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
