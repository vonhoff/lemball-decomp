#ifndef LEMBALL_CROCKETMANAGER_H
#define LEMBALL_CROCKETMANAGER_H

#include "AI/AICoord.h"

class CGameObject;
class CViewData;

// Mac blueprint: CRocketManager owns an array of CRocket (stride 0x144).

class CRocketManager {
public:
	int m_vtable00;                 // vtable pointer
	unsigned char m_abReserved04[0x2c];
	int m_nCapacity30;              // 0x30
	int m_nObjectCount34;           // 0x34
	unsigned char* m_pObjects38;    // 0x38
	void* m_pLevelMode3C;           // 0x3c

	int GetViewData(CViewData* pViewData);
	void Process(void);
	void Add(unsigned short nSlotId, int nWorldX, int nWorldY, int nWorldZ);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	void LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat);
};

typedef char CRocketManagerSizeCheck[sizeof(CRocketManager) == 0x40 ? 1 : -1];

#endif // LEMBALL_CROCKETMANAGER_H
