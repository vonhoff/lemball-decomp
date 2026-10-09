#include "Multiplayer/Transport/FileTransport/CFileReadSocket.h"

#include "Engine/Time/VsTime.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/CNetworkAddress.h"
#include "Multiplayer/Transport/CReadSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileBaseSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileCommonSocket.h"
#include "Multiplayer/Transport/FileTransport/CNetworkFile.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Multiplayer/Transport/Protocol/CHeaderMessage.h"
#include "Multiplayer/Transport/Protocol/CHeaders.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"
#include "Platform/Windows/Network/FileTransport/CFileNetwork.h"

// FUNCTION: LEMBALL 0x00479930
CFileReadSocket::CFileReadSocket() : CFileBaseSocket(), CReadSocket(), CFileCommonSocket()
{
	m_pendingReadSlot = FILE_READ_SOCKET_NO_PENDING_SLOT;
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
		} while (CurrentMilliTimer() - started < NETWORK_FILE_LOCK_RETRY_TIMEOUT_MS);
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
	if (!Seek(g_networkPacketSize * p_index + m_dataOffset)) {
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
		if (!CNetworkFile::Lock(m_headersOffset, m_file->m_payloadCapacity)) {
			static_cast<CFileNetwork*>(g_pBaseNetwork)->ResetTimer(0x32);
			return;
		}
		if (m_pendingReadSlot != FILE_READ_SOCKET_NO_PENDING_SLOT) {
			Seek(m_file->m_headers->m_payloadCapacity * m_pendingReadSlot + m_headersOffset);
			CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, m_file->m_payloadCapacity);
			if (CNetworkFile::UnLock(m_headersOffset, m_file->m_payloadCapacity)) {
				m_file->m_headers[m_pendingReadSlot].Set((unsigned char*) g_pNetworkPacketScratch);
				ReadBuff(m_pendingReadSlot);
				CHeaderMessage* header = &m_file->m_headers[m_pendingReadSlot];
				header->m_sequenceState.m_mirroredSequence = header->m_sequenceState.m_sequence;
				unsigned short sequence;
				int index;
				for (index = m_pendingReadSlot; index < m_headerSlotCount; index++) {
					HeaderSequenceState& state = m_file->m_headers[index].m_sequenceState;
					sequence = state.m_sequence;
					if (state.m_mirroredSequence < sequence) {
						m_pendingReadSlot = index;
						break;
					}
				}
				if (index == m_headerSlotCount) {
					m_pendingReadSlot = FILE_READ_SOCKET_NO_PENDING_SLOT;
				}
			}
		}
		else {
			Seek(m_headersOffset);
			CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, m_file->m_payloadCapacity);
			if (CNetworkFile::UnLock(m_headersOffset, m_file->m_payloadCapacity)) {
				m_file->Set((unsigned char*) g_pNetworkPacketScratch);
				int index = 0;
				bool found = false;
				for (; index < m_headerSlotCount; index++) {
					CHeaderMessage* header = &m_file->m_headers[index];
					if (header->m_sequenceState.m_mirroredSequence < header->m_sequenceState.m_sequence) {
						if (found) {
							m_pendingReadSlot = index;
							return;
						}
						ReadBuff(index);
						header = &m_file->m_headers[index];
						header->m_sequenceState.m_mirroredSequence = header->m_sequenceState.m_sequence;
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
