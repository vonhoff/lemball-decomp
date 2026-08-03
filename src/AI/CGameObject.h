#ifndef LEMBALL_CGAMEOBJECT_H
#define LEMBALL_CGAMEOBJECT_H

#include "AI/AICoord.h"

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

class CGameObject {
public:
	char m_abReserved00[0x64];
	int m_nEntityType64;
	char m_abReserved68[4];
	unsigned short m_nSlotId6C;
	char m_abReserved6E[2];
	CGameObjectCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	char m_abReserved78[0x10];
	int m_nMotionDuration88;
	char m_abReserved8C[0x10];
	AICOORD m_WorldPosition9C;
	char m_abReservedA8[0xc];
	short m_nHeadingOctantB4;
	char m_abReservedB6[2];
	int m_nStateB8;
	char m_abReservedBC[0xc];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;
	char m_abReservedD0[0x4c];
	int m_fOnMover11C;

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
	unsigned short GetId(void);
	void SetId(unsigned short nSlotId);
	void ReSetId(void);
	static unsigned short NextId(void);
	static unsigned short NextLoadingId(void);
};

#endif
