#ifndef LEMBALL_CBULLET_H
#define LEMBALL_CBULLET_H

#include "AI/AICoord.h"

// CBullet view modeled on the Macintosh blueprint class `CBullet`.
// Fields reuse the shared chunk-object conventions. The embedded network
// stream lives at +0x138; AddData/GetData are compiled against that
// secondary base, so their member bodies address this + 0x138.

struct CNetworkMessage;

struct CBulletNetworkStreamState {
	unsigned char m_abReserved00[0x20];
	void* m_pPayload20;
	unsigned char m_abReserved24[8];
};

struct CBullet {
	virtual void vf0(void);
	virtual void vf1(void);
	virtual void SetState(int nState);
	virtual void EmitProjectileRenderEntry(void* pRenderEntry);
	virtual void vf4(void);
	virtual int ServiceProjectile(void);
	unsigned char m_abReserved04[0x5c];
	void* m_pSourceContext60;              // 0x60
	int m_nEntityType64;
	unsigned short m_nBehaviourFlags68;
	unsigned char m_abReserved6A[2];
	unsigned short m_nSlotId6C;             // 0x6C
	unsigned char m_abReserved6E[0x2a];
	int m_nHeading98;                       // 0x98
	int m_nWorldX9C;                        // 0x9C
	int m_nWorldYA0;                        // 0xA0
	int m_nWorldZA4;                        // 0xA4
	int m_nDestXA8;                         // 0xA8
	int m_nDestYAC;                         // 0xAC
	int m_nDestZB0;                         // 0xB0
	unsigned short m_nHeadingOctantB4;      // 0xB4
	unsigned char m_abReservedB6[2];
	int m_nStateB8;                         // 0xb8
	unsigned char m_abReservedBC[0xc];
	int m_nStartTickC8;                     // 0xc8
	int m_nTerminalTickCC;                  // 0xcc
	unsigned char m_abReservedD0[0x44];
	int m_nPendingState114;                 // 0x114
	unsigned char m_abReserved118[0x20];
	CBulletNetworkStreamState m_NetworkStream138; // 0x138
	int m_nRuntimeFlag164;                  // 0x164
	int m_nBulletType168;                   // 0x168 (stream + 0x30)
	int m_nOwner16C;                        // 0x16C (stream + 0x34)
	signed short m_sCaller170;              // 0x170 (stream + 0x38)
	unsigned char m_abReserved172[0x12];
	unsigned char m_abSegmentFixpoint184[0x184 + 0x20 - 0x184];

	int Receive(unsigned short nMessage, CNetworkMessage* pMessage);
	void Set(unsigned short nCaller, int nBulletType, int nOwner, int nDirection,
		AICOORD source, AICOORD target);
	void FireBullet(void);
	void TriggerBullet(void);
	int Process(void);
	void AddData(void);
	void GetData(void);
	void Free(void);
};

typedef char CBulletNetworkStreamStateSizeCheck[sizeof(CBulletNetworkStreamState) == 0x2c ? 1 : -1];
typedef char CBulletSizeCheck[sizeof(CBullet) == 0x1a4 ? 1 : -1];

#endif
