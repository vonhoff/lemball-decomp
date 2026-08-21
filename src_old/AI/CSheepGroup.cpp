#define LEMBALL_CSHEEPGROUP_HARDTAIL_METHODS
#include "AI/CSheepGroup.h"
#include "AI/CGameObject.h"

// FUNCTION: LEMBALL 0x0041F530
void CSheepGroup::RunAway(int nX, int nY, int nZ)
{
	void** pVtable;
	void* pChild;
	void* pFirstChild;
	int nPending;
	int nDirection;
	int nAngle;
	int nSin;
	int nCos;
	int* pTrig;
	int* pRandom;

	pVtable = (void**) m_pVtable00;
	nPending = 0;
	pChild = ((void*(__fastcall*) (void*, int) ) pVtable[0x114 / 4])(this, 0);
	while (pChild != 0) {
		if (((int(__fastcall*)(void*, int)) 0x401f37)(pChild, 0) == 1) {
			++nPending;
		}
		pChild = ((void*(__fastcall*) (void*, int) ) pVtable[0x118 / 4])(this, 0);
	}
	if (nPending != 0) {
		return;
	}
	pFirstChild = ((void*(__fastcall*) (void*, int) ) pVtable[0x114 / 4])(this, 0);
	if (pFirstChild == 0) {
		return;
	}
	((void(__fastcall*)(void*, int, int))(*(void***) pFirstChild)[0x34 / 4])(pFirstChild, 0, 4);
	nDirection = ((int(__cdecl*)(int, int, int, int)) 0x401532)(*(int*) ((char*) pFirstChild + 0x9c) >> 12,
																*(int*) ((char*) pFirstChild + 0xa0) >> 12,
																nX >> 12,
																nY >> 12);
	nDirection = (nDirection + 1) & 7;
	nAngle = nDirection << 6;
	pTrig = *(int**) 0x4a13c0;
	nSin = pTrig[nAngle & 0x1ff];
	nCos = pTrig[(nAngle + 0x80) & 0x1ff];
	(void) nSin;
	(void) nCos;
	(void) nZ;
	pRandom = *(int**) 0x4a1bcc;
	*pRandom = (*pRandom * 0x29 + 0x1f) & 0x7fffff;
	((void(__fastcall*)(void*, int, int)) pVtable[0x110 / 4])(this, 0, *pRandom % 3);
	((void(__fastcall*)(void*, int)) pVtable[0x148 / 4])(this, 0);
	*(int*) ((char*) this + 0x168) = 1;
}

struct SheepGroupQueryView {
	virtual void ReservedSlot00(void);
	virtual void ReservedSlot01(void);
	virtual void ReservedSlot02(void);
	virtual void ReservedSlot03(void);
	virtual void ReservedSlot04(void);
	virtual void ReservedSlot05(void);
	virtual void ReservedSlot06(void);
	virtual void ReservedSlot07(void);
	virtual void ReservedSlot08(void);
	virtual void ReservedSlot09(void);
	virtual void ReservedSlot10(void);
	virtual void ReservedSlot11(void);
	virtual void ReservedSlot12(void);
	virtual void ReservedSlot13(void);
	virtual void ReservedSlot14(void);
	virtual void ReservedSlot15(void);
	virtual void ReservedSlot16(void);
	virtual void ReservedSlot17(void);
	virtual void BuildTargetQueryRect(CVSRect* pRect);
};

struct LevelManagedEntityOwnerView {
	int FindPlasChunkObjectPositionInRect(void* pRect, void* pPosition);
	int FindEnmyChunkObjectPositionInRect(void* pRect, void* pPosition);
	int FindShpgChunkObjectPositionInRect(void* pRect, void* pPosition);
};

// FUNCTION: LEMBALL 0x0041f730
void CSheepGroup::CheckAgainstLemmings(void)
{
	CVSRect Rect;
	AICOORD TargetPoint;
	void* pLevelMode;
	int fFound;

	TargetPoint.x = (int) 0xaa55aa55;
	TargetPoint.y = (int) 0xaa55aa55;
	TargetPoint.z = (int) 0xaa55aa55;
	Rect.width = 0;
	Rect.height = 0;
	Rect.x = 0;
	Rect.y = 0;
	((SheepGroupQueryView*) this)->BuildTargetQueryRect(&Rect);
	pLevelMode = *(void**) 0x4a7824;
	fFound = ((LevelManagedEntityOwnerView*) pLevelMode)
		->FindPlasChunkObjectPositionInRect(&Rect, &TargetPoint);
	if (fFound == 1) {
		((void(__fastcall*)(void*, int, AICOORD)) 0x401c30)(this, 0, TargetPoint);
		return;
	}
	fFound = ((LevelManagedEntityOwnerView*) pLevelMode)
		->FindEnmyChunkObjectPositionInRect(&Rect, &TargetPoint);
	if (fFound == 1) {
		((void(__fastcall*)(void*, int, AICOORD)) 0x401c30)(this, 0, TargetPoint);
		return;
	}
	fFound = ((LevelManagedEntityOwnerView*) pLevelMode)
		->FindShpgChunkObjectPositionInRect(&Rect, &TargetPoint);
	if (fFound == 1) {
		((void(__fastcall*)(void*, int, AICOORD)) 0x401c30)(this, 0, TargetPoint);
	}
}

// FUNCTION: LEMBALL 0x0041f820
int CSheepGroup::Process(void)
{
	(*(void (**)(int))(*(void***) ((void**) m_pVtable00) + 0x138 / 4))(0x18);
	if (m_nGroupElementCount124 > 0) {
		int i;
		for (i = 0; i < m_nGroupElementCount124; i++) {
			void* pChild = m_apChildObjects128[i];
			(*(void (**)(void))(*(void***) pChild + 0x14 / 4))();
		}
	}
	((void(__fastcall*)(void*)) 0x403765)(this);
	((void(__fastcall*)(void*)) 0x402577)(this);
	return 0;
}
