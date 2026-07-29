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

// VTABLE: LEMBALL 0x00493e00
struct ManagedEntityStateResetPacket : ManagedEntityPacketBase {
	ManagedEntityStateResetPacket();
};

// VTABLE: LEMBALL 0x00493e20
struct ManagedEntityPacket2A : ManagedEntityPacketBase {
	ManagedEntityPacket2A();
};

#endif
