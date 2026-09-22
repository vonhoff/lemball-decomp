#ifndef LEMBALL_VISOS_NETWORK_CFILECONNECT_H
#define LEMBALL_VISOS_NETWORK_CFILECONNECT_H

#include "CConnect.h"            // complete type
#include "CFileOpenManagement.h" // complete type

// SIZE 0x1b8
// VTABLE: LEMBALL 0x0049ab68 CFileCommonSocket
// VTABLE: LEMBALL 0x0049ab70 CNetworkFile
// VTABLE: LEMBALL 0x0049abb0 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049abd8 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049ac08 CFileConnect
// VTABLE: LEMBALL 0x0049ac18 CConnect
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CFileConnect : public CFileOpenManagement, public CConnect {
public:
	CFileConnect();
	virtual bool Start(const char* p_localName, const char* p_remoteName); // vtable+0x00
	virtual void Connect();                                                // vtable+0x10
	virtual void InitSocket();                                             // vtable+0x08
	virtual void Process();                                                // vtable+0x04
	virtual CNetworkMessage* ReceiveAcknowledgement();                     // vtable+0x1c
	void Closed(int p_notifyPeer);
	void ConnectSetup();
	void InitialiseFile();
	void Listen(CNetworkAddress* p_address);
	~CFileConnect();
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047bf40
// CFileConnect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047c000 SYMBOL
// ??_ECFileConnect@@$4PPPPPPPM@KI@AEPAXI@Z

// LIBRARY: LEMBALL 0x0047fe00 SYMBOL
// _strchr

// LIBRARY: LEMBALL 0x00480010 SYMBOL
// _strncmp

#endif
