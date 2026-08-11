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
	unsigned short m_nObjectFlags04;   // 0x04; entity-type-dependent flag (0x200 type1 / 0x100 type2)
	char m_abReserved06[2];
	int m_nPendingInstruction08;
	void* m_pReachabilityOwner0C;
	int m_nRuntimeField10;             // 0x10; cleared on Initialise
	CGameObjectBounds m_bounds14;
	int m_nAirMotionState2C;           // 0x2C
	int m_fAirborne30;                 // 0x30; airborne/jump-in-progress flag
	int m_nAirMotionField34;           // 0x34
	int m_nAirMotionField38;           // 0x38
	int m_nAirMotionField3C;           // 0x3C
	AICOORD m_InitialPosition40;
	int m_nJumpVelX4C;                 // 0x4C; jump velocity vector (init sentinel 0xaa55aa55)
	int m_nJumpVelY50;                 // 0x50
	int m_nJumpVelZ54;                 // 0x54
	int m_nJumpAccel58;                // 0x58
	CGameObject* m_pTarget5C;
	void* m_pOwningChunkStream60;
	int m_nEntityType64;
	unsigned short m_nBehaviourFlags68;  // 0x68; behaviour flags (bit2 = can jump)
	unsigned short m_nRegistryIndex6A;
	unsigned short m_nSlotId6C;
	unsigned short m_nRuntimeField6E;  // 0x6E
	CGameObjectCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	int m_nRectMinX78;                 // 0x78; entity screen rect (set via set_rect_from_bounds)
	int m_nRectMinY7C;                 // 0x7C
	int m_nRectMaxX80;                 // 0x80
	int m_nRectMaxY84;                 // 0x84
	int m_nMotionDuration88;
	int m_nRuntimeState8C;
	char m_abReserved90[4];
	int m_nFrameTime94;
	int m_nRuntimeField98;             // 0x98; cleared on Initialise
	AICOORD m_WorldPosition9C;
	int m_nDestWorldXA8;               // 0xA8; current move destination (from GetDestination)
	int m_nDestWorldYAC;               // 0xAC
	int m_nDestWorldZB0;               // 0xB0
	short m_nHeadingOctantB4;
	char m_abReservedB6[2];
	int m_nStateB8;
	short m_nActionPhaseBC;
	char m_abReservedBE[2];
	int m_nRuntimeFieldC0;             // 0xC0; cleared on Initialise
	unsigned short m_nRuntimeFieldC4;  // 0xC4; cleared on Initialise
	char m_abReservedC6[2];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;
	int m_nSecondaryUpdateTickD0;
	char m_abReservedD4[4];
	int m_nJumpFieldD8;                // 0xD8 (init sentinel)
	int m_nJumpFieldDC;                // 0xDC (init sentinel)
	int m_nJumpFieldE0;                // 0xE0 (init sentinel)
	int m_nJumpFieldE4;                // 0xE4; cleared on Restart (init sentinel)
	int m_nJumpFieldE8;                // 0xE8; cleared on Restart (init sentinel)
	int m_nJumpFieldEC;                // 0xEC; cleared on Restart (init sentinel)
	int m_nJumpFieldF0;                // 0xF0; cleared on Initialise
	int m_nJumpOriginXF4;              // 0xF4; jump origin position
	int m_nJumpOriginYF8;              // 0xF8
	int m_nJumpOriginZFC;              // 0xFC
	int m_nJumpElevation100;           // 0x100; jump initial height term
	int m_nJumpField104;               // 0x104; cleared on landing
	int m_nJumpAirborne108;            // 0x108; =1 while airborne
	int m_nJumpField10C;               // 0x10C; cleared on Initialise
	int m_nRuntimeRestartField110;     // 0x110; set to 0xffff on Restart
	int m_nPendingState114;
	char m_abReserved118[4];
	int m_fOnMover11C;
	unsigned short m_nRestartValue120;
	char m_abReserved122[2];

	void Restart(void);
	void Initialise(void);
	void StartMoving(void);
	void StopMoving(void);
	void StartLand(void);
	void Jump(void);
	int MapCheck(int nX, int nY);
	void TurnToFaceDestination(void);
	int FacingDestination(void);
	void DeleteFirstEntryFromDestinationList(void);
	void AddDestination(const AICOORD& position);
	void AlterDestination(const AICOORD& position);
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
#ifdef LEMBALL_DECLARE_CGAMEOBJECT_MOVE
	int Move(void);
#endif
};

typedef char CGameObjectSizeMustMatchLayout[sizeof(CGameObject) == 0x124 ? 1 : -1];

#endif
