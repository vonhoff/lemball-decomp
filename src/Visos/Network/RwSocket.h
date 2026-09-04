#ifndef LEMBALL_VISOS_NETWORK_RWSOCKET_H
#define LEMBALL_VISOS_NETWORK_RWSOCKET_H

#include "ReadSocket.h"
#include "WriteSocket.h"

// SIZE 0x108
// VTABLE: LEMBALL 0x00499038
// Original construction tables 0x00498fe0 and 0x00499008 are the two
// identically named BaseSocket views reached through ReadSocket and WriteSocket.
// reccmp cannot disambiguate their duplicate MSVC vtable names. Eight original
// socket constructors install all three tables, and the shared deleting
// destructor at 0x00462cb0 destroys ReadSocket at +0xa8, WriteSocket at +0x30,
// then BaseCommonSocket. The forwarding bodies below are intentionally inline:
// the original executable has their effects in annotated derived callers, not
// separate RwSocket functions to which FUNCTION annotations could be attached.
class RwSocket : public virtual BaseCommonSocket, public virtual WriteSocket, public virtual ReadSocket {
public:
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

// SYNTHETIC: LEMBALL 0x00462cb0
// RwSocket::`scalar deleting destructor'

#endif
