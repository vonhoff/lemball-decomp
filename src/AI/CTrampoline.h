#ifndef LEMBALL_CTRAMPOLINE_H
#define LEMBALL_CTRAMPOLINE_H

// Mac blueprint: CTrampoline chunk object (TRAM). Shared CGameObject-derived layout:
// world position 0x9c/0xa0/0xa4, state 0xb8, active flags 0x138/0x13c.
class CTrampoline {
public:
	void Set(unsigned short nSlot, const int* pPoint);

private:
	unsigned char m_abReserved00[0x9c];
	int m_nWorldX9C;                        // 0x9c
	int m_nWorldY9C;                        // 0xa0
	int m_nWorldZ9C;                        // 0xa4
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                         // 0xb8
	unsigned char m_abReservedBC[0x7c];
	int m_nActiveFlag138;                   // 0x138
	int m_nActiveFlag13C;                   // 0x13c
	unsigned char m_abTail140[4];           // 0x140-0x144
};

typedef char CTrampolineSizeMustMatchArrayStride[sizeof(CTrampoline) == 0x144 ? 1 : -1];

#endif
