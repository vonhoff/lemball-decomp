#ifndef LEMBALL_VISOS_NETWORK_RWSOCKET_H
#define LEMBALL_VISOS_NETWORK_RWSOCKET_H

#include "ReadSocket.h"
#include "WriteSocket.h"

// SIZE 0x108
// VTABLE: LEMBALL 0x00498fe0 BaseSocket's `ReadSocket
// VTABLE: LEMBALL 0x00499008 BaseSocket's `WriteSocket
// VTABLE: LEMBALL 0x00499038 RwSocket
// MSVC's PDB display names collapse the first two tables to the same BaseSocket
// view, but their COFF symbols retain the ReadSocket/WriteSocket path. Eight
// original socket constructors install all three tables, and the shared
// deleting destructor at 0x00462cb0 destroys ReadSocket at +0xa8, WriteSocket
// at +0x30, then BaseCommonSocket. The forwarding bodies below are intentionally
// inline: the original executable has their effects in annotated derived
// callers, not separate RwSocket functions to which FUNCTION annotations could
// be attached.
class RwSocket : public virtual BaseCommonSocket, public virtual WriteSocket, public virtual ReadSocket {
public:
	RwSocket() {}
	virtual ~RwSocket() {}

	void SendAcknowledgement() { WriteSocket::SendAcknowledgement(); }

	NetworkMessage* ReceiveAcknowledgement() { return WriteSocket::ReceiveAcknowledgement(); }

	void Closed(unsigned char p_arg0) { WriteSocket::Closed(p_arg0); }

	void Process()
	{
		WriteSocket::Process();
		ReadSocket::Process();
	}

	void SetCBuffers(int p_arg0, int p_arg1)
	{
		ReadSocket::SetCBuffers(p_arg0, p_arg1);
		WriteSocket::SetCBuffers(p_arg0, p_arg1);
	}

	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
	{
		ReadSocket::SetNcBuffers(p_arg0, p_arg1, p_arg2);
		WriteSocket::SetNcBuffers(p_arg0, p_arg1, p_arg2);
	}
};

// SYNTHETIC: LEMBALL 0x00462c70 SYMBOL
// ?SendAcknowledgement@RwSocket@@$4PPPPPPPM@PPPPPPHA@AEXXZ

// SYNTHETIC: LEMBALL 0x00462c80 SYMBOL
// ?ReceiveAcknowledgement@RwSocket@@$4PPPPPPPM@PPPPPPHA@AEPAVNetworkMessage@@XZ

// SYNTHETIC: LEMBALL 0x00462cb0
// RwSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462cf0
// RwSocket::Closed`vtordisp{-4, 0}'

// SYNTHETIC: LEMBALL 0x00462d20 SYMBOL
// ??_ERwSocket@@$4PPPPPPPM@DA@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462d30 SYMBOL
// ?SendAcknowledgement@RwSocket@@$4PPPPPPPM@A@AEXXZ

// SYNTHETIC: LEMBALL 0x00462d40 SYMBOL
// ?ReceiveAcknowledgement@RwSocket@@$4PPPPPPPM@A@AEPAVNetworkMessage@@XZ

// SYNTHETIC: LEMBALL 0x00462d50 SYMBOL
// ??_ERwSocket@@$4PPPPPPPM@KI@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462d60 SYMBOL
// ?SendAcknowledgement@RwSocket@@$4PPPPPPPM@HI@AEXXZ

// SYNTHETIC: LEMBALL 0x00462d70 SYMBOL
// ?ReceiveAcknowledgement@RwSocket@@$4PPPPPPPM@HI@AEPAVNetworkMessage@@XZ

#endif
