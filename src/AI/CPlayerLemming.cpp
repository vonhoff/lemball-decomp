#define LEMBALL_CPLAYERLEMMING_START_STANDING
#include "AI/CPlayerLemming.h"

extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pLevelDemoPlaybackController;
extern void* g_pLevelTileGrid;
extern int g_cActiveManagedEntities;
extern void RequestLocalLevelGameStateChange(void* pLevelMode, int nState);

struct LevelTileGridOwnerView {
	unsigned short GetZ(int x, int y, void** ppMoveChunk);
};

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

// FUNCTION: LEMBALL 0x00410090
void CPlayerLemming::SetBored(int nBaseTime)
{
	unsigned int uRandom;

	uRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1fU & 0x7fffff;
	*(unsigned int*) g_pSessionRandomState = uRandom;
	uRandom = nBaseTime + uRandom % 5000;
	m_nBoredTime190 = uRandom;
	uRandom = uRandom - uRandom % 0x42;
	m_nBoredTime190 = uRandom;
	uRandom = uRandom / 0x32;
	m_nBoredTime190 = uRandom;
	m_nBoredTime190 = m_nNextUpdateTickCC + uRandom;
}

// FUNCTION: LEMBALL 0x0040fa10
int CPlayerLemming::RandomAction(void)
{
	int nRandom;
	int nVariant;

	nRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1f & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	nVariant = nRandom % 3;
	m_nActionPhaseBC = (short) nVariant;
	if (nVariant == 0) {
		nRandom = g_nLevelFrameClockTick + 0x38;
		m_nNextUpdateTickCC = nRandom;
		return nRandom;
	}
	if (nVariant != 1) {
		if (nVariant != 2) {
			return nRandom / 3;
		}
		nRandom = g_nLevelFrameClockTick + 0x26;
		m_nNextUpdateTickCC = nRandom;
		return nRandom;
	}
	nRandom = g_nLevelFrameClockTick + 0x21;
	m_nNextUpdateTickCC = nRandom;
	return nRandom;
}

// FUNCTION: LEMBALL 0x0040f500
void CPlayerLemming::Die(void)
{
	typedef void RequestStateProc(int nState);
	void* pChild;
	void** ppActive;
	int cActive;
	int i;

	for (i = 0; i < m_nObjectCount220; ++i) {
		pChild = (void*) m_anObjectValues1F0[i];
		switch (*(int*) ((char*) pChild + 0x64)) {
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x27:
		case 0x29:
		case 0x2b:
		case 0x2d:
			((void(__fastcall*)(void*)) 0x401415)(pChild);
			break;
		}
	}

	cActive = *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118);
	if (cActive > 0) {
		ppActive = *(void***) ((char*) g_pActiveManagedEntityOwner + 0x120);
		for (i = 0; i < cActive; ++i) {
			if (ppActive[i] == this) {
				--*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118);
				while (i < *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118)) {
					ppActive[i] = ppActive[i + 1];
					++i;
				}
				ppActive[*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118)] = 0;
				break;
			}
		}
	}
	--g_cActiveManagedEntities;
	if (g_cActiveManagedEntities == 0) {
		((RequestStateProc*) RequestLocalLevelGameStateChange)(5);
	}
}

// FUNCTION: LEMBALL 0x0040f600
void CPlayerLemming::HitMine(void)
{
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	m_nJumpField10C = 1;
	vec[2] = 0xa000;
	((void (**)(void*, int)) m_pVtable00)[0x20 / 4](vec, 0);
	m_nAirMotionState2C = 1;
}
// FUNCTION: LEMBALL 0x0040f960
int CPlayerLemming::HasObject(int nObjectType)
{
	if (nObjectType == 5) {
		if (m_nLatchObjectType228 == 0x32) {
			return 1;
		}
	}
	else {
		int iVar1 = m_nObjectCount220;
		if (iVar1 != 0xc && iVar1 > 0) {
			int* piVar3 = m_anObjectTypes1C0;
			int iVar2 = 0;
			do {
				if (*piVar3 == nObjectType) {
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
int CPlayerLemming::AddObject(int nObjectType, int nObject)
{
	if (m_nObjectCount220 == 0xc) {
		return 0;
	}
	if (((int(__fastcall*)(void*, int))((void**) m_pVtable00)[0xb4 / 4])(this, nObjectType) != 0) {
		return 0;
	}
	m_anObjectTypes1C0[m_nObjectCount220] = nObjectType;
	m_anObjectValues1F0[m_nObjectCount220] = nObject;
	m_nObjectCount220 = m_nObjectCount220 + 1;
	return 1;
}
// FUNCTION: LEMBALL 0x00410250
void CPlayerLemming::OnConveyor(int nOnConveyor, void* pIce, int nDetach)
{
	if (nOnConveyor == 0 && *(int*) ((char*) this + 0x188) != 0 && nDetach != 0) {
		((void(__fastcall*)(void*, void*)) 0x402c16)(*(void**) ((char*) this + 0x18c), this);
	}
	*(int*) ((char*) this + 0x188) = nOnConveyor;
	*(void**) ((char*) this + 0x18c) = pIce;
	if (nOnConveyor != 0) {
		int iVar1 = ((int(__fastcall*)(void*))(*(void***) *(void**) ((char*) this + 0x224) + 0x108 / 4))(
			*(void**) ((char*) this + 0x224));
		if (iVar1 > 1) {
			(*(void (**)(int, void*))(*(void***) *(void**) ((char*) g_pActiveManagedEntityOwner + 0x15c) +
									  0x34 / 4))(1, (char*) this + 0x6a);
		}
		(*(void (**)(void*))(*(void***) *(void**) ((char*) this + 0x224) + 0x150 / 4))(
			*(void**) ((char*) this + 0x224));
	}
}
// FUNCTION: LEMBALL 0x00410220
void CPlayerLemming::Action(int nAction)
{
	m_nFrameTime94 = g_nLevelFrameClockTimeMs;
	if (nAction == 8) {
		((void(__fastcall*)(void*, int)) 0x402cfc)(this, 8);
		return;
	}
	m_nStateB8 = nAction;
}
// FUNCTION: LEMBALL 0x0040fcd0
void CPlayerLemming::ExternalControlEnd(void)
{
	if ((unsigned short) m_nActionPhaseBC != 0 && (unsigned short) m_nActionPhaseBC < 3) {
		((void (**)(void)) m_pVtable00)[0x68 / 4]();
		((void (**)(int)) m_pVtable00)[8 / 4](8);
	}
	else {
		((void (**)(int)) m_pVtable00)[8 / 4](0);
	}
}
// FUNCTION: LEMBALL 0x0040f4b0
int CPlayerLemming::FacingTarget(void)
{
	unsigned int uDir = ((unsigned int(__cdecl*)(int, int, int, int)) 0x401532)(m_WorldPosition9C.x >> 12,
																				m_WorldPosition9C.y >> 12,
																				m_nTargetWorldX1B4 >> 12,
																				m_nTargetWorldY1B8 >> 12);
	return (int) m_nHeadingOctantB4 - (int) uDir == 1;
}
// FUNCTION: LEMBALL 0x0040f430
int CPlayerLemming::FacingCursor(void)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) != 0) {
		return 1;
	}
	int local_4;
	int local_8;
	((void(__fastcall*)(void*, void*, void*)) 0x401e65)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160),
														&local_4,
														&local_8);
	unsigned int uDir = ((unsigned int(__cdecl*)(int, int, int, int)) 0x401532)(m_WorldPosition9C.x >> 12,
																				m_WorldPosition9C.y >> 12,
																				local_4,
																				local_8);
	return (int) m_nHeadingOctantB4 - (int) uDir == 1;
}
// FUNCTION: LEMBALL 0x0040f220
void CPlayerLemming::TurnToFaceTarget(void)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(*(int*) ((char*) this + 0x9c) >> 12,
															  *(int*) ((char*) this + 0xa0) >> 12,
															  *(int*) ((char*) this + 0x1b4) >> 12,
															  *(int*) ((char*) this + 0x1b8) >> 12);
	if (nOct != (int) *(short*) ((char*) this + 0xb4)) {
		if (*(int*) ((char*) 0x49d020 + (nOct - (int) *(short*) ((char*) this + 0xb4) & 7) * 4) < 0) {
			((void(__fastcall*)(void*)) 0x4023e7)(this);
		}
		else {
			((void(__fastcall*)(void*)) 0x402068)(this);
		}
		(*(void (**)(void*, int))(*(void***) this + 0x84 / 4))(this, 0xfa0);
	}
	*(int*) ((char*) this + 0xcc) =
		g_nLevelFrameClockTick + *(int*) ((char*) 0x49d0b0 + *(int*) ((char*) this + 0x64) * 4) / 0x32;
}
// FUNCTION: LEMBALL 0x0040f160
void CPlayerLemming::TurnToFaceCursor(void)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) {
		int local_4 = 0;
		int local_8 = 0;
		((void(__fastcall*)(void*, int*, int*)) 0x401e65)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160),
														  &local_4,
														  &local_8);
		int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(m_WorldPosition9C.x >> 12,
																  m_WorldPosition9C.y >> 12,
																  local_4,
																  local_8);
		if (nOct != (int) m_nHeadingOctantB4) {
			if (*(int*) ((char*) 0x49d020 + (nOct - (int) m_nHeadingOctantB4 & 7) * 4) < 0) {
				((void(__fastcall*)(void*)) 0x4023e7)(this);
			}
			else {
				((void(__fastcall*)(void*)) 0x402068)(this);
			}
			((void (**)(void*, int)) m_pVtable00)[0x84 / 4](this, 0xfa0);
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + *(int*) ((char*) 0x49d070 + m_nEntityType64 * 4) / 0x32;
	}
}
// FUNCTION: LEMBALL 0x0040f640
void CPlayerLemming::GetData(void)
{
	unsigned short local_e[2];
	*(unsigned int*) ((char*) this - 0x9c) =
		(unsigned int) (unsigned short) ((unsigned short(__fastcall*)(void*)) 0x45f070)(this) << 12;
	*(unsigned int*) ((char*) this - 0x98) =
		(unsigned int) (unsigned short) ((unsigned short(__fastcall*)(void*)) 0x45f070)(this) << 12;
	*(unsigned int*) ((char*) this - 0x94) =
		(unsigned int) (unsigned short) ((unsigned short(__fastcall*)(void*)) 0x45f070)(this) << 12;
	((void(__fastcall*)(void*, unsigned short*)) 0x45f090)(this, local_e);
	*(unsigned short*) ((char*) this - 0x84) = (unsigned short) (local_e[0] & 7);
	*(unsigned short*) ((char*) this - 0x7c) = (unsigned short) ((local_e[0] & 0x38) >> 3);
	((void(__fastcall*)(void*, unsigned short*)) 0x45f090)(this, local_e);
	*(unsigned int*) ((char*) this - 0x80) = (unsigned int) (local_e[0] & 0xff);
	*(unsigned int*) ((char*) this - 0xa0) = (unsigned int) (local_e[0] >> 8);
	*(unsigned int*) ((char*) this - 0xa4) = ((unsigned int(__fastcall*)(void*)) 0x45eff0)(this);
}
// FUNCTION: LEMBALL 0x0040f6f0
void CPlayerLemming::AddData(void)
{
	((void(__fastcall*)(void*, unsigned short)) 0x45ef40)(this, 0x2c);
	((void(__fastcall*)(void*, unsigned char)) 0x45ef60)(this, *(unsigned short*) ((char*) this + 0x30));
	((void(__fastcall*)(void*, unsigned short)) 0x45ef40)(this, *(int*) ((char*) this - 0x9c) >> 12);
	((void(__fastcall*)(void*, unsigned short)) 0x45ef40)(this, *(int*) ((char*) this - 0x98) >> 12);
	((void(__fastcall*)(void*, unsigned short)) 0x45ef40)(this, *(int*) ((char*) this - 0x94) >> 12);
	((void(__fastcall*)(void*, unsigned short)) 0x45ef40)(
		this,
		(unsigned short) (((*(unsigned short*) ((char*) this - 0x7c) & 7) << 3) |
						  (*(unsigned short*) ((char*) this - 0x84) & 7)));
	((void(__fastcall*)(void*, unsigned short)) 0x45ef40)(
		this,
		(unsigned short) ((*(unsigned char*) ((char*) this - 0xa0) << 8) | *(unsigned char*) ((char*) this - 0x80)));
	if (*(unsigned int*) ((char*) this - 0x5c) > (unsigned int) g_nLevelFrameClockTimeMs) {
		*(unsigned int*) ((char*) this - 0x5c) = g_nLevelFrameClockTimeMs;
	}
	((void(__fastcall*)(void*, unsigned int)) 0x45ef10)(this, *(unsigned int*) ((char*) this - 0x5c));
	*(unsigned int*) ((char*) this + 0x2c) = 0;
}

// FUNCTION: LEMBALL 0x00410100
void CPlayerLemming::StartStanding(void)
{
	void* pMoveChunk = 0;
	int nHeight = ((LevelTileGridOwnerView*) g_pLevelTileGrid)
						->GetZ(m_WorldPosition9C.x >> 12, m_WorldPosition9C.y >> 12, &pMoveChunk);
	int nCurrentHeight = m_WorldPosition9C.z >> 12;

	if (m_fOnMover11C == 0 && pMoveChunk != 0) {
		((void(__fastcall*)(void*, void*)) 0x4036b1)(pMoveChunk, this);
	}
	if (nCurrentHeight <= nHeight + 2) {
		if (pMoveChunk == 0) {
			m_WorldPosition9C.z = nHeight << 12;
		}
		((void(__fastcall*)(void*, void*, void*, unsigned short)) 0x40341d)(
			g_pActiveManagedEntityOwner, &m_WorldPosition9C, this, m_nBehaviourFlags68);
		return;
	}

	m_nNextUpdateTickCC = g_nLevelFrameClockTick;
	if ((m_nBehaviourFlags68 & 4) != 0) {
		m_nJumpVelY50 = 0;
		m_nJumpAirborne108 = 1;
		m_nJumpVelX4C = 0x3000;
		m_nJumpVelZ54 = ((nCurrentHeight - nHeight) / 8 + 1) * 0x1000;
		m_nActionPhaseBC = 0;
		m_nMotionStartTickC8 = g_nLevelFrameClockTick;
		m_nJumpElevation100 = nCurrentHeight;
		m_nJumpOriginXF4 = m_WorldPosition9C.x;
		m_nJumpOriginYF8 = m_WorldPosition9C.y;
		m_nJumpOriginZFC = nHeight << 12;
	}
}
