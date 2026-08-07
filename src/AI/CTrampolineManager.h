#ifndef LEMBALL_CTRAMPOLINEMANAGER_H
#define LEMBALL_CTRAMPOLINEMANAGER_H

// Mac blueprint: CTrampolineManager owns an array of CTrampoline (stride 0x144).
class CTrampolineManager {
public:
	void Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ);

private:
	unsigned char m_abReserved00[0x30];
	int m_nCapacity30;                      // 0x30
	int m_nObjectCount34;                   // 0x34
	unsigned char* m_pObjects38;            // 0x38
};

#endif
