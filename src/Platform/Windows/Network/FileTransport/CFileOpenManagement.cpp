#include "Platform/Windows/Network/FileTransport/CFileOpenManagement.h"

#include "Platform/Windows/Network/FileTransport/CFileCommonSocket.h"
#include "Platform/Windows/Network/FileTransport/CFileReadSocket.h"
#include "Platform/Windows/Network/FileTransport/CFileWriteSocket.h"
#include "Platform/Windows/Network/FileTransport/COpenCount.h"

// FUNCTION: LEMBALL 0x0047a470
bool CFileOpenManagement::IncOpenCount()
{
	Seek(0);
	if (!CFileReadSocket::Read(m_message, 1, 0)) {
		return false;
	}
	m_message.m_openCount++;
	Seek(0);
	return CFileWriteSocket::Write(m_message, 0, 1);
}

// FUNCTION: LEMBALL 0x0047a4d0
bool CFileOpenManagement::DecOpenCount()
{
	Seek(0);
	if (!CFileReadSocket::Read(m_message, 1, 0)) {
		return false;
	}
	m_message.m_openCount--;
	Seek(0);
	return CFileWriteSocket::Write(m_message, 0, 1);
}

// FUNCTION: LEMBALL 0x0047a530
int CFileOpenManagement::SysCloseSocket()
{
	DecOpenCount();
	int result = CFileCommonSocket::SysCloseSocket();
	if (m_message.m_openCount == 0) {
		Delete();
	}
	return result;
}
