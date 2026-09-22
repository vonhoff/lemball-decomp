#ifndef LEMBALL_VISOS_NETWORK_CFILERWSOCKET_H
#define LEMBALL_VISOS_NETWORK_CFILERWSOCKET_H

#include "CFileReadSocket.h"  // complete type
#include "CFileWriteSocket.h" // complete type
#include "CRwSocket.h"        // complete type

// SIZE 0x154
// VTABLE: LEMBALL 0x0049a79c CFileCommonSocket
// VTABLE: LEMBALL 0x0049a7a0 CNetworkFile
// VTABLE: LEMBALL 0x0049a7e0 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a808 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a838 CFileRwSocket
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CFileRwSocket : public virtual CRwSocket, public virtual CFileReadSocket, public virtual CFileWriteSocket {
public:
	CFileRwSocket();
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(int p_notifyPeer);                            // vtable+0x0c
	virtual CNetworkMessage* ReceiveAcknowledgement();                // vtable+0x1c
	virtual void SendAcknowledgement();                               // vtable+0x18
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047ba40 SYMBOL
// ?SocketError@CFileCommonSocket@@WPPPPPOMI@AEXXZ

// SYNTHETIC: LEMBALL 0x0047baf0 SYMBOL
// ?SetDestAddr@CFileWriteSocket@@WPPPPPOLA@AEXPAVCNetworkAddress@@@Z

// SYNTHETIC: LEMBALL 0x0047be30 SYMBOL
// ?SysCloseSocket@CFileCommonSocket@@WPPPPPOMI@AEHXZ

// SYNTHETIC: LEMBALL 0x0047be50
// CFileRwSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bf00 SYMBOL
// ??_ECFileRwSocket@@$4PPPPPPPM@KI@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a718
// CFileRwSocket::`vbtable'{for `CFileWriteSocket'}

// GLOBAL: LEMBALL 0x0049a730
// CFileRwSocket::`vbtable'{for `CFileReadSocket'}

// GLOBAL: LEMBALL 0x0049a748
// CFileRwSocket::`vbtable'{for `CFileCommonSocket'}

// GLOBAL: LEMBALL 0x0049a758
// CFileRwSocket::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a768
// CFileRwSocket::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a770
// CFileRwSocket::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a778
// CFileRwSocket::`vbtable'

#endif
