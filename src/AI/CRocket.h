#ifndef LEMBALL_CROCKET_H
#define LEMBALL_CROCKET_H

#include "AI/AICoord.h"

class CGameObject;

// Mac blueprint: CRocket is a single rocket firework object (stride 0x144 in
// the CRocketManager array).

class CRocket {
public:
	int m_vtable00;                 // vtable pointer
	unsigned char m_abReserved04[0x58];
	CGameObject* m_pTarget5C;       // 0x5c
	unsigned char m_abReserved60[0x34];
	int m_nFrameTime94;             // 0x94
	unsigned char m_abReserved98[4];
	int m_xPosWorld9C;              // 0x9c
	int m_yPosWorldA0;              // 0xa0
	int m_zPosWorldA4;              // 0xa4
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                 // 0xb8
	unsigned char m_abReservedBC[0xc];
	int m_nMotionStartTickC8;       // 0xc8
	unsigned char m_abReservedCC[0x70];
	int m_nActive13C;               // 0x13c
	int m_nTargetZ140;              // 0x140

	void Set(unsigned short nSlotId, const AICOORD& position);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	void DoActivate(void);
	void Restart(void);
};

typedef char CRocketSizeCheck[sizeof(CRocket) == 0x144 ? 1 : -1];

#endif // LEMBALL_CROCKET_H
