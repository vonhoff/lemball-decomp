#ifndef LEMBALL_VISOS_NETWORK_FILERWSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILERWSOCKET_H

#include "../../Common.h"
#include "FileReadSocket.h"  // complete type
#include "FileWriteSocket.h" // complete type
#include "RwSocket.h"        // complete type

// SIZE 0x154
// VTABLE: LEMBALL 0x0049a79c FileCommonSocket
// VTABLE: LEMBALL 0x0049a7a0 NetworkFile
// VTABLE: LEMBALL 0x0049a7e0 BaseSocket's `ReadSocket
// VTABLE: LEMBALL 0x0049a808 BaseSocket's `WriteSocket
// VTABLE: LEMBALL 0x0049a838 FileRwSocket
class FileRwSocket : public virtual RwSocket, public virtual FileReadSocket, public virtual FileWriteSocket {
public:
	FileRwSocket();
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(unsigned char p_notifyPeer);                  // vtable+0x0c
	virtual NetworkMessage* ReceiveAcknowledgement();                 // vtable+0x1c
	virtual void SendAcknowledgement();                               // vtable+0x18
	virtual ~FileRwSocket();                                          // vtable+0x14
};

// SYNTHETIC: LEMBALL 0x0047ba40 SYMBOL
// ?SocketError@FileCommonSocket@@WPPPPPOMI@AEXXZ

// SYNTHETIC: LEMBALL 0x0047baf0 SYMBOL
// ?SetDestAddr@FileWriteSocket@@WPPPPPOLA@AEXPAVNetworkAddress@@@Z

// SYNTHETIC: LEMBALL 0x0047be30 SYMBOL
// ?SysCloseSocket@FileCommonSocket@@WPPPPPOMI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047be50
// FileRwSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bf00 SYMBOL
// ??_EFileRwSocket@@$4PPPPPPPM@KI@AEPAXI@Z

#endif
