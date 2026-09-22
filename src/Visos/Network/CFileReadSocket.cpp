#include "CFileReadSocket.h"

#include "../Foundation/VsTime.h"
#include "../Messaging/CHeaderMessage.h"
#include "../Messaging/CHeaders.h"
#include "CFileNetwork.h"
#include "CNetworkAddress.h"
#include "CTcpIpNetwork.h"
#include "Visos/Messaging/CNetworkMessage.h"
#include "Visos/Network/CBaseNetwork.h"
#include "Visos/Network/CFileBaseSocket.h"
#include "Visos/Network/CFileCommonSocket.h"
#include "Visos/Network/CNetworkFile.h"
#include "Visos/Network/CReadSocket.h"

// FUNCTION: LEMBALL 0x00479930
CFileReadSocket::CFileReadSocket() : CFileBaseSocket(), CReadSocket(), CFileCommonSocket()
{
	m_unk0x10 = 0xffffffffUL;
}

// FUNCTION: LEMBALL 0x00479a40
bool CFileReadSocket::Read(CNetworkMessage& p_message, int p_remove, int p_wait)
{
	int offset = Tell();
	int length = p_message.m_payloadCapacity;
	if (p_wait == 0) {
		unsigned long started = CurrentMilliTimer();
		int locked;
		do {
			locked = CNetworkFile::Lock(offset, length);
			if (locked) {
				break;
			}
		} while (CurrentMilliTimer() - started < 100);
		if (!locked) {
			return false;
		}
	}
	if (!CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, length)) {
		CNetworkFile::UnLock(offset, length);
		return false;
	}
	p_message.Set((unsigned char*) g_pNetworkPacketScratch);
	if (p_remove == 0 && !CNetworkFile::UnLock(offset, length)) {
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00479b30
bool CFileReadSocket::ReadBuff(int p_index)
{
	if (!Seek(g_networkPacketSize * p_index + CFileBaseSocket::m_dataOffset)) {
		return false;
	}
	CHeaderMessage* header = &m_file->m_headers[p_index];
	if (!CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, header->m_headerValue)) {
		SocketError();
		return false;
	}
	m_lastReceiveTime = CurrentMilliTimer();
	g_receivedPacketSize = m_file->m_headers[p_index].m_headerValue;
	*g_pBroadcastReceiveAddress = header->m_text0;
	if (m_readReady == 0) {
		FirstReceive();
	}
	CReadSocket::ProcessPacket();
	return true;
}

// FUNCTION: LEMBALL 0x00479c10
void CFileReadSocket::Process()
{
	if ((m_readReady != 0 || m_eventPending != 0) && m_isOpen != 0) {
		if (!CNetworkFile::Lock(m_unk0x04, m_file->m_payloadCapacity)) {
			static_cast<CFileNetwork*>(g_pBaseNetwork)->ResetTimer(0x32);
			return;
		}
		if (m_unk0x10 != -1) {
			Seek(m_file->m_headers->m_payloadCapacity * m_unk0x10 + m_unk0x04);
			CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, m_file->m_payloadCapacity);
			if (CNetworkFile::UnLock(m_unk0x04, m_file->m_payloadCapacity)) {
				m_file->m_headers[m_unk0x10].Set((unsigned char*) g_pNetworkPacketScratch);
				ReadBuff(m_unk0x10);
				CHeaderMessage* header = &m_file->m_headers[m_unk0x10];
				header->m_mirroredSequence = header->m_sequence;
				int index;
				for (index = m_unk0x10; index < CFileCommonSocket::m_unk0x08; index++) {
					header = &m_file->m_headers[index];
					if (header->m_sequence > header->m_mirroredSequence) {
						m_unk0x10 = index;
						break;
					}
				}
				if (index == CFileCommonSocket::m_unk0x08) {
					m_unk0x10 = 0xffffffffUL;
				}
			}
		}
		else {
			Seek(m_unk0x04);
			CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, m_file->m_payloadCapacity);
			if (CNetworkFile::UnLock(m_unk0x04, m_file->m_payloadCapacity)) {
				m_file->Set((unsigned char*) g_pNetworkPacketScratch);
				bool found = false;
				for (int index = 0; index < CFileCommonSocket::m_unk0x08; index++) {
					CHeaderMessage* header = &m_file->m_headers[index];
					if (header->m_mirroredSequence < header->m_sequence) {
						if (found) {
							m_unk0x10 = index;
							return;
						}
						ReadBuff(index);
						header = &m_file->m_headers[index];
						header->m_mirroredSequence = header->m_sequence;
						found = true;
					}
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0047be10
void CFileReadSocket::Closed(int p_notifyPeer)
{
}
