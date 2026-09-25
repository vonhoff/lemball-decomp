#ifndef LEMBALL_VISOS_NETWORK_CFILECOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_CFILECOMMONSOCKET_H

#include "CBaseCommonSocket.h"
#include "CNetworkFile.h"

// SIZE 0x4c
// VTABLE: LEMBALL 0x0049a4f0 CBaseCommonSocket
// VTABLE: LEMBALL 0x0049a500 CNetworkFile
// VTABLE: LEMBALL 0x0049a53c CFileCommonSocket
#pragma vtordisp(off)
class CFileCommonSocket : public virtual CNetworkFile, public virtual CBaseCommonSocket {
public:
	virtual void InitialiseFile() = 0;
	bool CreateSocket(const char* p_path);
	virtual int SysCloseSocket();
	virtual void SocketError();

protected:
	int m_headerSlotCount; // 0x08
};
#pragma vtordisp(on)

// SYNTHETIC: LEMBALL 0x0047b910
// CFileCommonSocket::`scalar deleting destructor'

#endif
