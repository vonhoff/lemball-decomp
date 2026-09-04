#ifndef LEMBALL_VISOS_NETWORK_FILEWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEWRITESOCKET_H

#include "../../Common.h"
#include "FileBaseSocket.h"   // complete type
#include "FileCommonSocket.h" // complete type
#include "WriteSocket.h"      // complete type

// SIZE 0xdc
// VTABLE: LEMBALL 0x0049a694 FileCommonSocket
// VTABLE: LEMBALL 0x0049a698 NetworkFile
// VTABLE: LEMBALL 0x0049a6d8 BaseSocket
// VTABLE: LEMBALL 0x0049a708 BaseCommonSocket
class FileWriteSocket : public FileBaseSocket, public virtual WriteSocket, public virtual FileCommonSocket {
public:
	FileWriteSocket();
	int Write(NetworkMessage& p_message, unsigned char p_keepLock, unsigned char p_wait);
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(unsigned char p_notifyPeer);                  // vtable+0x0c
	virtual void SetDestAddr(NetworkAddress* p_address);              // vtable+0x20
	virtual ~FileWriteSocket();                                       // vtable+0x14

private:
	undefined4 m_unk0x10; // 0x10
};

// SYNTHETIC: LEMBALL 0x0047bca0 SYMBOL
// ?SysCloseSocket@FileCommonSocket@@WPPPPPPCI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047bcc0
// FileWriteSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bd10 SYMBOL
// ?SocketError@FileCommonSocket@@WPPPPPPCI@AEXXZ

#endif
