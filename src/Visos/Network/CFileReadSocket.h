#ifndef LEMBALL_VISOS_NETWORK_CFILEREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_CFILEREADSOCKET_H

#include "CFileBaseSocket.h"   // complete type
#include "CFileCommonSocket.h" // complete type
#include "CReadSocket.h"       // complete type

class CNetworkMessage;

// SIZE 0xbc
// VTABLE: LEMBALL 0x0049a56c CFileCommonSocket
// VTABLE: LEMBALL 0x0049a570 CNetworkFile
// VTABLE: LEMBALL 0x0049a5b0 CBaseSocket
// VTABLE: LEMBALL 0x0049a5d8 CBaseCommonSocket
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CFileReadSocket : public CFileBaseSocket, public virtual CReadSocket, public virtual CFileCommonSocket {
public:
	CFileReadSocket();
	bool Read(CNetworkMessage& p_message, int p_remove, int p_wait);
	bool ReadBuff(int p_index);
	virtual void Closed(int p_notifyPeer); // vtable+0x0c
	void Process();

private:
	unsigned int m_pendingReadSlot; // 0x10
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047bd80 SYMBOL
// ?SysCloseSocket@CFileCommonSocket@@WPPPPPPEI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047bda0
// CFileReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bdf0 SYMBOL
// ?SocketError@CFileCommonSocket@@WPPPPPPEI@AEXXZ

// SYNTHETIC: LEMBALL 0x0047be20 SYMBOL
// ??_ECFileReadSocket@@WCM@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a540
// CFileReadSocket::`vbtable'{for `CFileCommonSocket'}

// GLOBAL: LEMBALL 0x0049a550
// CFileReadSocket::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a558
// CFileReadSocket::`vbtable'{for `CFileReadSocket'}

#endif
