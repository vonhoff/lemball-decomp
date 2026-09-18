#include "FileOpenManagement.h"

#include "Visos/Network/FileCommonSocket.h"
#include "Visos/Network/FileReadSocket.h"
#include "Visos/Network/FileWriteSocket.h"
#include "Visos/Network/OpenCount.h"

// 68K 0x10208096 IncOpenCount__19CFileOpenManagementFv
// FUNCTION: LEMBALL 0x0047a470
bool FileOpenManagement::IncOpenCount()
{
	Seek(0);
	if (!FileReadSocket::Read(m_message, 1, 0)) {
		return 0;
	}
	m_message.m_openCount++;
	Seek(0);
	return FileWriteSocket::Write(m_message, 0, 1);
}

// 68K 0x10208148 DecOpenCount__19CFileOpenManagementFv
// FUNCTION: LEMBALL 0x0047a4d0
bool FileOpenManagement::DecOpenCount()
{
	Seek(0);
	if (!FileReadSocket::Read(m_message, 1, 0)) {
		return 0;
	}
	m_message.m_openCount--;
	Seek(0);
	return FileWriteSocket::Write(m_message, 0, 1);
}

// 68K 0x102081fa SysCloseSocket__19CFileOpenManagementFv
// FUNCTION: LEMBALL 0x0047a530
int FileOpenManagement::SysCloseSocket()
{
	DecOpenCount();
	int result = FileCommonSocket::SysCloseSocket();
	if (m_message.m_openCount == 0) {
		Delete();
	}
	return result;
}
