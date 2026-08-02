#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelPoint3 {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct LevelBounds3 {
	int m_nMinX;
	int m_nMinY;
	int m_nMinZ;
	int m_nMaxX;
	int m_nMaxY;
	int m_nMaxZ;
};

struct ManagedProjectileObjectView {
	void ActivateAtPosition(const LevelPoint3* pPosition);
};

struct ManagedProjectileVirtualView {
	virtual void Reserved00(void);
	virtual void Reserved01(void);
	virtual void Reserved02(void);
	virtual void Reserved03(void);
	virtual void Reserved04(void);
	virtual void Reserved05(void);
	virtual void Reserved06(void);
	virtual void Reserved07(void);
	virtual void Reserved08(void);
	virtual void Reserved09(void);
	virtual void Reserved10(void);
	virtual void Reserved11(void);
	virtual void Reserved12(void);
	virtual void Reserved13(void);
	virtual void Reserved14(void);
	virtual void Reserved15(void);
	virtual void Reserved16(void);
	virtual void Reserved17(void);
	virtual void Reserved18(void);
	virtual void Reserved19(void);
	virtual void Reserved20(void);
	virtual void Reserved21(void);
	virtual void Reserved22(void);
	virtual void Reserved23(void);
	virtual void Reserved24(void);
	virtual void Reserved25(void);
	virtual void Reserved26(void);
	virtual void Reserved27(void);
	virtual void Reserved28(void);
	virtual void Reserved29(void);
	virtual void Reserved30(void);
	virtual void Reserved31(void);
	virtual void Reserved32(void);
	virtual void SetLifetime(int nLifetime);
};

extern void* g_pActiveManagedEntityOwner;

// GLOBAL: LEMBALL 0x004a640c
int g_cActiveManagedEntities;

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation in LEVELVT.CPP.

// FUNCTION: LEMBALL 0x0040fa80
void ManagedProjectileObjectView::ActivateAtPosition(const LevelPoint3* pPosition)
{
	char* pBytes = (char*) this;
	LevelBounds3 bounds;
	int nX;
	int nY;
	int nZ;

	*(int*) (pBytes + 0x9c) = pPosition->m_nX;
	*(int*) (pBytes + 0xa0) = pPosition->m_nY;
	*(int*) (pBytes + 0xa4) = pPosition->m_nZ;
	*(int*) (pBytes + 0x2c) = 0;
	++g_cActiveManagedEntities;
	*(unsigned short*) (pBytes + 0xb4) = 0;
	*(int*) (pBytes + 0x220) = 0;
	*(int*) (pBytes + 0xb8) = 0;
	*(int*) (pBytes + 0x17c) = 0;
	*(int*) (pBytes + 0x178) = 0;
	*(int*) (pBytes + 0x18c) = 0;
	*(int*) (pBytes + 0x188) = 0;
	*(int*) (pBytes + 0x74) = 0;
	*(unsigned short*) (pBytes + 0xc4) = 0;
	*(int*) (pBytes + 0x184) = 0;
	*(int*) (pBytes + 0xf0) = 0;
	*(int*) (pBytes + 0xc0) = 0;
	*(int*) (pBytes + 0x8c) = 0;
	*(int*) (pBytes + 0x08) = 0;
	*(int*) (pBytes + 0x0c) = 0;
	*(int*) (pBytes + 0x104) = 0;
	*(int*) (pBytes + 0x108) = 0;
	*(int*) (pBytes + 0x10c) = 0;
	*(int*) (pBytes + 0x110) = 0xffff;
	*(int*) (pBytes + 0x30) = 0;
	*(int*) (pBytes + 0x34) = 0;
	*(int*) (pBytes + 0x4c) = 0;
	*(int*) (pBytes + 0x50) = 0;
	*(unsigned short*) (pBytes + 0x120) = 0xffff;
	*(int*) (pBytes + 0x54) = 0;
	*(unsigned short*) (pBytes + 0x6e) = 0;
	*(int*) (pBytes + 0x58) = 0;
	*(int*) (pBytes + 0x11c) = 0;
	*(unsigned short*) (pBytes + 0x228) = 0x19;
	((ManagedProjectileVirtualView*) this)->SetLifetime(4000);

	nX = *(int*) (pBytes + 0x9c) >> 12;
	nY = *(int*) (pBytes + 0xa0) >> 12;
	nZ = *(int*) (pBytes + 0xa4) >> 12;
	bounds.m_nMinX = nX - 8;
	bounds.m_nMinY = nY - 8;
	bounds.m_nMinZ = nZ;
	bounds.m_nMaxX = nX + 7;
	bounds.m_nMaxY = nY + 7;
	bounds.m_nMaxZ = nZ + 15;
	*(LevelBounds3*) (pBytes + 0x14) = bounds;

	{
		char* pOwnerBytes = (char*) g_pActiveManagedEntityOwner;
		int* pCount = (int*) (pOwnerBytes + 0x118);
		(*(void***) (pOwnerBytes + 0x120))[*pCount] = this;
		++*pCount;
	}
	**(unsigned short**) (pBytes + 0x70) = 0;
}
