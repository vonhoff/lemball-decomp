#include "AI/CBullet.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
typedef unsigned long DWORD;
#include "Platform/Windows/Mixed/Engine/MEDIA/VSSTRM.H"

extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x0041a5c0
void CBullet::Set(unsigned short nCaller, int nBulletType, int nOwner, int nDirection,
	AICOORD source, AICOORD target)
{
	int nTileX;
	int nTileY;
	unsigned short nHeight;

	(void) nDirection;
	m_nBulletType168 = nBulletType;
	m_nOwner16C = nOwner;
	m_nWorldX9C = source.x;
	m_nWorldYA0 = source.y;
	m_nWorldZA4 = source.z;
	m_nDestXA8 = target.x;
	m_nDestYAC = target.y;
	m_nRuntimeFlag164 = 1;
	nTileX = (target.x >> 12) >> 4;
	nTileY = (target.y >> 12) >> 4;
	if ((target.x >> 12) < 0 || (target.y >> 12) < 0 ||
		*(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
		*(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY) {
		nHeight = 0;
	}
	else {
		void* pTile = (char*) *(void**) ((char*) g_pLevelTileGrid + 0x0c) +
			(nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 12;
		nHeight = ((unsigned short(__fastcall*)(void*, int, int)) 0x4029a5)(
			pTile, target.x >> 12 & 0xf, target.y >> 12 & 0xf);
	}
	m_sCaller170 = nCaller;
	m_nDestZB0 = (nHeight + 12) << 12;
	m_nHeadingOctantB4 = (unsigned short) ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		m_nWorldX9C >> 12, m_nWorldYA0 >> 12, m_nDestXA8 >> 12, m_nDestYAC >> 12);
}

// FUNCTION: LEMBALL 0x0041aca0
int CBullet::Receive(unsigned short nMessage, CNetworkMessage* pMessage)
{
	if (nMessage != 0x2b) {
		return ((int(__fastcall*)(void*, short, void*)) 0x402342)(this, nMessage, pMessage);
	}
	int iVar2 = ((int(__fastcall*)(void*, void*)) 0x45f280)(&m_NetworkStream138, pMessage->m_nReserved20);
	if (iVar2 != 0) {
		pMessage->m_nReserved20 = (unsigned char*) m_NetworkStream138.m_pPayload20;
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
	if (g_nSelectedNetworkLobbyPeerId != 0) {
		((void(__fastcall*)(void*, void*)) 0x40126c)(m_pSourceContext60, &m_NetworkStream138);
	}
}
// FUNCTION: LEMBALL 0x0041a6d0
void CBullet::TriggerBullet(void)
{
	int aPos[3] = {
		m_nWorldX9C >> 12,
		m_nWorldYA0 >> 12,
		m_nWorldZA4 >> 12
	};
	int aDest[3] = {
		m_nDestXA8 >> 12,
		m_nDestYAC >> 12,
		m_nDestZB0 >> 12
	};
	((void(__fastcall*) (void*, int*, int*, int, int)) 0x4027de)(m_abSegmentFixpoint184, aPos, aDest, m_nStartTickC8, 0xc);
	m_nStateB8 = 0x1b;
	m_nTerminalTickCC = m_nStartTickC8 + 10;
}
// FUNCTION: LEMBALL 0x0041aaa0
void CBullet::AddData(void)
{
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, 0x2b);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nSlotId6C);
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(&m_NetworkStream138, 0, g_nLevelFrameClockTimeMs);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nWorldX9C >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nWorldYA0 >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nWorldZA4 >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nDestXA8 >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nDestYAC >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nDestZB0 >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_nHeadingOctantB4);
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(&m_NetworkStream138, 0, m_nHeading98);
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(&m_NetworkStream138, 0, m_nStartTickC8);
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(&m_NetworkStream138, 0, m_nBulletType168);
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(&m_NetworkStream138, 0, m_nOwner16C);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(&m_NetworkStream138, 0, m_sCaller170);
}
// FUNCTION: LEMBALL 0x0041ab80
void CBullet::GetData(void)
{
	((void(__cdecl*) (unsigned int)) 0x403107)(((unsigned int(__fastcall*) (void*)) 0x45eff0)(&m_NetworkStream138));
	m_nWorldX9C = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138)) << 12;
	m_nWorldYA0 = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138)) << 12;
	m_nWorldZA4 = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138)) << 12;
	m_nDestXA8 = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138)) << 12;
	m_nDestYAC = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138)) << 12;
	m_nDestZB0 = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138)) << 12;
	m_nHeadingOctantB4 = ((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138);
	m_nHeading98 = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(&m_NetworkStream138);
	m_nStartTickC8 = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(&m_NetworkStream138);
	m_nBulletType168 = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(&m_NetworkStream138);
	unsigned int nData = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(&m_NetworkStream138);
	m_nOwner16C = nData;
	if (nData == 0) {
		m_nOwner16C = 2;
	}
	m_sCaller170 = ((unsigned short(__fastcall*) (void*)) 0x45f070)(&m_NetworkStream138);
	m_nRuntimeFlag164 = 1;
	m_nPendingState114 = 1;
}
