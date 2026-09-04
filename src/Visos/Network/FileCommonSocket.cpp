#include "FileCommonSocket.h"

// 68K 0x10207070 CreateSocket__17CFileCommonSocketFPCc
// STUB: LEMBALL 0x00479880
bool FileCommonSocket::CreateSocket(const char* p_path)
{
	return 0;
}

// 68K 0x10207138 SysCloseSocket__17CFileCommonSocketFv
// STUB: LEMBALL 0x00479900
int FileCommonSocket::SysCloseSocket()
{
	return 0;
}

// 68K 0x101078e2 SocketError__17CFileCommonSocketFv
// FUNCTION: LEMBALL 0x0047b8f0
void FileCommonSocket::SocketError()
{
	BaseCommonSocket::SocketError((NetworkErrors) 0);
}

// 68K 0x101065e2 __dt__17CFileCommonSocketFv
FileCommonSocket::~FileCommonSocket()
{
}
