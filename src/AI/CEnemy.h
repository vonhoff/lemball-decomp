#ifndef LEMBALL_CENEMY_H
#define LEMBALL_CENEMY_H

class CEnemy {
public:
	int m_pVtable00;              // vtable pointer (slot 0 = vtbl)
	char m_abReserved04[0x28];
	int m_nState2C;               // +0x2c
	char m_abReserved30[0x10];
	int m_nSpawnX40;              // +0x40 (world coords, <<12 fixed point)
	int m_nSpawnY44;              // +0x44
	int m_nSpawnZ48;              // +0x48
	char m_abReserved4C[0x18];
	int m_nForm64;                // +0x64 (index into heading-speed table)
	unsigned short m_nRegistryId68; // +0x68
	char m_abReserved6A[2];
	unsigned short m_nSlotId6C;   // +0x6c
	char m_abReserved6E[0x2e];
	int m_nWorldX9C;              // +0x9c (<<12 fixed point)
	int m_nWorldYA0;              // +0xa0
	int m_nWorldZA4;              // +0xa4
	char m_abReservedA8[0xc];
	short m_wHeadingDirB4;        // +0xb4 (0..7 octant)
	short m_wSaveHeadingB6;       // +0xb6
	int m_nStateB8;
	char m_abReservedBC[0x10];
	int m_nUpdateTickCC;          // +0xcc (clock tick of next action)
	char m_abReservedD0[0x3c];
	int m_nHitMinScore10C;        // +0x10c
	char m_abReserved110[0x14];
	int m_nHitBulletState124;     // +0x124
	int m_nReserved128;           // +0x128
	int m_nAction0Rules12C;
	int m_nAction0Kind130;
	void* m_pDesc134;             // +0x134
	int m_nAction1Rules138;
	int m_nAction1Kind13C;
	void* m_pDesc140;             // +0x140
	int m_nAction2Rules144;
	int m_nAction2Kind148;
	void* m_pDesc14C;             // +0x14c
	char m_abReserved150[0xc];
	int m_nTargetX15C;            // +0x15c (<<12 fixed point)
	int m_nTargetY160;            // +0x160
	char m_abReserved164[4];
	int m_nReserved168;           // +0x168

	void Restart(void);
	void EnemyAction_PATROL(void* pDesc);
	int LineOfSight(int param_1, int param_2);
	void TurnToFaceTarget(void);
	void HitBullet(void* pBullet);
	int FacingTarget(void);
	void HitMine(void);
	void Fire(void);
	int Process(void);
	void ProcessAction(int nRules, int nActions, void* pUnion);
	int CheckRadius(int nRadius);
};

#endif // LEMBALL_CENEMY_H
