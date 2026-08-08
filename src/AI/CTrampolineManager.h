#ifndef LEMBALL_CTRAMPOLINEMANAGER_H
#define LEMBALL_CTRAMPOLINEMANAGER_H

// Mac blueprint: CTrampolineManager owns an array of CTrampoline (stride 0x144).
class CTrampolineManager {
public:
	void Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ);
	void LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat);
	int GetViewData(int param_1);

private:
	unsigned char m_abReserved00[0x30];
	int m_nCapacity30;                      // 0x30
	int m_nObjectCount34;                   // 0x34
	unsigned char* m_pObjects38;            // 0x38
	void* m_pLevelMode3C;                    // 0x3c
};

#endif
