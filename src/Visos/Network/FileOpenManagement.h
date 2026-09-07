#ifndef LEMBALL_VISOS_NETWORK_FILEOPENMANAGEMENT_H
#define LEMBALL_VISOS_NETWORK_FILEOPENMANAGEMENT_H

#include "../../Common.h"
#include "FileRwSocket.h" // complete type
#include "OpenCount.h"    // complete type

// SIZE 0x34
class FileOpenManagement : public virtual FileRwSocket {
public:
	bool DecOpenCount();
	bool IncOpenCount();
	int SysCloseSocket();
	~FileOpenManagement();
	friend class FileConnect;

private:
	friend class FileBroadcast;
	OpenCount m_message; // 0x04
};

// SYNTHETIC: LEMBALL 0x0047b9d0
// FileOpenManagement::`scalar deleting destructor'

#endif
