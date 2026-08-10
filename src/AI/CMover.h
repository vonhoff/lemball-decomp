#ifndef LEMBALL_CMOVER_H
#define LEMBALL_CMOVER_H

#include "AI/AICoord.h"
#include "AI/CMove3d.h"

class CGameObject;

struct ManagedEntitySlotOwnerView {
	char m_abReserved00[0x6c];
	unsigned short m_nSlotId6C;
};

struct CMover : public ManagedEntitySlotOwnerView {
public:
	CMover(void);
	~CMover(void);
	void Restart(void);
	void Initialise(void);
	void SetPos(void);
	void SetUpNextNode(unsigned long nTime);
	void MoveObjects(int nDeltaX, int nDeltaY, int nDeltaZ);
	void Switch(void);
	int IsOn(AICOORD& point);
	int GetOn(CGameObject* pObject);
	void VerifyObjects(void);
	void StopObjectsMoving(void);
	int IsAt(int x, int y, int* pHeight);
	void Set(unsigned short nSlotId, short nHeading, int nMode, int iNode, int nValue);
	void FindObjectsOnTopOfMe(void);
	CMover& CopyPrefix(const CMover& source);

private:
	char m_abReserved6E[0x26];
	int m_nField94;                   // 0x94; cleared on Initialise
	char m_abReserved98[4];
	int m_nWorldX9C;
	int m_nWorldYA0;
	int m_nWorldZA4;
	char m_abReservedA8[0x10];
	int m_nFieldB8;                   // 0xB8; set to 0x18 on Initialise
	short m_nHeadingBC;
	char m_abReservedBE[0xa];
	int m_nNodeStartTickC8;    // 0xc8
	int m_nNodeEndTickCC;      // 0xcc
	char m_abReservedD0[0x68];
	int m_fActive138;
	int m_nField13C;                  // 0x13C; cleared on Initialise
	int m_nField140;                  // 0x140; set to 1 on Initialise
	int m_nMode144;
	int m_fSwitch148;
	CMove3d m_Move14C;
	int m_iNode168;
	int m_nValue16C;
	int m_nState170;
	int m_cAttachedEntities174;
	void* m_apAttachedEntities178[10];
};

typedef char CMoverSizeCheck[sizeof(CMover) == 0x1a0 ? 1 : -1];

#endif
