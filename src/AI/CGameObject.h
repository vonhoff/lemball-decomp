#ifndef LEMBALL_CGAMEOBJECT_H
#define LEMBALL_CGAMEOBJECT_H

#include "AI/AICoord.h"

class CAI;
struct tCoord3d;

struct CVSRect {
	short width;
	short height;
	short x;
	short y;
};

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
	char m_abReserved2C[0x38];
	int m_nEntityType64;
	char m_abReserved68[4];
	unsigned short m_nSlotId6C;
	char m_abReserved6E[2];
	CGameObjectCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	char m_abReserved78[0x10];
	int m_nMotionDuration88;
	int m_nRuntimeState8C;
	char m_abReserved90[0xc];
	AICOORD m_WorldPosition9C;
	char m_abReservedA8[0xc];
	short m_nHeadingOctantB4;
	char m_abReservedB6[2];
	int m_nStateB8;
	short m_nActionPhaseBC;
	char m_abReservedBE[0xa];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;
	char m_abReservedD0[0x44];
	int m_nPendingState114;
	char m_abReserved118[4];
	int m_fOnMover11C;
	unsigned short m_nRestartValue120;
	char m_abReserved122[2];

	void Restart(void);
	void Initialise(void);
	void StartMoving(void);
	void StopMoving(void);
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
};

#endif
