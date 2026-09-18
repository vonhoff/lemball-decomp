#ifndef LEMBALL_VISOS_NETWORK_FILEREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEREADSOCKET_H

#include "FileBaseSocket.h"   // complete type
#include "FileCommonSocket.h" // complete type
#include "ReadSocket.h"       // complete type

class NetworkMessage;

// SIZE 0xbc
// VTABLE: LEMBALL 0x0049a56c FileCommonSocket
// VTABLE: LEMBALL 0x0049a570 NetworkFile
// VTABLE: LEMBALL 0x0049a5b0 BaseSocket
// VTABLE: LEMBALL 0x0049a5d8 BaseCommonSocket
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class FileReadSocket : public FileBaseSocket, public virtual ReadSocket, public virtual FileCommonSocket {
public:
	FileReadSocket();
	bool Read(NetworkMessage& p_message, int p_remove, int p_wait);
	bool ReadBuff(int p_index);
	virtual void Closed(int p_notifyPeer); // vtable+0x0c
	void Process();

private:
	unsigned int m_unk0x10; // 0x10
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047bd80 SYMBOL
// ?SysCloseSocket@FileCommonSocket@@WPPPPPPEI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047bda0
// FileReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bdf0 SYMBOL
// ?SocketError@FileCommonSocket@@WPPPPPPEI@AEXXZ

// SYNTHETIC: LEMBALL 0x0047be20 SYMBOL
// ??_EFileReadSocket@@WCM@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a540
// FileReadSocket::`vbtable'{for `FileCommonSocket'}

// GLOBAL: LEMBALL 0x0049a550
// FileReadSocket::`vbtable'{for `ReadSocket'}

// GLOBAL: LEMBALL 0x0049a558
// FileReadSocket::`vbtable'{for `FileReadSocket'}

#endif
