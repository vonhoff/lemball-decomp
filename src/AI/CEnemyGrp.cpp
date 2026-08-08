#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "AI/CGenericGroup.h"
#include "AI/CEnemy.h"
#include "AI/CAI.h"
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTick;

struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
};

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct EnmyManagedEntityGroupView : LevelChunkObjectBaseView {
	void* m_pVtable00;
	char m_abReserved04[0x120];

	EnmyManagedEntityGroupView* ConstructEnmyManagedEntityGroup(int nType,
																unsigned short nVariant,
																unsigned short nStateId);
};

extern CGenericGroupChunkVtable g_LEVELVT_EnmyManagedEntityGroupVtable;

struct CEnemyGroup : EnmyManagedEntityGroupView {
	CEnemyGroup(int nAI, unsigned short nObjectManager, unsigned short nFormationManager);
};

// FUNCTION: LEMBALL 0x00420ae0
CEnemyGroup::CEnemyGroup(int nAI, unsigned short nObjectManager, unsigned short nFormationManager)
{
	InitializeLevelChunkObjectBase(nAI, nObjectManager, nFormationManager);
	*(void**) this = &g_LEVELVT_EnmyManagedEntityGroupVtable;
}

// FUNCTION: LEMBALL 0x00420600
void CEnemy::HitBullet(void* pBullet)
{
	if (*(int*) ((char*) pBullet + 0x16c) != 1) {
		m_nHitBulletState124 = 1;
		m_nUpdateTickCC = g_nLevelFrameClockTick + 0x3c;
		short sVar1 = *(short*) ((char*) pBullet + 0xb4);
		m_nState2C = 1;
		m_wHeadingDirB4 = sVar1 + 4U & 7;
	}
}

// FUNCTION: LEMBALL 0x0041fec0
int CEnemy::Process(void)
{
	if (m_nStateB8 != 8) {
		switch (m_nReserved128) {
		case 0:
			ProcessAction(m_nAction0Kind130, m_nAction0Rules12C, &m_pDesc134);
			break;
		case 1:
			ProcessAction(m_nAction1Kind13C, m_nAction1Rules138, &m_pDesc140);
			break;
		case 2:
			ProcessAction(m_nAction2Kind148, m_nAction2Rules144, &m_pDesc14C);
			break;
		}
		((void(__cdecl*)(void*, void*)) 0x4032ba)(g_pActiveManagedEntityOwner, this);
		((void(__cdecl*)(AICOORD*, void*, unsigned short)) 0x40341d)(
			(AICOORD*) ((char*) this + 0x9c), this, m_nRegistryId68);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00420430
void CEnemy::Fire(void)
{
	AICOORD source;
	source.x = m_nWorldX9C;
	source.y = m_nWorldYA0;
	source.z = m_nWorldZA4 + 0xc000;
	((CAI*) g_pActiveManagedEntityOwner)->FireBullet(
		m_nSlotId6C, 0, 1, (int) m_wHeadingDirB4,
		source, *(AICOORD*) ((char*) this + 0x15c));
	m_nReserved168 = 2;
	m_nUpdateTickCC = g_nLevelFrameClockTick + *(int*) ((char*) this + 0x16c) / 0x32;
}
// FUNCTION: LEMBALL 0x004206a0
void CEnemy::HitMine(void)
{
	m_nHitMinScore10C = 1;
	((void(__fastcall*)(void*, int)) 0x402f22)(g_pActiveManagedEntityOwner, 300);
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) this + 0x20 / 4))(vec, 0);
	m_nState2C = 1;
}
// FUNCTION: LEMBALL 0x00420650
int CEnemy::FacingTarget(void)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		m_nWorldX9C >> 12,
		m_nWorldYA0 >> 12,
		m_nTargetX15C >> 12,
		m_nTargetY160 >> 12);
	return (int) m_wHeadingDirB4 == nOct;
}
// FUNCTION: LEMBALL 0x00420350
void CEnemy::TurnToFaceTarget(void)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		m_nWorldX9C >> 12,
		m_nWorldYA0 >> 12,
		m_nTargetX15C >> 12,
		m_nTargetY160 >> 12);
	if (nOct != (int) m_wHeadingDirB4) {
		if (*(int*) ((char*) 0x49d020 + (nOct - (int) m_wHeadingDirB4 & 7) * 4) < 0) {
			((void(__fastcall*)(void*)) 0x4023e7)(this);
		} else {
			((void(__fastcall*)(void*)) 0x402068)(this);
		}
	}
	m_nUpdateTickCC = g_nLevelFrameClockTick +
		*(int*) ((char*) 0x49d0b0 + m_nForm64 * 4) / 0x32;
}
// FUNCTION: LEMBALL 0x0041fcd0
void CEnemy::Restart(void)
{
	((LevelChunkObjectRuntimeStateView*) this)->ResetRuntimeStateThunk();
	m_nWorldX9C = m_nSpawnX40;
	m_nWorldYA0 = m_nSpawnY44;
	m_nWorldZA4 = m_nSpawnZ48;
	m_wHeadingDirB4 = m_wSaveHeadingB6;
	m_nReserved128 = 0;
	m_nReserved168 = 0;
	m_nHitBulletState124 = 0;
	m_nState2C = 0;
	void** ppReg = *(void***) ((char*) g_pActiveManagedEntityOwner + 0x120);
	ppReg[*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118)] = this;
	*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) += 1;
	if (m_pDesc134 != 0) {
		*(int*) ((char*) m_pDesc134 + 0x8) = 0;
		*(int*) ((char*) m_pDesc134 + 0xc) = 1;
	}
	if (m_pDesc140 != 0) {
		*(int*) ((char*) m_pDesc140 + 0x8) = 0;
		*(int*) ((char*) m_pDesc140 + 0xc) = 1;
	}
	if (m_pDesc14C != 0) {
		*(int*) ((char*) m_pDesc14C + 0x8) = 0;
		*(int*) ((char*) m_pDesc14C + 0xc) = 1;
	}
}
// FUNCTION: LEMBALL 0x004202a0
int CEnemy::LineOfSight(int param_1, int param_2)
{
	unsigned int local_8 = param_1 - m_nWorldX9C;
	unsigned int uVar4 = param_2 - m_nWorldYA0;
	unsigned int local_4;
	unsigned int* puVar1;
	unsigned int* puVar2;
	if ((int) local_8 < 0) { local_4 = -(int) local_8; puVar1 = &local_4; }
	else { puVar1 = &local_8; }
	if ((int) uVar4 < 0) { puVar2 = &local_4; local_4 = -(int) uVar4; }
	else { puVar2 = &local_8; local_8 = uVar4; }
	int iVar3 = (int) *puVar2 >> 12;
	unsigned int uVar5 = *puVar2 & 0xfff;
	int iVar5 = (int) ((uVar5 * 0x6a0u) >> 12);
	if (iVar3 * 0x6a0 + iVar5 < (int) *puVar1 && (int) *puVar1 < iVar5 + (iVar3 * 0x1350 + (int) uVar5) * 2) {
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x004200f0
void CEnemy::EnemyAction_PATROL(void* pDesc)
{
	unsigned int local_18 = 0xaa55aa55;
	unsigned int local_14 = 0xaa55aa55;
	unsigned int local_10 = 0xaa55aa55;
	unsigned int local_c[3];
	unsigned int* pPoint;

	if (((int(__fastcall*) ()) 0x401f37)() != 1) {
		unsigned int idx = (unsigned int) *(unsigned short*) (*(int*) (*(int**) pDesc + 0x10) + *(int*) (*(int**) pDesc + 8) * 2);
		pPoint = (unsigned int*) ((unsigned int*(__fastcall*) (void*, unsigned int*, unsigned int)) 0x401410)(*(void**) 0x4a74b0, local_c, idx);
		local_18 = pPoint[0];
		local_14 = pPoint[1];
		local_10 = pPoint[2];
		*(int*) (*(int**) pDesc + 8) += *(int*) (*(int**) pDesc + 0xc);
		{
			int* pBase = *(int**) pDesc;
			int cur = *(int*) ((char*) pBase + 8);
			if (*(int*) ((char*) pBase + 4) <= cur || cur < 0) {
				if (*(int*) pBase == 0) {
					*(int*) ((char*) pBase + 0xc) = -*(int*) ((char*) pBase + 0xc);
					*(int*) ((char*) pBase + 8) += *(int*) ((char*) pBase + 0xc);
				}
				else if (*(int*) pBase == 1) {
					*(int*) ((char*) pBase + 8) = 0;
				}
			}
		}
		((void(__fastcall*) (void*, unsigned int*)) 0x401d52)(this, &local_18);
	}
}


// FUNCTION: LEMBALL 0x0041ff60
void CEnemy::ProcessAction(int nRules, int nActions, void* pUnion)
{
	if (((int(__cdecl*)(int)) 0x4030f3)(nRules) == 1) {
		if (++m_nReserved128 > 2) {
			m_nReserved128 = 0;
		}
		return;
	}
	switch (nRules - 1) {
	case 0:
		((void (__fastcall*)(void*, int)) 0x402bda)(this, nActions);
		break;
	case 1:
		((void (__fastcall*)(void*, int)) 0x401663)(this, nActions);
		break;
	case 2:
		((void (__fastcall*)(void*, int)) 0x401681)(this, nActions);
		break;
	case 3:
		((void (__fastcall*)(void*, int)) 0x4010b4)(this, nActions);
		break;
	default:
		((void (__fastcall*)(void*)) 0x40207c)(this);
		break;
	}
}

// FUNCTION: LEMBALL 0x00420200
int CEnemy::CheckRadius(int nRadius)
{
	unsigned short aRect[4];
	void* pOwner = g_pActiveManagedEntityOwner;

	aRect[0] = 0;
	aRect[1] = 0;
	aRect[2] = (unsigned short) ((m_nWorldX9C >> 12) - nRadius);
	aRect[3] = (unsigned short) ((m_nWorldYA0 >> 12) - nRadius);
	if (((int (__fastcall*)(void*, void*, void*, int)) 0x4020b3)(pOwner, aRect, (char*) this + 0x150, 2)) {
		return 1;
	}
	aRect[0] = 0;
	aRect[1] = 0;
	return ((int (__fastcall*)(void*, void*, void*, int)) 0x40175d)(pOwner, aRect, (char*) this + 0x150, 2);
}
