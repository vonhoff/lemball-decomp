#ifndef LEMBALL_CBULLET_H
#define LEMBALL_CBULLET_H

// CBullet view modeled on the Macintosh blueprint class `CBullet`.
// Fields reuse the shared chunk-object conventions. The embedded network
// stream lives at +0x138; AddData/GetData are compiled against that
// secondary base, so their member bodies address this + 0x138.

class CNetworkMessage;

struct CBullet {
	virtual void vf0(void);
	virtual void vf1(void);
	virtual void SetState(int nState);
	virtual void EmitProjectileRenderEntry(void* pRenderEntry);
	virtual void vf4(void);
	virtual int ServiceProjectile(void);
	unsigned char m_abReserved04[0x5c];
	void* m_pSourceContext60;              // 0x60
	unsigned char m_abReserved64[0x34];
	int m_nHeading98;                       // 0x98
	unsigned char m_abReserved9C[0x1c];
	int m_nStateB8;                         // 0xb8
	unsigned char m_abReservedBC[0xc];
	int m_nStartTickC8;                     // 0xc8
	int m_nTerminalTickCC;                  // 0xcc
	unsigned char m_abReservedD0[0x44];
	int m_nPendingState114;                 // 0x114
	unsigned char m_abReserved118[0x20];
	unsigned char* m_pStream138;            // 0x138 embedded network stream
	unsigned char m_abReserved13C[0x1c];
	int m_nRuntimeFlag164;                  // 0x164
	int m_nBulletType168;                   // 0x168 (stream + 0x30)
	int m_nOwner16C;                        // 0x16C (stream + 0x34)
	signed short m_sCaller170;              // 0x170 (stream + 0x38)
	unsigned char m_abReserved172[0x12];
	unsigned char m_abSegmentFixpoint184[0x184 + 0x20 - 0x184];

	int Receive(unsigned short nMessage, CNetworkMessage* pMessage);
	void FireBullet(void);
	void TriggerBullet(void);
	void AddData(void);
	void GetData(void);
	void Free(void);
};

#endif
