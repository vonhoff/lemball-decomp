#ifndef LEMBALL_VISOS_NETWORK_FILEREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEREADSOCKET_H

#include "../../Common.h"
#include "FileBaseSocket.h"   // complete type
#include "FileCommonSocket.h" // complete type
#include "ReadSocket.h"       // complete type

// SIZE 0xbc
// VTABLE: LEMBALL 0x0049a56c FileCommonSocket
// Original NetworkFile subobject table: LEMBALL 0x0049a570. It is byte-identical
// to NetworkFile's own table, whose single VTABLE annotation owns that address.
// VTABLE: LEMBALL 0x0049a5b0 BaseSocket
// VTABLE: LEMBALL 0x0049a5d8 BaseCommonSocket
class FileReadSocket : public FileBaseSocket, public virtual ReadSocket, public virtual FileCommonSocket {
public:
	FileReadSocket();
	bool Read(NetworkMessage& p_message, unsigned char p_remove, unsigned char p_wait);
	bool ReadBuff(int p_index);
	virtual void Closed(unsigned char p_notifyPeer); // vtable+0x0c
	virtual ~FileReadSocket();                       // vtable+0x14
	void Process();

private:
	undefined4 m_unk0x10; // 0x10
};

// SYNTHETIC: LEMBALL 0x0047bd80 SYMBOL
// ?SysCloseSocket@FileCommonSocket@@WPPPPPPEI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047bda0
// FileReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bdf0 SYMBOL
// ?SocketError@FileCommonSocket@@WPPPPPPEI@AEXXZ

// SYNTHETIC: LEMBALL 0x0047be20 SYMBOL
// ??_EFileReadSocket@@WCM@AEPAXI@Z

#endif
