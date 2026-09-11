#include "FileOpenManagement.h"

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
	int result = DecOpenCount();
	if (m_message.m_openCount == 0) {
		FileCommonSocket::SysCloseSocket();
	}
	return result;
}

// 68K 0x10106a58 __dt__19CFileOpenManagementFv
FileOpenManagement::~FileOpenManagement()
{
}
