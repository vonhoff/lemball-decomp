#ifndef LEMBALL_VISOS_NETWORK_FILEWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEWRITESOCKET_H

#include "../../Common.h"

// SIZE 0xd8
// VTABLE: LEMBALL 0x0049a694 native callback
// VTABLE: LEMBALL 0x0049a698 NetworkFile view
// VTABLE: LEMBALL 0x0049a6d8 WriteSocket view
// VTABLE: LEMBALL 0x0049a708 BaseCommonSocket view
class FileWriteSocket {
public:
	FileWriteSocket();
	int Write(NetworkMessage& p_message, unsigned char p_keepLock, unsigned char p_wait);
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(unsigned char p_notifyPeer);                  // vtable+0x0c
	virtual void SetDestAddr(NetworkAddress* p_address);              // vtable+0x20
	virtual ~FileWriteSocket();                                       // vtable+0x14

private:
	undefined m_storage[0xd8]; // 0x00
};

#endif
