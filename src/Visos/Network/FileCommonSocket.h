#ifndef LEMBALL_VISOS_NETWORK_FILECOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILECOMMONSOCKET_H

#include "../../Common.h"

// SIZE 0x4c
class FileCommonSocket {
public:
	bool CreateSocket(const char* p_path);
	int SysCloseSocket();
	void SocketError();
	~FileCommonSocket();

private:
	undefined m_storage[0x4c]; // 0x00
};

#endif
