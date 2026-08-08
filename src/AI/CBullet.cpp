#include "AI/CBullet.h"

extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;

// FUNCTION: LEMBALL 0x0041aca0
int CBullet::Receive(unsigned short nMessage, CNetworkMessage* pMessage)
{
	if (nMessage != 0x2b) {
		return ((int(__fastcall*)(void*, short, void*)) 0x402342)(this, nMessage, pMessage);
	}
	int iVar2 = ((int(__fastcall*)(void*, void*)) 0x45f280)((void*) ((char*) this + 0x138), *(void**) ((char*) pMessage + 0x20));
	if (iVar2 != 0) {
		*(void**) ((char*) pMessage + 0x20) = *(void**) ((char*) this + 0x158);
	}
	((void(__fastcall*)(void*, void*)) 0x401299)(m_pSourceContext60, this);
	((void(__fastcall*)(void*)) 0x40303a)(this);
	return 1;
}
// FUNCTION: LEMBALL 0x0041a760
void CBullet::FireBullet(void)
{
	m_nStartTickC8 = g_nLevelFrameClockTick;
	((void(__fastcall*)(void*)) 0x40303a)(this);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*)(void*, void*)) 0x40126c)(m_pSourceContext60, (char*) this + 0x138);
	}
}
// FUNCTION: LEMBALL 0x0041a6d0
void CBullet::TriggerBullet(void)
{
	int aPos[3] = {
		*(int*) ((char*) this + 0x9c) >> 12,
		*(int*) ((char*) this + 0xa0) >> 12,
		*(int*) ((char*) this + 0xa4) >> 12
	};
	int aDest[3] = {
		*(int*) ((char*) this + 0xa8) >> 12,
		*(int*) ((char*) this + 0xac) >> 12,
		*(int*) ((char*) this + 0xb0) >> 12
	};
	((void(__fastcall*) (void*, int*, int*, int, int)) 0x4027de)((char*) this + 0x184, aPos, aDest, m_nStartTickC8, 0xc);
	m_nStateB8 = 0x1b;
	m_nTerminalTickCC = m_nStartTickC8 + 10;
}
// FUNCTION: LEMBALL 0x0041aaa0
void CBullet::AddData(void)
{
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, 0x2b);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(unsigned short*) ((char*) this + 0x6c));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)((char*) this + 0x138, 0, g_nLevelFrameClockTimeMs);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(int*) ((char*) this + 0x9c) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(int*) ((char*) this + 0xa0) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(int*) ((char*) this + 0xa4) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(int*) ((char*) this + 0xa8) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(int*) ((char*) this + 0xac) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(int*) ((char*) this + 0xb0) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(unsigned short*) ((char*) this + 0xb4));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)((char*) this + 0x138, 0, *(unsigned int*) ((char*) this + 0x98));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)((char*) this + 0x138, 0, *(unsigned int*) ((char*) this + 0xc8));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)((char*) this + 0x138, 0, m_nBulletType168);
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)((char*) this + 0x138, 0, m_nOwner16C);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)((char*) this + 0x138, 0, *(unsigned short*) ((char*) this + 0x170));
}
// FUNCTION: LEMBALL 0x0041ab80
void CBullet::GetData(void)
{
	((void(__cdecl*) (unsigned int)) 0x403107)(((unsigned int(__fastcall*) (void*)) 0x45eff0)((char*) this + 0x138));
	*(unsigned int*) ((char*) this + 0x9c) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138)) << 12;
	*(unsigned int*) ((char*) this + 0xa0) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138)) << 12;
	*(unsigned int*) ((char*) this + 0xa4) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138)) << 12;
	*(unsigned int*) ((char*) this + 0xa8) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138)) << 12;
	*(unsigned int*) ((char*) this + 0xac) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138)) << 12;
	*(unsigned int*) ((char*) this + 0xb0) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138)) << 12;
	*(unsigned short*) ((char*) this + 0xb4) = ((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138);
	*(unsigned int*) ((char*) this + 0x98) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)((char*) this + 0x138);
	*(unsigned int*) ((char*) this + 0xc8) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)((char*) this + 0x138);
	*(unsigned int*) ((char*) this + 0x168) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)((char*) this + 0x138);
	unsigned int nData = ((unsigned int(__fastcall*) (void*)) 0x45eff0)((char*) this + 0x138);
	*(unsigned int*) ((char*) this + 0x16c) = nData;
	if (nData == 0) {
		*(unsigned int*) ((char*) this + 0x16c) = 2;
	}
	*(unsigned short*) ((char*) this + 0x170) = ((unsigned short(__fastcall*) (void*)) 0x45f070)((char*) this + 0x138);
	*(int*) ((char*) this + 0x164) = 1;
	*(int*) ((char*) this + 0x114) = 1;
}
