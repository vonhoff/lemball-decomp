#ifndef LEMBALL_CLASER_H
#define LEMBALL_CLASER_H

#include "AI/AICoord.h"

class CGameObject;
class CViewData;
class CLaserManager;

class CLaser {
public:
	CLaser(void);
	void Restart(void);
	void Initialise(void);
	~CLaser(void);
	void Set(unsigned short nSlotId, const AICOORD& position, int nObjectType);
	int CheckHits(void);
	int Process(void);
	int Activate(void);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	int GetViewData(CViewData* pViewData);

	friend class CLaserManager;

private:
	unsigned char m_abReserved00[0x64];
	int m_nEntityType64;                   // 0x64
	unsigned char m_abReserved68[0x34];
	AICOORD m_WorldPosition9C;            // 0x9c
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                        // 0xb8
	unsigned char m_abReservedBC[0xc];
	unsigned char m_abReservedC8[4];
	int m_nFrameTickCC;                    // 0xcc
	unsigned char m_abReservedD0[0x68];
	int m_anRuntimeState138[3];            // 0x138..0x143
	CGameObject* m_pTargetObject144;       // 0x144
};

typedef char CLaserSizeCheck[sizeof(CLaser) == 0x148 ? 1 : -1];

#endif
