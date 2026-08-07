#ifndef LEMBALL_CGAMEOBJECT_H
#define LEMBALL_CGAMEOBJECT_H

#include "AI/AICoord.h"

class CAI;
struct tCoord3d;

#ifndef LEMBALL_CVSRECT_DEFINED
#define LEMBALL_CVSRECT_DEFINED
struct CVSRect {
	short width;
	short height;
	short x;
	short y;
};
#endif

struct CGameObjectCommand {
	int m_nType;
	AICOORD m_Position;
	unsigned short m_nFlags;
	unsigned short m_nReserved;
};

struct CGameObjectCommandQueue {
	unsigned short m_cEntries;
	unsigned short m_cCapacity;
	CGameObjectCommand* m_pEntries;
};

struct CGameObjectBounds {
	int m_nMinX;
	int m_nMinY;
	int m_nMinZ;
	int m_nMaxX;
	int m_nMaxY;
	int m_nMaxZ;
};

class CGameObject {
public:
	void* m_pVtable00;
	char m_abReserved04[4];
	int m_nPendingInstruction08;
	void* m_pReachabilityOwner0C;
	char m_abReserved10[4];
	CGameObjectBounds m_bounds14;
	char m_abReserved2C[0x14];
	AICOORD m_InitialPosition40;
	char m_abReserved4C[0x10];
	CGameObject* m_pTarget5C;
	void* m_pOwningChunkStream60;
	int m_nEntityType64;
	unsigned char m_abReserved68[2];
	unsigned short m_nRegistryIndex6A;
	unsigned short m_nSlotId6C;
	char m_abReserved6E[2];
	CGameObjectCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	char m_abReserved78[0x10];
	int m_nMotionDuration88;
	int m_nRuntimeState8C;
	char m_abReserved90[4];
	int m_nFrameTime94;
	char m_abReserved98[4];
	AICOORD m_WorldPosition9C;
	char m_abReservedA8[0xc];
	short m_nHeadingOctantB4;
	char m_abReservedB6[2];
	int m_nStateB8;
	short m_nActionPhaseBC;
	char m_abReservedBE[0xa];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;
	int m_nSecondaryUpdateTickD0;
	char m_abReservedD4[0x40];
	int m_nPendingState114;
	char m_abReserved118[4];
	int m_fOnMover11C;
	unsigned short m_nRestartValue120;
	char m_abReserved122[2];

	void Restart(void);
	void Initialise(void);
	void StartMoving(void);
	void StopMoving(void);
	int MapCheck(int nX, int nY);
	void TurnToFaceDestination(void);
	int FacingDestination(void);
	void DeleteFirstEntryFromDestinationList(void);
	void AddDestination(const AICOORD& position);
	AICOORD* GetDestination(AICOORD* position);
	int DestinationExists(void);
	void EmptyDestinationList(void);
	void GetBoundingBox(CVSRect& rect);
	int IsSelectable(void);
	void ResetInstructions(void);
	void StartSommersault(void);
	int OnLift(tCoord3d& point);
	void OffLift(tCoord3d& point);
	int OnLift(tCoord3d& start, tCoord3d& end);
	void OffLift(tCoord3d& start, tCoord3d& end);
	static void Init(CAI* pAI);
	unsigned short GetId(void);
	void SetId(unsigned short nSlotId);
	void ReSetId(void);
	static unsigned short NextId(void);
	static unsigned short NextLoadingId(void);
	void RegisterId(void);
	void UpdateCollision(void);
	void DestroyLevelChunkObjectBase(void);
	void InitializeLevelChunkObjectBase(int nEntityType, unsigned short nReserved68, unsigned short nCommandCapacity);

	CGameObject* ConstructCGameObject(int nEntityType, unsigned short nReserved68, unsigned short nCommandCapacity);
};

typedef char CGameObjectSizeMustMatchLayout[sizeof(CGameObject) == 0x124 ? 1 : -1];

#endif
