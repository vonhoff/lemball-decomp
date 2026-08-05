#ifndef LEMBALL_CBULLET_H
#define LEMBALL_CBULLET_H

// CBullet view modeled on the Macintosh blueprint class `CBullet`.
// Fields reuse the shared chunk-object conventions (m_nStateB8 at 0xb8, pending state at 0x114).

struct CBullet {
	virtual void vf0(void);
	virtual void vf1(void);
	virtual void SetState(int nState);
	virtual void EmitProjectileRenderEntry(void* pRenderEntry);
	virtual void vf4(void);
	virtual int ServiceProjectile(void);
	unsigned char m_abReserved04[0xb8 - 0x04];
	int m_nStateB8;                     // 0xb8
	unsigned char m_abReservedBC[0x114 - 0xbc];
	int m_nPendingState114;             // 0x114
	unsigned char m_abReserved118[0x164 - 0x118];
	int m_nRuntimeFlag164;              // 0x164

	void Free(void);
};

#endif
