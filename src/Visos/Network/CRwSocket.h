#ifndef LEMBALL_VISOS_NETWORK_CRWSOCKET_H
#define LEMBALL_VISOS_NETWORK_CRWSOCKET_H

#include "CReadSocket.h"
#include "CWriteSocket.h"

// SIZE 0x108
// VTABLE: LEMBALL 0x00498fe0 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x00499008 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x00499038 CRwSocket
// MSVC's PDB display names collapse the first two tables to the same CBaseSocket
// view, but their COFF symbols retain the CReadSocket/CWriteSocket path. Eight
// original socket constructors install all three tables, and the shared
// deleting destructor at 0x00462cb0 destroys CReadSocket at +0xa8, CWriteSocket
// at +0x30, then CBaseCommonSocket. The forwarding bodies remain inline; their
// emitted versions are mapped below by their COFF symbols.
class CRwSocket : public virtual CBaseCommonSocket, public virtual CWriteSocket, public virtual CReadSocket {
public:
	CRwSocket() {}

	void SendAcknowledgement() { CWriteSocket::SendAcknowledgement(); }

	CNetworkMessage* ReceiveAcknowledgement() { return CWriteSocket::ReceiveAcknowledgement(); }

	void Closed(int p_notifyPeer) { CWriteSocket::Closed(p_notifyPeer); }

	void Process()
	{
		CWriteSocket::Process();
		CReadSocket::Process();
	}

	void SetCBuffers(int p_packetCount, int p_messageCapacity)
	{
		CReadSocket::SetCBuffers(p_packetCount, p_messageCapacity);
		CWriteSocket::SetCBuffers(p_packetCount, p_messageCapacity);
	}

	void SetNCBuffers(unsigned long p_lastSinglePacketMessageId, unsigned long p_lastMessageId, int p_messageCapacity)
	{
		CReadSocket::SetNCBuffers(p_lastSinglePacketMessageId, p_lastMessageId, p_messageCapacity);
		CWriteSocket::SetNCBuffers(p_lastSinglePacketMessageId, p_lastMessageId, p_messageCapacity);
	}
};

// FUNCTION: LEMBALL 0x00462c30 SYMBOL
// ?SendAcknowledgement@CRwSocket@@UAEXXZ

// FUNCTION: LEMBALL 0x00462c50 SYMBOL
// ?ReceiveAcknowledgement@CRwSocket@@UAEPAVCNetworkMessage@@XZ

// SYNTHETIC: LEMBALL 0x00462c70 SYMBOL
// ?SendAcknowledgement@CRwSocket@@$4PPPPPPPM@PPPPPPHA@AEXXZ

// SYNTHETIC: LEMBALL 0x00462c80 SYMBOL
// ?ReceiveAcknowledgement@CRwSocket@@$4PPPPPPPM@PPPPPPHA@AEPAVCNetworkMessage@@XZ

// SYNTHETIC: LEMBALL 0x00462cb0
// CRwSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462cf0 SYMBOL
// ?Closed@CRwSocket@@$4PPPPPPPM@A@AEXH@Z

// FUNCTION: LEMBALL 0x00462d00 SYMBOL
// ?Closed@CRwSocket@@UAEXH@Z

// SYNTHETIC: LEMBALL 0x00462d20 SYMBOL
// ??_ECRwSocket@@$4PPPPPPPM@DA@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462d30 SYMBOL
// ?SendAcknowledgement@CRwSocket@@$4PPPPPPPM@A@AEXXZ

// SYNTHETIC: LEMBALL 0x00462d40 SYMBOL
// ?ReceiveAcknowledgement@CRwSocket@@$4PPPPPPPM@A@AEPAVCNetworkMessage@@XZ

// SYNTHETIC: LEMBALL 0x00462d50 SYMBOL
// ??_ECRwSocket@@$4PPPPPPPM@KI@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00462d60 SYMBOL
// ?SendAcknowledgement@CRwSocket@@$4PPPPPPPM@HI@AEXXZ

// SYNTHETIC: LEMBALL 0x00462d70 SYMBOL
// ?ReceiveAcknowledgement@CRwSocket@@$4PPPPPPPM@HI@AEPAVCNetworkMessage@@XZ

#endif
