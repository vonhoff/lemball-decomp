#ifndef LEMBALL_LEVELRUN_PACKET_H
#define LEMBALL_LEVELRUN_PACKET_H

#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/MEDIA/VSSTRM.H"

// VTABLE: LEMBALL 0x00493d80
struct ManagedEntityPacketBase : VsNetEffStreamCommon {
	ManagedEntityPacketBase(int nPacketType);
	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
	static void operator delete(void* pObject);

	void* m_pManagedEntity2c;
};

// VTABLE: LEMBALL 0x00493da0
struct ManagedEntityPacket23 : ManagedEntityPacketBase {
	ManagedEntityPacket23();

	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
};

// VTABLE: LEMBALL 0x00493dc0
struct ManagedEntityPacket27 : ManagedEntityPacketBase {
	ManagedEntityPacket27();

	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
};

// VTABLE: LEMBALL 0x00493de0
struct ManagedEntityPacket28 : ManagedEntityPacketBase {
	ManagedEntityPacket28();

	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
};

// VTABLE: LEMBALL 0x00493e00
struct ManagedEntityStateResetPacket : ManagedEntityPacketBase {
	ManagedEntityStateResetPacket();
};

// VTABLE: LEMBALL 0x00493e20
struct ManagedEntityPacket2A : ManagedEntityPacketBase {
	ManagedEntityPacket2A();
};

// VTABLE: LEMBALL 0x00493e40
struct ManagedEntityPacket24 : ManagedEntityPacketBase {
	ManagedEntityPacket24();

	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
};

// VTABLE: LEMBALL 0x00493e60
struct ManagedEntityPacket25 : ManagedEntityPacketBase {
	ManagedEntityPacket25();

	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
};

// VTABLE: LEMBALL 0x00493e80
struct ManagedEntityPacket26 : ManagedEntityPacketBase {
	ManagedEntityPacket26();

	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
};

#endif
