#ifndef LEMBALL_CAI_H
#define LEMBALL_CAI_H

#include "AI/CGameObject.h"

struct tagMESSAGE;

class CAI {
public:
	void* m_vtable00;                 // vtable pointer (slot 0 = vtbl)
	char m_abReserved04[0x8];
	int m_nMsgCounter0C;
	char m_abReserved10[0x24];        // render-queue-node + eff-stream base
	int m_nModeCounter34;             // 0x34; incremented by 0x60 on initialise
	char m_abReserved38[0x14];
	int m_nInitialized4C;             // 0x4C; 1 once constructed/initialised
	void* m_pField50;                 // 0x50; ptr to allocated 0xdc byte block
	unsigned short m_nStep54;         // 0x54; slot-assign step (set 10)
	char m_abReserved56[2];
	int m_nMode58;
	int m_nMode5C;                    // 0x5C
	int m_nMode60;                    // 0x60
	int m_nSession64;
	int m_nFlags68;
	int m_nFlags6C;
	int m_nFlags70;
	void* m_pRuntime74;
	void* m_pNetDeltaStream78;        // 0x78; network level-chunk delta stream
	int m_nField7C;                   // 0x7C
	char m_abReserved80[0x10];
	int m_anTrapMarkers90[12];
	int m_boundsC0;
	int m_boundsC4;
	int m_boundsC8;
	int m_boundsCC;
	int m_nProfileD0;
	int m_nEntityCountD4;             // 0xD4
	int m_nFieldD8;                   // 0xD8
	int m_nFieldDC;                   // 0xDC
	int m_nFieldE0;                   // 0xE0
	int m_nFieldE4;                   // 0xE4
	int m_nTimeScoreE8;
	int m_nTimerEC;                   // 0xEC
	int m_nTimeScoreF0;
	int m_nFieldF4;                   // 0xF4
	int m_nFieldF8;                   // 0xF8
	int m_nFieldFC;                   // 0xFC
	int m_nField100;                  // 0x100
	char m_abReserved104[4];
	int m_nField108;                  // 0x108
	int m_nField10C;                  // 0x10C
	void* m_pTileGrid110;
	void* m_pTileReachabilityHelper114;  // 0x114
	int m_nListCount118;              // 0x118
	int m_nListCap11C;                // 0x11C (=100)
	void* m_pList120;                 // 0x120; ptr to 100-entry block
	int m_nField124;                  // 0x124
	int m_nField128;                  // 0x128
	int m_nField12C;                  // 0x12C
	char m_abReserved130[4];
	int m_nField134;                  // 0x134
	int m_nField138;                  // 0x138
	int m_nField13C;                  // 0x13C
	int m_nField140;                  // 0x140 (0xffffffff sentinel)
	int m_nField144;                  // 0x144 (0xffffffff sentinel)
	int m_nField148;                  // 0x148 (0xffffffff sentinel)
	char m_abReserved14C[8];
	int m_nField154;                  // 0x154; param_1 (owns vm-block stream)
	char m_abReserved158[4];
	void* m_pMsgTarget15C;
	void* m_pEntityQueueCursor160;    // 0x160
	void* m_pRenderDispatchQueue164;  // 0x164
	void* m_pProjectileMgr168;
	void* m_pProjectileGeometryTable16C;  // 0x16C
	void* m_pShpgMgr170;              // 0x170
	void* m_pEnmyMgr174;              // 0x174
	void* m_pLevelChunkLoaderCtx178;  // 0x178
	void* m_pNodeMgr17C;              // 0x17C
	void* m_pBallMgr180;              // 0x180
	void* m_pCollMgr184;              // 0x184
	void* m_pMineMgr188;              // 0x188
	void* m_pLiftMgr18C;              // 0x18C
	void* m_pDoorMgr190;
	void* m_pTimerBlock194;           // 0x194
	void* m_pRockMgr198;              // 0x198
	char m_abReserved19C[4];
	void* m_pLasrMgr1A0;              // 0x1A0
	void* m_pCaptureTriggerMgr1A4;    // 0x1A4
	void* m_pBoonMgr1A8;              // 0x1A8
	void* m_pIceMgr1AC;               // 0x1AC
	void* m_pPgunMgr1B0;              // 0x1B0
	void* m_pObjectMgr1B4;
	char m_abReserved1B8[0x8];
	void* m_pMoveMgr1C0;
	void* m_pChunkStreamMgr1C4;

	void AddNewTrapDoor(int nX, int nY, int nZ, unsigned long nParam4);
	void HitTrampoline(const AICOORD& position, CGameObject* pGameObject);
	void FindMoverHeight(int nX, int nY, int& nHeight);
	void BulletCheckGroupIntersection(CVSRect* pRect, AICOORD* pOutput);
	void FireBullet(unsigned short nSlotId, int nBulletType, int nOwner, int nDirection,
		AICOORD source, AICOORD target);
	void Start(void);
	void SetNetworkTrapDoors(int nProfile, int nMinX, int nMinY, int nMaxX, int nMaxY);
	int ProcessMsg(tagMESSAGE* pMsg);
	void SendGameState(int nState, int nStage);
	int OpenDoor(const AICOORD& position, CGameObject* pGameObject, unsigned short nFlags);
};

#endif // LEMBALL_CAI_H
