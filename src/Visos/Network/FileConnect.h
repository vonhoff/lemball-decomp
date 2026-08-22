#ifndef LEMBALL_SCAFFOLD_VISOS_NETWORK_FILECONNECT_H
#define LEMBALL_SCAFFOLD_VISOS_NETWORK_FILECONNECT_H

#include "../../Common.h"

// SIZE 0x1b8
// VTABLE: LEMBALL 0x0049ab68 native callback
// VTABLE: LEMBALL 0x0049ab70 NetworkFile view
// VTABLE: LEMBALL 0x0049abb0 ReadSocket view
// VTABLE: LEMBALL 0x0049abd8 WriteSocket view
// VTABLE: LEMBALL 0x0049ac08 BaseCommonSocket view
// VTABLE: LEMBALL 0x0049ac18 primary Connect view
class FileConnect {
public:
	FileConnect();
	virtual bool Start(const char* p_localName, const char* p_remoteName); // vtable+0x00
	virtual void Connect();                                                // vtable+0x10
	virtual void InitSocket();                                             // vtable+0x08
	virtual void Process();                                                // vtable+0x04
	virtual void ReceiveAcknowledgement();                                 // vtable+0x1c
	void Closed(unsigned char p_notifyPeer);
	void ConnectSetup();
	void InitialiseFile();
	void Listen(NetworkAddress* p_address);
	~FileConnect();

private:
	undefined m_storage[0x1b8]; // 0x00
};

#endif
