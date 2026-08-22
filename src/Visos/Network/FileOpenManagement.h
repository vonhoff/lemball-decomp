#ifndef LEMBALL_SCAFFOLD_VISOS_NETWORK_FILEOPENMANAGEMENT_H
#define LEMBALL_SCAFFOLD_VISOS_NETWORK_FILEOPENMANAGEMENT_H

#include "../../Common.h"
#include "../Messaging/NetworkMessage.h" // complete type

// SIZE 0x34
class FileOpenManagement {
public:
	bool DecOpenCount();
	bool IncOpenCount();
	bool SysCloseSocket();
	~FileOpenManagement();

private:
	NetworkMessage m_message;   // 0x04
	unsigned short m_openCount; // 0x30
	undefined2 m_reserved32;    // 0x32
};

#endif
