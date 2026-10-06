#include "CReadSocket.h"

#include "Multiplayer/Transport/Packets/BasePacketHeader.h"
#include "Multiplayer/Transport/Protocol/CAckMessage.h"
#include "Multiplayer/Transport/Packets/CReadCBuff.h"
#include "Multiplayer/Transport/Packets/CReadCMSBuff.h"
#include "Multiplayer/Transport/Packets/CReadMSBuff.h"
#include "Multiplayer/Transport/Packets/CReadNCBuff.h"
#include "Multiplayer/Transport/Packets/CReadNCMSBuff.h"
#include "Multiplayer/Transport/Packets/CReadPacket.h"
#include "CNetworkAddress.h"
#include "CBaseNetwork.h"
#include "Engine/Time/VsTime.h"
#include "NetworkConstants.h"
#include "Multiplayer/Transport/Packets/CBasePacketBuff.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"
#include "CBaseCommonSocket.h"

#include <new.h>
#include <stddef.h>

// FUNCTION: LEMBALL 0x0045f820
CReadSocket::CReadSocket()
{
	m_nonCriticalBuffer = NULL;
	m_nonCriticalMultiBuffer = NULL;
	m_criticalBuffer = NULL;
	m_criticalMultiBuffer = NULL;
	m_packetHeader = NULL;
	if (g_pBroadcastReceiveAddress == NULL) {
		g_pBroadcastReceiveAddress = g_pBaseNetwork->GetNewNetworkAddress();
	}
}

// FUNCTION: LEMBALL 0x0045f8a0
CReadSocket::~CReadSocket()
{
	DeleteNCBuffers();
	DeleteCBuffers();
}

// FUNCTION: LEMBALL 0x0045f8d0
void CReadSocket::DeleteNCBuffers()
{
	CReadNCBuff* nc;
	CReadNCMSBuff* ncms;

	nc = m_nonCriticalBuffer;
	if (nc != NULL) {
		nc->CBasePacketBuff::~CBasePacketBuff();
		operator delete(nc);
	}
	ncms = m_nonCriticalMultiBuffer;
	if (ncms != NULL) {
		delete ncms;
	}
}

// FUNCTION: LEMBALL 0x0045f900
void CReadSocket::DeleteCBuffers()
{
	CReadCBuff* critical;
	CReadCMSBuff* criticalMulti;

	critical = m_criticalBuffer;
	if (critical != NULL) {
		critical->CBasePacketBuff::~CBasePacketBuff();
		operator delete(critical);
	}
	criticalMulti = m_criticalMultiBuffer;
	if (criticalMulti != NULL) {
		delete criticalMulti;
	}
}

// FUNCTION: LEMBALL 0x0045f930
void CReadSocket::SetNCBuffers(unsigned long p_lastSinglePacketMessageId,
							   unsigned long p_lastMessageId,
							   int p_messageCapacity)
{
	void* storage;

	DeleteNCBuffers();
	storage = operator new(sizeof(CReadNCBuff));
	if (storage == NULL) {
		m_nonCriticalBuffer = NULL;
	}
	else {
		m_nonCriticalBuffer =
			new (storage) CReadNCBuff(p_lastSinglePacketMessageId, (unsigned short) g_networkPacketSize);
	}
	storage = operator new(sizeof(CReadNCMSBuff));
	if (storage != NULL) {
		m_nonCriticalMultiBuffer = new (storage) CReadNCMSBuff(p_lastSinglePacketMessageId + 1,
															   p_lastMessageId,
															   p_messageCapacity,
															   (unsigned short) g_networkPacketSize);
		return;
	}
	m_nonCriticalMultiBuffer = NULL;
}

// FUNCTION: LEMBALL 0x0045f9b0
void CReadSocket::SetCBuffers(int p_packetCount, int p_messageCapacity)
{
	void* storage;

	DeleteCBuffers();
	storage = operator new(sizeof(CReadCBuff));
	if (storage == NULL) {
		m_criticalBuffer = NULL;
	}
	else {
		m_criticalBuffer = new (storage) CReadCBuff(p_packetCount, (unsigned short) g_networkPacketSize);
	}
	storage = operator new(sizeof(CReadCMSBuff));
	if (storage == NULL) {
		m_criticalMultiBuffer = NULL;
	}
	else {
		m_criticalMultiBuffer =
			new (storage) CReadCMSBuff(p_packetCount, p_messageCapacity, (unsigned short) g_networkPacketSize);
	}
	if (g_pNetworkPacketScratch == NULL) {
		g_pNetworkPacketScratch = (BasePacketHeader*) operator new(g_networkPacketSize);
	}
}

// FUNCTION: LEMBALL 0x0045fa40
bool CReadSocket::ProcessPacket()
{
	CReadPacket* packet;
	CReadMSBuff* message;
	BasePacketHeader* packetHeader;
	unsigned int messageId;
	unsigned short subpacketSequence;

	m_packetHeader = g_pNetworkPacketScratch;
	Set((unsigned char*) g_pNetworkPacketScratch);
	packetHeader = m_packetHeader;
	if (packetHeader->m_magic != BASE_PACKET_MAGIC) {
		return false;
	}

	messageId = packetHeader->m_messageId;
	switch (messageId) {
	case NETWORK_MESSAGE_PULSE:
		return false;
	case NETWORK_MESSAGE_ACKNOWLEDGEMENT:
		ReceiveAcknowledgement();
		return false;
	}

	subpacketSequence = packetHeader->m_subpacketSequence;
	if (subpacketSequence != BASE_PACKET_UNSEGMENTED) {
		if (packetHeader->m_critical == 0) {
			message = m_nonCriticalMultiBuffer->UpdateSubPacket();
			if (message != NULL) {
				PostRead(NETWORK_EVENT_NONCRITICAL_MESSAGE_REASSEMBLED, message);
				return true;
			}
		}
		else {
			message = m_criticalMultiBuffer->StoreSubPacket();
			if (message != NULL) {
				PostRead(NETWORK_EVENT_CRITICAL_MESSAGE_REASSEMBLED, message);
				return true;
			}
		}
	}
	else if (packetHeader->m_critical == 0) {
		packet = m_nonCriticalBuffer->UpdatePacket();
		if (packet != NULL) {
			PostRead(NETWORK_EVENT_NONCRITICAL_PACKET_READY, packet);
			return true;
		}
	}
	else {
		unsigned short packetSequence = packetHeader->m_packetSequence;

		if (m_criticalBuffer->m_nextSequence >= (int) (unsigned int) packetSequence) {
			CAckMessage* acknowledgement = g_pAckMessage;

			acknowledgement->m_packetSequence = packetSequence;
			acknowledgement->m_subpacketSequence = subpacketSequence;
			SendAcknowledgement();
			return false;
		}

		packet = m_criticalBuffer->StorePacket();
		if (packet == NULL) {
			if (m_criticalBuffer->m_outOfOrder != 0) {
				Closed(1);
			}
		}
		else {
			do {
				unsigned short acknowledgementSubpacketSequence;
				CAckMessage* acknowledgement;

				PostRead(NETWORK_EVENT_CRITICAL_PACKET_READY, packet);
				packetHeader = m_packetHeader;
				acknowledgement = g_pAckMessage;
				acknowledgementSubpacketSequence = packetHeader->m_subpacketSequence;
				acknowledgement->m_packetSequence = packetHeader->m_packetSequence;
				acknowledgement->m_subpacketSequence = acknowledgementSubpacketSequence;
				SendAcknowledgement();
				packet = m_criticalBuffer->NextPacketReady();
			} while (packet != NULL);

			return true;
		}
	}

	return false;
}

// FUNCTION: LEMBALL 0x0045fbb0
void CReadSocket::Process()
{
	if (m_readReady != 0) {
		if (m_closePending != 0 && CurrentMilliTimer() - m_lastReceiveTime > NETWORK_RECEIVE_IDLE_TIMEOUT_MS) {
			CloseSocket();
			Closed(1);
		}
	}
	else if (m_eventPending != 0 && CurrentMilliTimer() - m_lastReceiveTime > NETWORK_RECEIVE_IDLE_TIMEOUT_MS) {
		CloseSocket();
		Closed(0);
	}
}

// FUNCTION: LEMBALL 0x0045fc30
bool CReadSocket::IsChanged(CNetworkMessage& p_message)
{
	return m_nonCriticalBuffer->GetPacket(p_message.m_messageId)->m_used;
}

// FUNCTION: LEMBALL 0x0045fc50
void CReadSocket::GetLatest(CNetworkMessage& p_message)
{
	CReadPacket* packet = m_nonCriticalBuffer->GetPacket(p_message.m_messageId);
	packet->EnterCritical();
	p_message.Set(packet->m_data + sizeof(BasePacketHeader));
	packet->LeaveCritical();
	packet->m_used = 0;
}

// FUNCTION: LEMBALL 0x0045fcc0
void CReadSocket::UnUseAllNC()
{
	m_nonCriticalBuffer->UnUseAll();
}

// FUNCTION: LEMBALL 0x0045fcd0
void CReadSocket::UnUseAllC()
{
	m_criticalBuffer->UnUseAll();
}

// FUNCTION: LEMBALL 0x00462920
void CReadSocket::FirstReceive()
{
}

// FUNCTION: LEMBALL 0x00462930
void CReadSocket::PostRead(NetworkEvents p_event, CBasePacket* p_packet)
{
}
