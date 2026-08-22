#ifndef LEMBALL_SCAFFOLD_VISOS_NETWORK_FILEBASESOCKET_H
#define LEMBALL_SCAFFOLD_VISOS_NETWORK_FILEBASESOCKET_H

#include "../../Common.h"

// SIZE 0x04
class FileBaseSocket {
public:
	FileBaseSocket();

private:
	NetworkFile* m_file; // 0x00
};

#endif
