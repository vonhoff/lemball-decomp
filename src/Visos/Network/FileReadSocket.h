#ifndef LEMBALL_VISOS_NETWORK_FILEREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEREADSOCKET_H

#include "../../Common.h"

// SIZE 0xb8
// VTABLE: LEMBALL 0x0049a56c native callback
// Subobject VTABLE: 0x0049a570 NetworkFile view
// VTABLE: LEMBALL 0x0049a5b0 ReadSocket view
// VTABLE: LEMBALL 0x0049a5d8 BaseCommonSocket view
class FileReadSocket {
public:
	FileReadSocket();
	bool Read(NetworkMessage& p_message, unsigned char p_remove, unsigned char p_wait);
	bool ReadBuff(int p_index);
	virtual void Closed(unsigned char p_notifyPeer); // vtable+0x0c
	virtual ~FileReadSocket();                       // vtable+0x14
	void Process();

private:
	undefined m_storage[0xb8]; // 0x00
};

// SYNTHETIC: LEMBALL 0x0047bda0
// FileReadSocket::`scalar deleting destructor'

#endif
