#ifndef LEMBALL_VISOS_NETWORK_CFILEWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_CFILEWRITESOCKET_H

#include "CFileBaseSocket.h"   // complete type
#include "CFileCommonSocket.h" // complete type
#include "CWriteSocket.h"      // complete type

class CNetworkMessage;

// SIZE 0xdc
// VTABLE: LEMBALL 0x0049a694 CFileCommonSocket
// VTABLE: LEMBALL 0x0049a698 CNetworkFile
// VTABLE: LEMBALL 0x0049a6d8 CBaseSocket
// VTABLE: LEMBALL 0x0049a708 CBaseCommonSocket
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CFileWriteSocket : public CFileBaseSocket, public virtual CWriteSocket, public virtual CFileCommonSocket {
public:
	CFileWriteSocket();
	int Write(CNetworkMessage& p_message, int p_keepLock, int p_wait);
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(int p_notifyPeer);                            // vtable+0x0c
	virtual void SetDestAddr(CNetworkAddress* p_address);             // vtable+0x20
	virtual ~CFileWriteSocket();                                      // vtable+0x14
	friend class CFileRWSocket;

private:
	int m_nextWriteSlot; // 0x10: current file packet slot
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047bca0 SYMBOL
// ?SysCloseSocket@CFileCommonSocket@@WPPPPPPCI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047bcc0
// CFileWriteSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bd10 SYMBOL
// ?SocketError@CFileCommonSocket@@WPPPPPPCI@AEXXZ

// SYNTHETIC: LEMBALL 0x0047bd60 SYMBOL
// ?SetDestAddr@CFileWriteSocket@@$4PPPPPPPM@A@AEXPAVCNetworkAddress@@@Z

// GLOBAL: LEMBALL 0x0049a668
// CFileWriteSocket::`vbtable'{for `CFileCommonSocket'}

// GLOBAL: LEMBALL 0x0049a678
// CFileWriteSocket::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a680
// CFileWriteSocket::`vbtable'{for `CFileWriteSocket'}

#endif
