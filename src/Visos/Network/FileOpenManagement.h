#ifndef LEMBALL_VISOS_NETWORK_FILEOPENMANAGEMENT_H
#define LEMBALL_VISOS_NETWORK_FILEOPENMANAGEMENT_H

#include "../../Common.h"
#include "OpenCount.h" // complete type

// SIZE 0x34
class FileOpenManagement {
public:
	bool DecOpenCount();
	bool IncOpenCount();
	bool SysCloseSocket();
	~FileOpenManagement();

private:
	OpenCount m_message; // 0x04
};

// SYNTHETIC: LEMBALL 0x0047b9d0
// FileOpenManagement::`scalar deleting destructor'

#endif
