#include "AI/CPlayerLemming.h"

extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pLevelDemoPlaybackController;

// FUNCTION: LEMBALL 0x0040f0d0
void CPlayerLemming::SetGroup(CPlayerLemmingGroup* pGroup)
{
	m_pGroup224 = pGroup;
}

// FUNCTION: LEMBALL 0x0040f0e0
CPlayerLemmingGroup* CPlayerLemming::GetGroup(void)
{
	return m_pGroup224;
}

// FUNCTION: LEMBALL 0x0040f0f0
void CPlayerLemming::SetGroupLeader(int fLeader)
{
	m_fGroupLeader17C = fLeader;
}

// FUNCTION: LEMBALL 0x0040f100
void CPlayerLemming::SetGroup(int fGrouped)
{
	m_fGrouped180 = fGrouped;
}

// MACINTOSH: CPlayerLemming::SetBored(int)
// FUNCTION: LEMBALL 0x00410090
void CPlayerLemming::SetBored(int nBaseTime)
{
	unsigned int uRandom;

	uRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1fU & 0x7fffff;
	*(unsigned int*) g_pSessionRandomState = uRandom;
	uRandom = nBaseTime + uRandom % 5000;
	*(unsigned int*) ((char*) this + 0x190) = uRandom;
	uRandom = uRandom - uRandom % 0x42;
	*(unsigned int*) ((char*) this + 0x190) = uRandom;
	uRandom = uRandom / 0x32;
	*(unsigned int*) ((char*) this + 0x190) = uRandom;
	*(unsigned int*) ((char*) this + 0x190) = *(int*) ((char*) this + 0xcc) + uRandom;
}

// MACINTOSH: CPlayerLemming::RandomAction()
// FUNCTION: LEMBALL 0x0040fa10
int CPlayerLemming::RandomAction(void)
{
	int nRandom;
	int nVariant;

	nRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1f & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	nVariant = nRandom % 3;
	*(short*) ((char*) this + 0xbc) = (short) nVariant;
	if (nVariant == 0) {
		nRandom = g_nLevelFrameClockTick + 0x38;
		*(int*) ((char*) this + 0xcc) = nRandom;
		return nRandom;
	}
	if (nVariant != 1) {
		if (nVariant != 2) {
			return nRandom / 3;
		}
		nRandom = g_nLevelFrameClockTick + 0x26;
		*(int*) ((char*) this + 0xcc) = nRandom;
		return nRandom;
	}
	nRandom = g_nLevelFrameClockTick + 0x21;
	*(int*) ((char*) this + 0xcc) = nRandom;
	return nRandom;
}


// FUNCTION: LEMBALL 0x0040f600
void __fastcall CPlayerLemming_HitMine(void* pObject)
{
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	*(int*) ((char*) pObject + 0x10c) = 1;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
	*(int*) ((char*) pObject + 0x2c) = 1;
}
// FUNCTION: LEMBALL 0x0040f960
int __fastcall CPlayerLemming_HasObject(void* pObject, int nUnused, int param_1)
{
	if (param_1 == 5) {
		if (*(short*) ((char*) pObject + 0x228) == 0x32) {
			return 1;
		}
	} else {
		int iVar1 = *(int*) ((char*) pObject + 0x220);
		if (iVar1 != 0xc && iVar1 > 0) {
			int* piVar3 = (int*) ((char*) pObject + 0x1c0);
			int iVar2 = 0;
			do {
				if (*piVar3 == param_1) {
					return 1;
				}
				piVar3++;
				iVar2++;
			} while (iVar2 < iVar1);
		}
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0040f9b0
int __fastcall CPlayerLemming_AddObject(void* pObject, int nUnused, int param_1, int param_2)
{
	if (*(int*) ((char*) pObject + 0x220) == 0xc) {
		return 0;
	}
	if (((int(__fastcall*)(void*, int)) (*(void***) pObject)[0xb4 / 4])(pObject, param_1) != 0) {
		return 0;
	}
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 0x220) * 4 + 0x1c0) = param_1;
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 0x220) * 4 + 0x1f0) = param_2;
	*(int*) ((char*) pObject + 0x220) = *(int*) ((char*) pObject + 0x220) + 1;
	return 1;
}
// FUNCTION: LEMBALL 0x00410250
void __fastcall CPlayerLemming_OnConveyor(void* pThis, int nUnused, int param_1, void* param_2, int param_3)
{
	if (param_1 == 0 && *(int*) ((char*) pThis + 0x188) != 0 && param_3 != 0) {
		((void(__fastcall*)(void*, void*)) 0x402c16)(*(void**) ((char*) pThis + 0x18c), pThis);
	}
	*(int*) ((char*) pThis + 0x188) = param_1;
	*(void**) ((char*) pThis + 0x18c) = param_2;
	if (param_1 != 0) {
		int iVar1 = ((int(__fastcall*)(void*)) (*(void***) *(void**) ((char*) pThis + 0x224) + 0x108 / 4))(*(void**) ((char*) pThis + 0x224));
		if (iVar1 > 1) {
			(*( void(**)(int, void*)) (*(void***) *(void**) ((char*) g_pActiveManagedEntityOwner + 0x15c) + 0x34 / 4))(1, (char*) pThis + 0x6a);
		}
		(*( void(**)(void*)) (*(void***) *(void**) ((char*) pThis + 0x224) + 0x150 / 4))(*(void**) ((char*) pThis + 0x224));
	}
}
// FUNCTION: LEMBALL 0x00410220
void __fastcall CPlayerLemming_Action(void* pObject, int nUnused, int param_1)
{
	*(int*) ((char*) pObject + 0x94) = g_nLevelFrameClockTimeMs;
	if (param_1 == 8) {
		((void(__fastcall*)(void*, int)) 0x402cfc)(pObject, 8);
		return;
	}
	*(int*) ((char*) pObject + 0xb8) = param_1;
}
// FUNCTION: LEMBALL 0x0040fcd0
void __fastcall CPlayerLemming_ExternalControlEnd(void* pThis)
{
	if (*(unsigned short*) ((char*) pThis + 0xbc) != 0 && *(unsigned short*) ((char*) pThis + 0xbc) < 3) {
		(*( void(**)(void)) (*(void***) pThis + 0x68 / 4))();
		(*( void(**)(int)) (*(void***) pThis + 8 / 4))(8);
	} else {
		(*( void(**)(int)) (*(void***) pThis + 8 / 4))(0);
	}
}
// FUNCTION: LEMBALL 0x0040f4b0
int __fastcall CPlayerLemming_FacingTarget(void* pThis)
{
	unsigned int uDir = ((unsigned int(__cdecl*)(int, int, int, int)) 0x401532)(
		(*(int*) ((char*) pThis + 0x9c)) >> 12, (*(int*) ((char*) pThis + 0xa0)) >> 12,
		(*(int*) ((char*) pThis + 0x1b4)) >> 12, (*(int*) ((char*) pThis + 0x1b8)) >> 12);
	return (int) (short) (*(short*) ((char*) pThis + 0xb4)) - (int) uDir == 1;
}
// FUNCTION: LEMBALL 0x0040f430
int __fastcall CPlayerLemming_FacingCursor(void* pThis)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) != 0) {
		return 1;
	}
	int local_4;
	int local_8;
	((void(__fastcall*)(void*, void*, void*)) 0x401e65)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160), &local_4, &local_8);
	unsigned int uDir = ((unsigned int(__cdecl*)(int, int, int, int)) 0x401532)(
		(*(int*) ((char*) pThis + 0x9c)) >> 12, (*(int*) ((char*) pThis + 0xa0)) >> 12,
		local_4, local_8);
	return (int) (short) (*(short*) ((char*) pThis + 0xb4)) - (int) uDir == 1;
}
// FUNCTION: LEMBALL 0x0040f220
void __fastcall CPlayerLemming_TurnToFaceTarget(void* pThis, int nUnused)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		*(int*) ((char*) pThis + 0x9c) >> 12,
		*(int*) ((char*) pThis + 0xa0) >> 12,
		*(int*) ((char*) pThis + 0x1b4) >> 12,
		*(int*) ((char*) pThis + 0x1b8) >> 12);
	if (nOct != (int) *(short*) ((char*) pThis + 0xb4)) {
		if (*(int*) ((char*) 0x49d020 + (nOct - (int) *(short*) ((char*) pThis + 0xb4) & 7) * 4) < 0) {
			((void(__fastcall*)(void*)) 0x4023e7)(pThis);
		} else {
			((void(__fastcall*)(void*)) 0x402068)(pThis);
		}
		(*( void(**)(void*, int)) (*(void***) pThis + 0x84 / 4))(pThis, 0xfa0);
	}
	*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick +
		*(int*) ((char*) 0x49d0b0 + *(int*) ((char*) pThis + 0x64) * 4) / 0x32;
}
// FUNCTION: LEMBALL 0x0040f160
void __fastcall CPlayerLemming_TurnToFaceCursor(void* pThis, int nUnused)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) {
		int local_4 = 0;
		int local_8 = 0;
		((void(__fastcall*)(void*, int*, int*)) 0x401e65)(
			*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160), &local_4, &local_8);
		int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
			*(int*) ((char*) pThis + 0x9c) >> 12,
			*(int*) ((char*) pThis + 0xa0) >> 12,
			local_4, local_8);
		if (nOct != (int) *(short*) ((char*) pThis + 0xb4)) {
			if (*(int*) ((char*) 0x49d020 + (nOct - (int) *(short*) ((char*) pThis + 0xb4) & 7) * 4) < 0) {
				((void(__fastcall*)(void*)) 0x4023e7)(pThis);
			} else {
				((void(__fastcall*)(void*)) 0x402068)(pThis);
			}
			(*( void(**)(void*, int)) (*(void***) pThis + 0x84 / 4))(pThis, 0xfa0);
		}
		*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick +
			*(int*) ((char*) 0x49d070 + *(int*) ((char*) pThis + 0x64) * 4) / 0x32;
	}
}
// FUNCTION: LEMBALL 0x0040f640
void __fastcall CPlayerLemming_GetData(void* pThis, int nUnused)
{
	unsigned short local_e[2];
	*(unsigned int*) ((char*) pThis - 0x9c) = (unsigned int) (unsigned short) ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis) << 12;
	*(unsigned int*) ((char*) pThis - 0x98) = (unsigned int) (unsigned short) ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis) << 12;
	*(unsigned int*) ((char*) pThis - 0x94) = (unsigned int) (unsigned short) ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis) << 12;
	((void(__fastcall*) (void*, unsigned short*)) 0x45f090)(pThis, local_e);
	*(unsigned short*) ((char*) pThis - 0x84) = (unsigned short) (local_e[0] & 7);
	*(unsigned short*) ((char*) pThis - 0x7c) = (unsigned short) ((local_e[0] & 0x38) >> 3);
	((void(__fastcall*) (void*, unsigned short*)) 0x45f090)(pThis, local_e);
	*(unsigned int*) ((char*) pThis - 0x80) = (unsigned int) (local_e[0] & 0xff);
	*(unsigned int*) ((char*) pThis - 0xa0) = (unsigned int) (local_e[0] >> 8);
	*(unsigned int*) ((char*) pThis - 0xa4) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
}
// FUNCTION: LEMBALL 0x0040f6f0
void __fastcall CPlayerLemming_AddData(void* pThis, int nUnused)
{
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, 0x2c);
	((void(__fastcall*) (void*, unsigned char)) 0x45ef60)(pThis, *(unsigned short*) ((char*) pThis + 0x30));
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, *(int*) ((char*) pThis - 0x9c) >> 12);
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, *(int*) ((char*) pThis - 0x98) >> 12);
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, *(int*) ((char*) pThis - 0x94) >> 12);
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, (unsigned short) (((*(unsigned short*) ((char*) pThis - 0x7c) & 7) << 3) | (*(unsigned short*) ((char*) pThis - 0x84) & 7)));
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, (unsigned short) ((*(unsigned char*) ((char*) pThis - 0xa0) << 8) | *(unsigned char*) ((char*) pThis - 0x80)));
	if (*(unsigned int*) ((char*) pThis - 0x5c) > (unsigned int) g_nLevelFrameClockTimeMs) {
		*(unsigned int*) ((char*) pThis - 0x5c) = g_nLevelFrameClockTimeMs;
	}
	((void(__fastcall*) (void*, unsigned int)) 0x45ef10)(pThis, *(unsigned int*) ((char*) pThis - 0x5c));
	*(unsigned int*) ((char*) pThis + 0x2c) = 0;
}
