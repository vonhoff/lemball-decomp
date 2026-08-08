#ifndef LEMBALL_CAI_H
#define LEMBALL_CAI_H

#include "AI/CGameObject.h"

struct tagMESSAGE;

class CAI {
public:
	void* m_vtable00;                 // vtable pointer (slot 0 = vtbl)
	char m_abReserved04[0x8];
	int m_nMsgCounter0C;
	char m_abReserved10[0x48];
	int m_nMode58;
	char m_abReserved5C[0x8];
	int m_nSession64;
	int m_nFlags68;
	int m_nFlags6C;
	int m_nFlags70;
	void* m_pRuntime74;
	char m_abReserved78[0x18];
	int m_anTrapMarkers90[12];
	int m_boundsC0;
	int m_boundsC4;
	int m_boundsC8;
	int m_boundsCC;
	int m_nProfileD0;
	char m_abReservedD4[0x14];
	int m_nTimeScoreE8;
	char m_abReservedEC[0x4];
	int m_nTimeScoreF0;
	char m_abReservedF4[0x1c];
	void* m_pTileGrid110;
	char m_abReserved114[0x48];
	void* m_pMsgTarget15C;
	char m_abReserved160[0x8];
	void* m_pProjectileMgr168;
	char m_abReserved16C[0x24];
	void* m_pDoorMgr190;
	char m_abReserved194[0x20];
	void* m_pObjectMgr1B4;
	char m_abReserved1B8[0x8];
	void* m_pMoveMgr1C0;
	void* m_pChunkStreamMgr1C4;

	void AddNewTrapDoor(int nX, int nY, int nZ, unsigned long nParam4);
	void HitTrampoline(const AICOORD& position, CGameObject* pGameObject);
	void FindMoverHeight(int nX, int nY, int& nHeight);
	void BulletCheckGroupIntersection(CVSRect* pRect, AICOORD* pOutput);
	void Start(void);
	void SetNetworkTrapDoors(int nProfile, int nMinX, int nMinY, int nMaxX, int nMaxY);
	int ProcessMsg(tagMESSAGE* pMsg);
	void SendGameState(int nState, int nStage);
	int OpenDoor(const AICOORD& position, CGameObject* pGameObject, unsigned short nFlags);
};

#endif // LEMBALL_CAI_H
