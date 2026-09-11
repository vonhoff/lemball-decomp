#include "FileCommonSocket.h"

#include "../Foundation/VsTime.h"

// 68K 0x10207070 CreateSocket__17CFileCommonSocketFPCc
// FUNCTION: LEMBALL 0x00479880
bool FileCommonSocket::CreateSocket(const char* p_path)
{
	if (!Open(p_path, 1, 0)) {
		if (!Create(p_path, 1)) {
			unsigned long started = CurrentMilliTimer();
			do {
			} while (CurrentMilliTimer() - started < 100);

			if (!Open(p_path, 1, 0)) {
				return false;
			}
		}
		else {
			InitialiseFile();
		}
	}
	return true;
}

// 68K 0x10207138 SysCloseSocket__17CFileCommonSocketFv
// FUNCTION: LEMBALL 0x00479900
int FileCommonSocket::SysCloseSocket()
{
	return Close() ? 0 : 6;
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
