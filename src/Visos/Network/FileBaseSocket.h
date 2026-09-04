#ifndef LEMBALL_VISOS_NETWORK_FILEBASESOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEBASESOCKET_H

#include "../../Common.h"

// SIZE 0x0c
class FileBaseSocket {
public:
	FileBaseSocket();

private:
	NetworkFile* m_file;  // 0x00
	undefined4 m_unk0x04; // 0x04
	undefined4 m_unk0x08; // 0x08
};

#endif
