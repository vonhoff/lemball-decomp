#ifndef LEMBALL_VISOS_NETWORK_FILERWSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILERWSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h" // complete type
#include "FileCommonSocket.h" // complete type
#include "ReadSocket.h"       // complete type
#include "WriteSocket.h"      // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x0049a79c native callback
// VTABLE: LEMBALL 0x0049a7a0 NetworkFile view
// VTABLE: LEMBALL 0x0049a7e0 ReadSocket view
// VTABLE: LEMBALL 0x0049a808 WriteSocket view
// VTABLE: LEMBALL 0x0049a838 BaseCommonSocket view
class FileRwSocket : public virtual BaseCommonSocket,
					 public virtual WriteSocket,
					 public virtual ReadSocket,
					 public FileCommonSocket {
public:
	FileRwSocket();
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(unsigned char p_notifyPeer);                  // vtable+0x0c
	virtual void ReceiveAcknowledgement();                            // vtable+0x1c
	virtual void SendAcknowledgement();                               // vtable+0x18
	virtual ~FileRwSocket();                                          // vtable+0x14

private:
	undefined m_storage[0x150]; // 0x00
};

// SYNTHETIC: LEMBALL 0x0047be50
// FileRwSocket::`scalar deleting destructor'


#endif

