#include "AI/CAI.h"

#include "AI/CBulletManager.h"
#include "AI/CDoorManager.h"
#include "AI/CMoverManager.h"
#include "AI/CPlayerLemmingGroupManager.h"

extern void* g_pLevelDemoPlaybackController;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pActiveNetworkRuntimeWindow;
extern void __fastcall AppendType18ChunkObject(void* pStream,
											   int nUnused,
											   unsigned short param_1,
											   void* param_2,
											   int param_3,
											   int param_4);
extern "C" unsigned long __stdcall timeGetTime();

// FUNCTION: LEMBALL 0x00412600
void CAI::FireBullet(unsigned short nSlotId,
					 int nBulletType,
					 int nOwner,
					 int nDirection,
					 AICOORD source,
					 AICOORD target)
{
	((CBulletManager*) m_pProjectileMgr168)->RequestBullet(nSlotId, nBulletType, nOwner, nDirection, source, target);
}

// FUNCTION: LEMBALL 0x00412eb0
void CAI::AddNewTrapDoor(int nX, int nY, int nZ, unsigned long nParam4)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	int nXFixed = nX << 12;
	int nYFixed = nY << 12;
	int nZFixed = nZ << 12;
	AppendType18ChunkObject(m_pChunkStreamMgr1C4, 0, (unsigned short) nSlot, &nXFixed, 0, nParam4);
}

// FUNCTION: LEMBALL 0x004125c0
void CAI::HitTrampoline(const AICOORD& position, CGameObject* pGameObject)
{
	((CTrampolineManager*) m_pObjectMgr1B4)->Hit((void*) &position, pGameObject);
}

// FUNCTION: LEMBALL 0x004130d0
void CAI::FindMoverHeight(int nX, int nY, int& nHeight)
{
	((CMoverManager*) m_pMoveMgr1C0)->Find(nX, nY, &nHeight);
}

// FUNCTION: LEMBALL 0x004127e0
void CAI::BulletCheckGroupIntersection(CVSRect* pRect, AICOORD* pOutput)
{
	((CBulletManager*) m_pProjectileMgr168)
		->FindProjectilePositionInRect((const LevelShortRect*) pRect, (LevelProjectilePosition*) pOutput);
}

// FUNCTION: LEMBALL 0x00411b10
void CAI::Start(void)
{
	if (m_nSession64 != 0) {
		m_nFlags6C = 1;
		m_nFlags70 = 0;
		void* pRuntime = *(void**) 0x4a0120;
		*(int*) ((char*) pRuntime + 0x48) = 3;
		*(int*) ((char*) pRuntime + 0x4c) = 0;
		return;
	}
	if (g_pLevelDemoPlaybackController != 0 && *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) != 0) {
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x38) = (int) timeGetTime();
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x3c) = 0;
	}
	((void(__fastcall*)(void*, int)) 0x4013ed)(this, 2);
	m_nFlags68 = 1;
}

// FUNCTION: LEMBALL 0x004131e0
void CAI::SetNetworkTrapDoors(int nProfile, int nMinX, int nMinY, int nMaxX, int nMaxY)
{
	m_nProfileD0 = nProfile;
	if (nMinX == -1) {
		m_boundsC0 = *(int*) (0x49cf50 + (nProfile << 4));
		m_boundsC4 = *(int*) (0x49cf54 + (nProfile << 4));
		m_boundsC8 = *(int*) (0x49cf58 + (nProfile << 4));
		m_boundsCC = *(int*) (0x49cf5c + (nProfile << 4));
	} else {
		m_boundsC0 = nMinX;
		m_boundsC4 = nMinY;
		m_boundsC8 = nMaxX;
		m_boundsCC = nMaxY;
	}
	if (nProfile > 0) {
		int* pi = m_anTrapMarkers90;
		int v = 0;
		int count = nProfile;
		do {
			if (pi[0] > 0x400 || pi[0] < 0) {
				pi[0] = v;
			}
			if (pi[4] > 0x400 || pi[4] < 0) {
				pi[4] = v;
			}
			pi++;
			v += 0x10;
		} while (--count != 0);
	}
}

// FUNCTION: LEMBALL 0x00412660
int CAI::ProcessMsg(tagMESSAGE* pMsg)
{
	short* raw = (short*) pMsg;
	if (raw[0] == 4) {
		((CPlayerLemmingGroupManager*) m_pMsgTarget15C)->PlayerGroupRequestFire(*(int*) (raw + 4), *(int*) (raw + 6));
		return 0;
	}
	if (m_nMode58 == 0) {
		return 1;
	}
	switch (raw[0]) {
	case 2:
		((CPlayerLemmingGroupManager*) m_pMsgTarget15C)
			->AddNewWaypointToCurrentGroup(*(int*) (raw + 4), *(int*) (raw + 6));
		return 0;
	case 3:
		((CPlayerLemmingGroupManager*) m_pMsgTarget15C)->RemoveWaypointsFromCurrentGroup();
		return 0;
	case 6:
		((CPlayerLemmingGroupManager*) m_pMsgTarget15C)->MakeNextGroupPlayerControlled();
		return 0;
	case 7:
		((CPlayerLemmingGroupManager*) m_pMsgTarget15C)->MakeNextGroupPlayerControlled();
		return 0;
	case 8:
		((CPlayerLemmingGroupManager*) m_pMsgTarget15C)->UseObject(*(int*) (raw + 4));
		return 0;
	case 5:
		(*(void (**)(void*, int, int))(*(void***) m_pMsgTarget15C +
									   0x34 / 4))(m_pMsgTarget15C, *(int*) (raw + 4), *(int*) (raw + 6));
		return 0;
	default:
		m_nMsgCounter0C++;
		return 0;
	}
}

// FUNCTION: LEMBALL 0x00411b70
void CAI::SendGameState(int nState, int nStage)
{
	if (g_nSelectedNetworkLobbyPeerId == 0) {
		return;
	}
	if (*(int*) (*(int*) ((char*) m_pRuntime74 + 0x28)) != 0) {
		unsigned long start = timeGetTime();
		while (*(int*) (*(int*) ((char*) m_pRuntime74 + 0x28)) != 0 &&
		       timeGetTime() - start < 2000) {
			(*(void(**)(void*)) (*(int*) *(int*) g_pActiveNetworkRuntimeWindow + 0x30))(*(void**) g_pActiveNetworkRuntimeWindow);
		}
	}
	if (*(int*) (*(int*) ((char*) m_pRuntime74 + 0x28)) == 0) {
		m_nFlags6C = 1;
		*(int*) (*(int*) ((char*) m_pRuntime74 + 0x2c)) = nState;
		*(int*) (*(int*) ((char*) m_pRuntime74 + 0x30)) = nStage;
		*(int*) (*(int*) ((char*) m_pRuntime74 + 0x34)) = m_nTimeScoreE8;
		*(int*) (*(int*) ((char*) m_pRuntime74 + 0x38)) = m_nTimeScoreF0;
		((void(__fastcall*)(void*, int)) 0x45f2b0)(m_pRuntime74, g_nSelectedNetworkLobbyPeerId);
	}
}

// FUNCTION: LEMBALL 0x00412ad0
int CAI::OpenDoor(const AICOORD& position, CGameObject* pGameObject, unsigned short nFlags)
{
	int tileX = ((position.x >> 12) + ((position.x >> 12) >> 31 & 0xf)) >> 4;
	int tileY = ((position.y >> 12) + ((position.y >> 12) >> 31 & 0xf)) >> 4;
	unsigned short tile = 0x3;
	if (tileX >= 0 && tileY >= 0) {
		void* grid = m_pTileGrid110;
		if (tileX < *(int*) ((char*) grid + 0x10) && tileY < *(int*) ((char*) grid + 0x14)) {
			tile = *(unsigned short*) (*(int*) ((char*) grid + 0xc) + 6 +
									   (*(int*) ((char*) grid + 0x10) * tileY + tileX) * 0xc);
		}
	}
	if ((tile & 0x8000) && (nFlags & 0x20)) {
		return ((CDoorManager*) m_pDoorMgr190)->Open((void*) &position, pGameObject);
	}
	return 0;
}
