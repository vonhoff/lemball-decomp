#include "ReadSocket.h"

#include "../Messaging/AckMessage.h"
#include "../Messaging/BasePacketHeader.h"
#include "../Messaging/ReadCBuff.h"
#include "../Messaging/ReadCmsBuff.h"
#include "../Messaging/ReadMsBuff.h"
#include "../Messaging/ReadNcBuff.h"
#include "../Messaging/ReadNcmsBuff.h"
#include "../Messaging/ReadPacket.h"
#include "NetworkAddress.h"
#include "TcpIpNetwork.h"

#include <new.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x1020b4d4 __ct__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045f820
ReadSocket::ReadSocket()
{
	m_nonCriticalBuffer = 0;
	m_nonCriticalMultiBuffer = 0;
	m_criticalBuffer = 0;
	m_criticalMultiBuffer = 0;
	m_packetHeader = 0;
	if (g_pBroadcastReceiveAddress == 0) {
		g_pBroadcastReceiveAddress = (NetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
	}
}

// 68K 0x1020b56e __dt__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045f8a0
ReadSocket::~ReadSocket()
{
	DeleteNcBuffers();
	DeleteCBuffers();
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
// FUNCTION: LEMBALL 0x0045f900
void ReadSocket::DeleteCBuffers()
{
	ReadCBuff* critical;
	ReadCmsBuff* criticalMulti;

	critical = m_criticalBuffer;
	if (critical != 0) {
		critical->BasePacketBuff::~BasePacketBuff();
		operator delete(critical);
	}
	criticalMulti = m_criticalMultiBuffer;
	if (criticalMulti != 0) {
		delete criticalMulti;
	}
}

// 68K 0x1020b704 SetNCBuffers__11CReadSocketFUlUli
// FUNCTION: LEMBALL 0x0045f930
void ReadSocket::SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
{
	void* storage;

	DeleteNcBuffers();
	storage = operator new(sizeof(ReadNcBuff));
	if (storage == 0) {
		m_nonCriticalBuffer = 0;
	}
	else {
		m_nonCriticalBuffer = new (storage) ReadNcBuff(p_arg0, (unsigned short) g_networkPacketSize);
	}
	storage = operator new(sizeof(ReadNcmsBuff));
	if (storage != 0) {
		m_nonCriticalMultiBuffer =
			new (storage) ReadNcmsBuff(p_arg0 + 1, p_arg1, p_arg2, (unsigned short) g_networkPacketSize);
		return;
	}
	m_nonCriticalMultiBuffer = 0;
}

// 68K 0x1020b7a8 SetCBuffers__11CReadSocketFii
// FUNCTION: LEMBALL 0x0045f9b0
void ReadSocket::SetCBuffers(int p_arg0, int p_arg1)
{
	void* storage;

	DeleteCBuffers();
	storage = operator new(sizeof(ReadCBuff));
	if (storage == 0) {
		m_criticalBuffer = 0;
	}
	else {
		m_criticalBuffer = new (storage) ReadCBuff(p_arg0, (unsigned short) g_networkPacketSize);
	}
	storage = operator new(sizeof(ReadCmsBuff));
	if (storage == 0) {
		m_criticalMultiBuffer = 0;
	}
	else {
		m_criticalMultiBuffer = new (storage) ReadCmsBuff(p_arg0, p_arg1, (unsigned short) g_networkPacketSize);
	}
	if (g_pNetworkPacketScratch == 0) {
		g_pNetworkPacketScratch = (BasePacketHeader*) operator new(g_networkPacketSize);
	}
}

// 68K 0x1020b85c ProcessPacket__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045fa40
bool ReadSocket::ProcessPacket()
{
	ReadPacket* packet;
	ReadMsBuff* message;
	BasePacketHeader* packetHeader;
	unsigned int messageId;
	unsigned short subpacketSequence;

	m_packetHeader = g_pNetworkPacketScratch;
	Set((unsigned char*) g_pNetworkPacketScratch);
	packetHeader = m_packetHeader;
	if (packetHeader->m_magic != 0x56533039) {
		return false;
	}

	messageId = packetHeader->m_messageId;
	switch (messageId) {
	case 1:
		return false;
	case 2:
		ReceiveAcknowledgement();
		return false;
	}

	subpacketSequence = packetHeader->m_subpacketSequence;
	if (subpacketSequence != 0x100) {
		if (packetHeader->m_critical == 0) {
			message = m_nonCriticalMultiBuffer->UpdateSubPacket();
			if (message != 0) {
				PostRead(9, message);
				return true;
			}
		}
		else {
			message = m_criticalMultiBuffer->StoreSubPacket();
			if (message != 0) {
				PostRead(8, message);
				return true;
			}
		}
	}
	else if (packetHeader->m_critical == 0) {
		packet = m_nonCriticalBuffer->UpdatePacket();
		if (packet != 0) {
			PostRead(7, packet);
			return true;
		}
	}
	else {
		unsigned short packetSequence = packetHeader->m_packetSequence;

		if (m_criticalBuffer->m_nextSequence >= (int) (unsigned int) packetSequence) {
			AckMessage* acknowledgement = g_pAckMessage;

			acknowledgement->m_packetSequence = packetSequence;
			acknowledgement->m_subpacketSequence = subpacketSequence;
			SendAcknowledgement();
			return false;
		}

		packet = m_criticalBuffer->StorePacket();
		if (packet == 0) {
			if (m_criticalBuffer->m_outOfOrder != 0) {
				Closed(1);
			}
		}
		else {
			do {
				unsigned short acknowledgementSubpacketSequence;
				AckMessage* acknowledgement;

				PostRead(5, packet);
				packetHeader = m_packetHeader;
				acknowledgement = g_pAckMessage;
				acknowledgementSubpacketSequence = packetHeader->m_subpacketSequence;
				acknowledgement->m_packetSequence = packetHeader->m_packetSequence;
				acknowledgement->m_subpacketSequence = acknowledgementSubpacketSequence;
				SendAcknowledgement();
				packet = m_criticalBuffer->NextPacketReady();
			} while (packet != 0);

			return true;
		}
	}

	return false;
}

// 68K 0x1020ba78 Process__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045fbb0
void ReadSocket::Process()
{
	if (m_readReady != 0) {
		if (m_closePending != 0 && timeGetTime() - m_lastReceiveTime > 10000) {
			CloseSocket();
			Closed(1);
		}
	}
	else if (m_eventPending != 0 && timeGetTime() - m_lastReceiveTime > 10000) {
		CloseSocket();
		Closed(0);
	}
}

// 68K 0x1020bb26 IsChanged__11CReadSocketFR15CNetworkMessage
// FUNCTION: LEMBALL 0x0045fc30
bool ReadSocket::IsChanged(NetworkMessage& p_arg0)
{
	return m_nonCriticalBuffer->GetPacket(p_arg0.m_messageId)->m_used;
}

// 68K 0x1020bb80 GetLatest__11CReadSocketFR15CNetworkMessage
// FUNCTION: LEMBALL 0x0045fc50
void ReadSocket::GetLatest(NetworkMessage& p_arg0)
{
	ReadPacket* packet = m_nonCriticalBuffer->GetPacket(p_arg0.m_messageId);

	packet->EnterCritical();
	p_arg0.Set(packet->m_data + sizeof(BasePacketHeader));
	packet->LeaveCritical();
	packet->m_used = 0;
}

// 68K 0x1020bc08 UnUseAllNC__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045fcc0
void ReadSocket::UnUseAllNc()
{
	m_nonCriticalBuffer->UnUseAll();
}

// 68K 0x1020bc3c UnUseAllC__11CReadSocketFv
// FUNCTION: LEMBALL 0x0045fcd0
void ReadSocket::UnUseAllC()
{
	m_criticalBuffer->UnUseAll();
}

// 68K 0x10107764 FirstReceive__11CReadSocketFv
// FUNCTION: LEMBALL 0x00462920
void ReadSocket::FirstReceive()
{
}

// 68K 0x1010778c PostRead__11CReadSocketF13NetworkEventsP11CBasePacket
// FUNCTION: LEMBALL 0x00462930
void ReadSocket::PostRead(NetworkEvents p_arg0, BasePacket* p_arg1)
{
}
