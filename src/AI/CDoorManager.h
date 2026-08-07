#ifndef LEMBALL_CDOORMANAGER_H
#define LEMBALL_CDOORMANAGER_H

struct CDoor;

// Shared CDoorManager modeled on the Macintosh blueprint class `CDoorManager`.
// Consolidates the four formerly-duplicate local struct definitions
// (CDoorAct.cpp, CDoorSlot.cpp, LVACTDSP.CPP, LVMODESLOT.CPP).

struct CDoorManager {
	unsigned char m_abReserved00[0x34];
	int m_nObjectCount34;                 // 0x34
	int m_nCapacity38;                    // 0x38
	CDoor* m_pObjects3C;                  // 0x3c

	unsigned short Id(int nIndex);
	void Switch(int nAction, unsigned int nSlot);
	void Restart(void);
	int Open(void* pCoord, void* pGameObject);
	void Initialise(int nCount);
	int Add(short nSlot, void* pObjectType, unsigned short nDoorType, int nWorldX, int nWorldY, int nWorldZ);
};

#endif
