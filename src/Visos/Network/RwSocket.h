#ifndef LEMBALL_VISOS_NETWORK_RWSOCKET_H
#define LEMBALL_VISOS_NETWORK_RWSOCKET_H

#include "ReadSocket.h"
#include "WriteSocket.h"

// SIZE 0x108
// VTABLE: LEMBALL 0x00498fe0 ReadSocket virtual base
// VTABLE: LEMBALL 0x00499008 WriteSocket virtual base
// VTABLE: LEMBALL 0x00499038 BaseCommonSocket virtual base
// The x86 construction vtables and shared deleting destructor at 0x00462cb0
// establish this intermediate virtual-base aggregate.
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
