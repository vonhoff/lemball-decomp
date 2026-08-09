#ifndef LEMBALL_CPAINTGUNMANAGER_H
#define LEMBALL_CPAINTGUNMANAGER_H

// Mac blueprint: CPaintGunManager owns an array of CPaintGun (stride 0x144).
class CViewData;

class CPaintGunManager {
public:
	void Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ, int nSubtype);
	void LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat);
	void Process(void);
	int GetViewData(CViewData* pViewData);

private:
	unsigned char m_abReserved00[0x30];
	int m_nCapacity30;                      // 0x30
	int m_nObjectCount34;                   // 0x34
	unsigned char* m_pObjects38;            // 0x38
	void* m_pLevelMode3C;                    // 0x3c
	~CPaintGunManager(void);
};

#endif
