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
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class FileWriteSocket : public FileBaseSocket, public virtual WriteSocket, public virtual FileCommonSocket {
public:
	FileWriteSocket();
	int Write(NetworkMessage& p_message, undefined4 p_keepLock, undefined4 p_wait);
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(int p_notifyPeer);                            // vtable+0x0c
	virtual void SetDestAddr(NetworkAddress* p_address);              // vtable+0x20
	virtual ~FileWriteSocket();                                       // vtable+0x14
	friend class FileRwSocket;

private:
	int m_unk0x10; // 0x10: current file packet slot
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047bca0 SYMBOL
// ?SysCloseSocket@FileCommonSocket@@WPPPPPPCI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047bcc0
// FileWriteSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bd10 SYMBOL
// ?SocketError@FileCommonSocket@@WPPPPPPCI@AEXXZ

#endif
