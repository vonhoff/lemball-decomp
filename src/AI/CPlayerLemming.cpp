#define LEMBALL_CPLAYERLEMMING_START_STANDING
#include "AI/CPlayerLemming.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
typedef unsigned long DWORD;
#include "Platform/Windows/Mixed/Engine/MEDIA/VSSTRM.H"

extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pLevelDemoPlaybackController;
extern void* g_pLevelTileGrid;
extern int g_cActiveManagedEntities;
extern void RequestLocalLevelGameStateChange(void* pLevelMode, int nState);
extern unsigned int __cdecl ReturnFacingDirection(int nX1, int nY1, int nX2, int nY2);

struct LevelTileGridOwnerView {
	unsigned short GetZ(int x, int y, void** ppMoveChunk);
};

struct LevelManagedEntityStateFields {
	void IncrementHeadingOctant(void);
	void DecrementHeadingOctant(void);
};

struct CMover {
	int GetOn(CGameObject* pObject);
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
		int nObjectCount = m_nObjectCount220;
		if (nObjectCount != 0xc && nObjectCount > 0) {
			int* pObjectType = m_anObjectTypes1C0;
			int iObject = 0;
			do {
				if (*pObjectType == nObjectType) {
					return 1;
				}
				pObjectType++;
				iObject++;
			} while (iObject < nObjectCount);
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
	if (nOnConveyor == 0 && m_fOnIce188 != 0 && nDetach != 0) {
		((void(__fastcall*)(void*, void*)) 0x402c16)(m_pIce18C, this);
	}
	m_fOnIce188 = nOnConveyor;
	m_pIce18C = pIce;
	if (nOnConveyor != 0) {
		int nGroupLemmingCount = ((int(__fastcall*)(void*))(*(void***) m_pGroup224 + 0x108 / 4))(m_pGroup224);
		if (nGroupLemmingCount > 1) {
			(*(void (**)(int, void*))(*(void***) *(void**) ((char*) g_pActiveManagedEntityOwner + 0x15c) +
									  0x34 / 4))(1, &m_nRegistryIndex6A);
		}
		(*(void (**)(void*))(*(void***) m_pGroup224 + 0x150 / 4))(m_pGroup224);
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
	unsigned int uDir = ReturnFacingDirection(m_WorldPosition9C.x >> 12,
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
	int nCursorX;
	int nCursorY;
	((void(__fastcall*)(void*, void*, void*)) 0x401e65)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160),
														&nCursorX,
														&nCursorY);
	unsigned int nTargetOctant = ReturnFacingDirection(m_WorldPosition9C.x >> 12,
																						 m_WorldPosition9C.y >> 12,
																						 nCursorX,
																						 nCursorY);
	return (int) m_nHeadingOctantB4 - (int) nTargetOctant == 1;
}
// FUNCTION: LEMBALL 0x0040f220
void CPlayerLemming::TurnToFaceTarget(void)
{
	int nOct = ReturnFacingDirection(m_WorldPosition9C.x >> 12,
															  m_WorldPosition9C.y >> 12,
															  m_nTargetWorldX1B4 >> 12,
															  m_nTargetWorldY1B8 >> 12);
	if (nOct != (int) m_nHeadingOctantB4) {
		if (*(int*) ((char*) 0x49d020 + (nOct - (int) m_nHeadingOctantB4 & 7) * 4) < 0) {
			((LevelManagedEntityStateFields*) this)->DecrementHeadingOctant();
		}
		else {
			((LevelManagedEntityStateFields*) this)->IncrementHeadingOctant();
		}
		(*(void (**)(void*, int))(*(void***) this + 0x84 / 4))(this, 0xfa0);
	}
	m_nNextUpdateTickCC = g_nLevelFrameClockTick + *(int*) ((char*) 0x49d0b0 + m_nEntityType64 * 4) / 0x32;
}
// FUNCTION: LEMBALL 0x0040f160
void CPlayerLemming::TurnToFaceCursor(void)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) {
		int nCursorX = 0;
		int nCursorY = 0;
		((void(__fastcall*)(void*, int*, int*)) 0x401e65)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160),
														  &nCursorX,
														  &nCursorY);
		int nOct = ReturnFacingDirection(m_WorldPosition9C.x >> 12,
																  m_WorldPosition9C.y >> 12,
																  nCursorX,
																  nCursorY);
		if (nOct != (int) m_nHeadingOctantB4) {
			if (*(int*) ((char*) 0x49d020 + (nOct - (int) m_nHeadingOctantB4 & 7) * 4) < 0) {
				((LevelManagedEntityStateFields*) this)->DecrementHeadingOctant();
			}
			else {
				((LevelManagedEntityStateFields*) this)->IncrementHeadingOctant();
			}
			((void (**)(void*, int)) m_pVtable00)[0x84 / 4](this, 0xfa0);
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + *(int*) ((char*) 0x49d070 + m_nEntityType64 * 4) / 0x32;
	}
}
// FUNCTION: LEMBALL 0x0040f640
void CPlayerLemming::GetData(void)
{
	unsigned short aPackedStateWords[2];
	*(unsigned int*) ((char*) this - 0x9c) =
		(unsigned int) (unsigned short) ((CNetworkMessage*) this)->ReadEffStreamU16BEValue() << 12;
	*(unsigned int*) ((char*) this - 0x98) =
		(unsigned int) (unsigned short) ((CNetworkMessage*) this)->ReadEffStreamU16BEValue() << 12;
	*(unsigned int*) ((char*) this - 0x94) =
		(unsigned int) (unsigned short) ((CNetworkMessage*) this)->ReadEffStreamU16BEValue() << 12;
	((CNetworkMessage*) this)->ReadEffStreamU16BE((unsigned char*) aPackedStateWords);
	*(unsigned short*) ((char*) this - 0x84) = (unsigned short) (aPackedStateWords[0] & 7);
	*(unsigned short*) ((char*) this - 0x7c) = (unsigned short) ((aPackedStateWords[0] & 0x38) >> 3);
	((CNetworkMessage*) this)->ReadEffStreamU16BE((unsigned char*) aPackedStateWords);
	*(unsigned int*) ((char*) this - 0x80) = (unsigned int) (aPackedStateWords[0] & 0xff);
	*(unsigned int*) ((char*) this - 0xa0) = (unsigned int) (aPackedStateWords[0] >> 8);
	*(unsigned int*) ((char*) this - 0xa4) = ((CNetworkMessage*) this)->ReadEffStreamU32BEValue();
}
// FUNCTION: LEMBALL 0x0040f6f0
void CPlayerLemming::AddData(void)
{
	((CNetworkMessage*) this)->WriteEffStreamU16BE(0x2c);
	((CNetworkMessage*) this)->WriteEffStreamU8(*(unsigned short*) ((char*) this + 0x30));
	((CNetworkMessage*) this)->WriteEffStreamU16BE(*(int*) ((char*) this - 0x9c) >> 12);
	((CNetworkMessage*) this)->WriteEffStreamU16BE(*(int*) ((char*) this - 0x98) >> 12);
	((CNetworkMessage*) this)->WriteEffStreamU16BE(*(int*) ((char*) this - 0x94) >> 12);
	((CNetworkMessage*) this)
		->WriteEffStreamU16BE((unsigned short) (((*(unsigned short*) ((char*) this - 0x7c) & 7) << 3) |
												(*(unsigned short*) ((char*) this - 0x84) & 7)));
	((CNetworkMessage*) this)
		->WriteEffStreamU16BE((unsigned short) ((*(unsigned char*) ((char*) this - 0xa0) << 8) |
												*(unsigned char*) ((char*) this - 0x80)));
	if (*(unsigned int*) ((char*) this - 0x5c) > (unsigned int) g_nLevelFrameClockTimeMs) {
		*(unsigned int*) ((char*) this - 0x5c) = g_nLevelFrameClockTimeMs;
	}
	((CNetworkMessage*) this)->WriteEffStreamU32BE(*(unsigned int*) ((char*) this - 0x5c));
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
		((CMover*) pMoveChunk)->GetOn(this);
	}
	if (nCurrentHeight <= nHeight + 2) {
		if (pMoveChunk == 0) {
			m_WorldPosition9C.z = nHeight << 12;
		}
		((void(__fastcall*)(void*, void*, void*, unsigned short)) 0x40341d)(g_pActiveManagedEntityOwner,
																			&m_WorldPosition9C,
																			this,
																			m_nBehaviourFlags68);
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

extern int __fastcall GetLastQueuedProjectileRequestCode(void* pObject);
extern void __fastcall RemoveQueuedProjectileRequestCode(void* pObject, int nUnusedEdx, int nCode);
extern void* g_pLevelProgressState;

struct LevelVtSmallFunctionView {
	void AddLevelScoreClamped(int nValue);
};

// FUNCTION: LEMBALL 0x0040ff70
void CPlayerLemming::RequestBalloon(void)
{
	int aTargetPoint[3];
	int nRequestCode;

	aTargetPoint[0] = (int) 0xaa55aa55;
	aTargetPoint[1] = (int) 0xaa55aa55;
	aTargetPoint[2] = (int) 0xaa55aa55;
	nRequestCode = GetLastQueuedProjectileRequestCode(this);
	if (nRequestCode <= 0xffff) {
		if (nRequestCode == 0xffff) {
			m_fAirborne30 = 0;
			return;
		}
		switch (nRequestCode) {
		case 0x27:
			*(int*) ((char*) this + 0x16c) = 0x28;
			RemoveQueuedProjectileRequestCode(this, 0, 0x27);
			m_nActionPhaseBC = 3;
			break;
		case 0x29:
			*(int*) ((char*) this + 0x16c) = 0x2a;
			RemoveQueuedProjectileRequestCode(this, 0, 0x29);
			m_nActionPhaseBC = 1;
			break;
		case 0x2b:
			*(int*) ((char*) this + 0x16c) = 0x2c;
			RemoveQueuedProjectileRequestCode(this, 0, 0x2b);
			m_nActionPhaseBC = 4;
			break;
		case 0x2d:
			*(int*) ((char*) this + 0x16c) = 0x2e;
			RemoveQueuedProjectileRequestCode(this, 0, 0x2d);
			m_nActionPhaseBC = 0;
			break;
		}
	}
	m_fAirborne30 =
		((int(__fastcall*)(void*, int, int, int*)) 0x401983)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x1a8),
															 0,
															 *(int*) ((char*) this + 0x16c),
															 aTargetPoint);
	m_nMotionStartTickC8 = g_nLevelFrameClockTick;
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(10);
}

// FUNCTION: LEMBALL 0x0040f310
void CPlayerLemming::Fire(void)
{
	int aSourcePoint[3];
	int nDirection;
	int nState;

	aSourcePoint[0] = m_WorldPosition9C.x;
	aSourcePoint[1] = m_WorldPosition9C.y;
	aSourcePoint[2] = m_WorldPosition9C.z + 0xa000;
	nDirection = (int) m_nHeadingOctantB4;
	nState = m_nStateB8;
	switch (nState) {
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
	case 10:
	case 0x0b:
	case 0x0c:
	case 0x0e:
	case 0x0f:
		break;
	default:
		if (*(int*) ((char*) g_pLevelProgressState + 0x24) != 0 || m_nLatchObjectType228 != 0) {
			((void(__fastcall*)(void*, int, int))(*(void***) this)[0x84 / 4])(this, 0, 4000);
			((void(__fastcall*)(void*, int, int, int, int, int, int, int, int, int, int, int)) 0x402eff)(
				g_pActiveManagedEntityOwner,
				0,
				aSourcePoint[0],
				aSourcePoint[1],
				aSourcePoint[2],
				m_nTargetWorldX1B4,
				m_nTargetWorldY1B8,
				*(int*) ((char*) this + 0x1bc),
				(unsigned int) m_nSlotId6C,
				0,
				0,
				nDirection);
			m_nRuntimeField98 = 0x0d;
			if (*(int*) ((char*) g_pLevelProgressState + 0x24) == 0) {
				--m_nLatchObjectType228;
			}
		}
		break;
	}
	*(int*) ((char*) this + 0x184) = 0;
}

// FUNCTION: LEMBALL 0x0040fd10
void CPlayerLemming::OnBalloon(void)
{
	char* pGrid;
	int Target[3];
	int nDistance;
	int nHeight;
	int nCurrentZ;
	int nDelta;
	int nTileX;
	int nTileY;

	Target[0] = (int) 0xaa55aa55;
	Target[1] = (int) 0xaa55aa55;
	Target[2] = (int) 0xaa55aa55;
	((void(__fastcall*)(void*, int, int, int*)) 0x401983)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x1a8),
														  0,
														  *(int*) ((char*) this + 0x16c),
														  Target);
	nDistance = ((int(__cdecl*)(int, int, int, int)) 0x40254a)(m_WorldPosition9C.x >> 12,
															   m_WorldPosition9C.y >> 12,
															   Target[0] >> 12,
															   Target[1] >> 12);
	pGrid = (char*) g_pLevelTileGrid;
	if (nDistance < 0x10) {
		*(int*) ((char*) this + 0x30) = 0;
		((void(__fastcall*)(void*, int, int))(*(void***) this)[0x0d])(this, 0, 0x2b);
		*(int*) ((char*) this + 0x108) = 1;
		m_nActionPhaseBC = 0;
		m_nMotionStartTickC8 = g_nLevelFrameClockTick;
		m_nStateB8 = 0x0b;
		m_nJumpElevation100 = m_WorldPosition9C.z >> 12;
		((void(__fastcall*)(void*, int)) 0x402446)(this, 0);
		m_nJumpOriginXF4 = m_WorldPosition9C.x;
		m_nJumpOriginYF8 = m_WorldPosition9C.y;
		nTileX = (m_WorldPosition9C.x >> 12) >> 4;
		nTileY = (m_WorldPosition9C.y >> 12) >> 4;
		if ((m_WorldPosition9C.x >> 12) < 0 || (m_WorldPosition9C.y >> 12) < 0 || nTileX >= *(int*) (pGrid + 0x10) ||
			nTileY >= *(int*) (pGrid + 0x14)) {
			nHeight = 0;
		}
		else {
			nHeight = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
				*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0x0c,
				0,
				(m_WorldPosition9C.x >> 12) & 0xf,
				(m_WorldPosition9C.y >> 12) & 0xf);
		}
		m_nJumpOriginZFC = nHeight << 12;
		return;
	}
	nTileX = (m_WorldPosition9C.x >> 12) >> 4;
	nTileY = (m_WorldPosition9C.y >> 12) >> 4;
	if ((m_WorldPosition9C.x >> 12) < 0 || (m_WorldPosition9C.y >> 12) < 0 || nTileX >= *(int*) (pGrid + 0x10) ||
		nTileY >= *(int*) (pGrid + 0x14)) {
		nHeight = 0;
	}
	else {
		nHeight = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
			*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0x0c,
			0,
			(m_WorldPosition9C.x >> 12) & 0xf,
			(m_WorldPosition9C.y >> 12) & 0xf);
	}
	nHeight += 0x20;
	nCurrentZ = m_WorldPosition9C.z >> 12;
	nDelta = (g_nLevelFrameClockTick - m_nMotionStartTickC8) * 2;
	if (nDistance) {
		m_WorldPosition9C.x += (Target[0] - m_WorldPosition9C.x) * nDelta / nDistance;
		m_WorldPosition9C.y += (Target[1] - m_WorldPosition9C.y) * nDelta / nDistance;
	}
	if (nCurrentZ < nHeight - 6 || nCurrentZ > nHeight + 6) {
		if (nCurrentZ > nHeight) {
			m_WorldPosition9C.z -= 0x2000;
		}
		else {
			m_WorldPosition9C.z += 0x2000;
		}
	}
	m_nMotionStartTickC8 = g_nLevelFrameClockTick;
}
