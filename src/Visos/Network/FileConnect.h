#ifndef LEMBALL_VISOS_NETWORK_FILECONNECT_H
#define LEMBALL_VISOS_NETWORK_FILECONNECT_H

#include "../../Common.h"
#include "Connect.h"            // complete type
#include "FileOpenManagement.h" // complete type

// SIZE 0x1b8
// VTABLE: LEMBALL 0x0049ab68 native callback
// VTABLE: LEMBALL 0x0049ab70 NetworkFile view
// VTABLE: LEMBALL 0x0049abb0 ReadSocket view
// VTABLE: LEMBALL 0x0049abd8 WriteSocket view
// VTABLE: LEMBALL 0x0049ac08 BaseCommonSocket view
// VTABLE: LEMBALL 0x0049ac18 primary Connect view
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class FileConnect : public FileOpenManagement, public Connect {
public:
	FileConnect();
	virtual bool Start(const char* p_localName, const char* p_remoteName); // vtable+0x00
	virtual void Connect();                                                // vtable+0x10
	virtual void InitSocket();                                             // vtable+0x08
	virtual void Process();                                                // vtable+0x04
	virtual NetworkMessage* ReceiveAcknowledgement();                      // vtable+0x1c
	void Closed(int p_notifyPeer);
	void ConnectSetup();
	void InitialiseFile();
	void Listen(NetworkAddress* p_address);
	~FileConnect();
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047bf40
// FileConnect::`scalar deleting destructor'

// LIBRARY: LEMBALL 0x0047fe00 SYMBOL
// _strchr

// LIBRARY: LEMBALL 0x00480010 SYMBOL
// _strncmp

#endif
