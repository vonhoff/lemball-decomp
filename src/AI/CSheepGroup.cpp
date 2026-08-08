#define LEMBALL_CSHEEPGROUP_HARDTAIL_METHODS
#include "AI/CSheepGroup.h"

// FUNCTION: LEMBALL 0x0041f820
int CSheepGroup::Process(void)
{
	int iVar3 = 0;
	(*( void(**)(int)) (*(void***) ((void**) m_pVtable00) + 0x138 / 4))(0x18);
	if (m_nGroupElementCount124 > 0) {
		int i;
		for (i = 0; i < m_nGroupElementCount124; i++) {
			void* piVar1 = m_apChildObjects128[i];
			(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
		}
	}
	((void(__fastcall*)(void*)) 0x403765)(this);
	((void(__fastcall*)(void*)) 0x402577)(this);
	return 0;
}

#include "AI/CGameObject.h"

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
	fFound = ((int(__fastcall*)(void*, int, CVSRect*, AICOORD*)) 0x4020b3)(
		pLevelMode, 0, &Rect, &TargetPoint);
	if (fFound == 1) {
		((void(__fastcall*)(void*, int, AICOORD)) 0x401c30)(this, 0, TargetPoint);
		return;
	}
	fFound = ((int(__fastcall*)(void*, int, CVSRect*, AICOORD*)) 0x402f81)(
		pLevelMode, 0, &Rect, &TargetPoint);
	if (fFound == 1) {
		((void(__fastcall*)(void*, int, AICOORD)) 0x401c30)(this, 0, TargetPoint);
		return;
	}
	fFound = ((int(__fastcall*)(void*, int, CVSRect*, AICOORD*)) 0x4030da)(
		pLevelMode, 0, &Rect, &TargetPoint);
	if (fFound == 1) {
		((void(__fastcall*)(void*, int, AICOORD)) 0x401c30)(this, 0, TargetPoint);
	}
}
