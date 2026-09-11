#include "FileReadSocket.h"

#include "../Foundation/VsTime.h"
#include "../Messaging/HeaderMessage.h"
#include "../Messaging/Headers.h"
#include "FileNetwork.h"
#include "NetworkAddress.h"
#include "TcpIpNetwork.h"

// 68K 0x102071b6 __ct__15CFileReadSocketFv
// FUNCTION: LEMBALL 0x00479930
FileReadSocket::FileReadSocket() : FileBaseSocket(), ReadSocket(), FileCommonSocket()
{
	m_unk0x10 = 0xffffffffUL;
}

// 68K 0x10207326 Read__15CFileReadSocketFR15CNetworkMessageUcUc
// FUNCTION: LEMBALL 0x00479a40
bool FileReadSocket::Read(NetworkMessage& p_message, unsigned char p_remove, unsigned char p_wait)
{
	int offset = NetworkFile::Tell();
	int length = p_message.m_payloadCapacity;
	if (p_wait == 0) {
		unsigned long started = CurrentMilliTimer();
		while (!NetworkFile::Lock(offset, length)) {
			if (100 <= CurrentMilliTimer() - started) {
				return false;
			}
		}
	}
	if (NetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, length)) {
		p_message.Set((unsigned char*) g_pNetworkPacketScratch);
		if (p_remove == 0 && !NetworkFile::UnLock(offset, length)) {
			return false;
		}
		return true;
	}
	NetworkFile::UnLock(offset, length);
	return false;
}

// 68K 0x1020742e ReadBuff__15CFileReadSocketFi
// FUNCTION: LEMBALL 0x00479b30
bool FileReadSocket::ReadBuff(int p_index)
{
	if (!NetworkFile::Seek(g_networkPacketSize * p_index + FileBaseSocket::m_dataOffset)) {
		return false;
	}
	HeaderMessage* header = &m_file->m_headers[p_index];
	if (!NetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, header->m_headerValue)) {
		FileCommonSocket::SocketError();
		return false;
	}
	m_lastReceiveTime = CurrentMilliTimer();
	g_receivedPacketSize = header->m_headerValue;
	*g_pBroadcastReceiveAddress = header->m_text0;
	if (FileCommonSocket::m_eventPending == 0) {
		ReadSocket::FirstReceive();
	}
	ReadSocket::ProcessPacket();
	return true;
}

// 68K 0x1020754c Process__15CFileReadSocketFv
// FUNCTION: LEMBALL 0x00479c10
void FileReadSocket::Process()
{
	if ((FileCommonSocket::m_eventPending != 0 || FileCommonSocket::m_platformState != 0) &&
		FileCommonSocket::m_closePending != 0) {
		if (!NetworkFile::Lock(m_unk0x04, m_file->m_payloadCapacity)) {
			FileNetwork* network = 0;
			if (g_pBaseNetwork != 0) {
				network = static_cast<FileNetwork*>(g_pBaseNetwork);
			}
			network->ResetTimer(0x32);
			return;
		}
		if (m_unk0x10 == -1) {
			NetworkFile::Seek(m_unk0x04);
			NetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, m_file->m_payloadCapacity);
			if (NetworkFile::UnLock(m_unk0x04, m_file->m_payloadCapacity)) {
				m_file->Set((unsigned char*) g_pNetworkPacketScratch);
				if (FileCommonSocket::m_unk0x08 > 0) {
					bool found = false;
					int index = 0;
					do {
						HeaderMessage* header = &m_file->m_headers[index];
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
						++index;
					} while (index < FileCommonSocket::m_unk0x08);
				}
			}
		}
		else {
			int index = m_unk0x10;
			NetworkFile::Seek(m_file->m_headers->m_payloadCapacity * index + m_unk0x04);
			NetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, m_file->m_payloadCapacity);
			if (NetworkFile::UnLock(m_unk0x04, m_file->m_payloadCapacity)) {
				HeaderMessage* header = &m_file->m_headers[index];
				header->Set((unsigned char*) g_pNetworkPacketScratch);
				ReadBuff(index);
				header = &m_file->m_headers[index];
				header->m_mirroredSequence = header->m_sequence;
				if (index < FileCommonSocket::m_unk0x08) {
					for (;; ++index) {
						header = &m_file->m_headers[index];
						if (header->m_mirroredSequence < header->m_sequence) {
							m_unk0x10 = index;
							break;
						}
						if (index + 1 >= FileCommonSocket::m_unk0x08) {
							++index;
							break;
						}
					}
				}
				if (index == FileCommonSocket::m_unk0x08) {
					m_unk0x10 = 0xffffffffUL;
				}
			}
		}
	}
}

// 68K 0x1010669c __dt__15CFileReadSocketFv
FileReadSocket::~FileReadSocket()
{
}

// 68K 0x101078ba Closed__15CFileReadSocketFUc
// FUNCTION: LEMBALL 0x0047be10
void FileReadSocket::Closed(int p_notifyPeer)
{
}
