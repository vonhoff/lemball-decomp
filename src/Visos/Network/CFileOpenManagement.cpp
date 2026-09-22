#include "CFileOpenManagement.h"

#include "Visos/Network/CFileCommonSocket.h"
#include "Visos/Network/CFileReadSocket.h"
#include "Visos/Network/CFileWriteSocket.h"
#include "Visos/Network/COpenCount.h"

// FUNCTION: LEMBALL 0x0047a470
bool CFileOpenManagement::IncOpenCount()
{
	Seek(0);
	if (!CFileReadSocket::Read(m_message, 1, 0)) {
		return 0;
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
		return 0;
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
