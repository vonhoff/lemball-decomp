#include "CFileCommonSocket.h"

#include "../Foundation/VsTime.h"
#include "Visos/Network/CBaseCommonSocket.h"

// FUNCTION: LEMBALL 0x00479880
bool CFileCommonSocket::CreateSocket(const char* p_path)
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

// FUNCTION: LEMBALL 0x00479900
int CFileCommonSocket::SysCloseSocket()
{
	return Close() ? 0 : 6;
}

// FUNCTION: LEMBALL 0x0047b8f0
void CFileCommonSocket::SocketError()
{
	CBaseCommonSocket::SocketError((NetworkErrors) 0);
}
