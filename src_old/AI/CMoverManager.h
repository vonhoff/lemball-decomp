#ifndef LEMBALL_CMOVERMANAGER_H
#define LEMBALL_CMOVERMANAGER_H

#include "AI/AICoord.h"

class CViewData;
struct CMover;

// Shared CMoverManager modeled on the Macintosh blueprint class `CMoverManager`.
// Owns an array of CMover (stride 0x1a0). Consolidates the formerly-duplicate
// local struct definitions (LEVELSTAT.CPP, LEVELVT.CPP).

struct CMoverManager {
	char m_abReserved00[0x30];
	int m_nCapacity30;                 // 0x30
	int m_cObjects34;                  // 0x34
	CMover* m_pObjects38;              // 0x38
	void* m_pLevelMode3C;              // 0x3c

	CMover* Find(int x, int y, int* pHeight);
	void Remove(CMover* pObject);
	int HasTriggeredObject(void* pEntity, const int* pPosition);
	void ResetObjectCount(void);
	void Initialise(int nCapacity);

	void Add(unsigned short nSlotId, unsigned short nFlags, void* pParam3, int nParam4, void* pParam5);
	int GetViewData(CViewData* pViewData);
	void LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat);
};

#endif
