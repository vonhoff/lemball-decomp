#ifndef LEMBALL_CLASER_H
#define LEMBALL_CLASER_H

#include "AI/AICoord.h"

class CGameObject;
class CViewData;

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

private:
	unsigned char m_abReserved00[0xb8];
	int m_nStateB8;                        // 0xb8
	unsigned char m_abReservedBC[0x7c];
	int m_anRuntimeState138[3];            // 0x138..0x143
	unsigned char m_abReserved144[4];
};

#endif
