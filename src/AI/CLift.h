#ifndef LEMBALL_CLIFT_H
#define LEMBALL_CLIFT_H

#include "AI/CInvisibleSwitch.h"

typedef int eLiftActivateType;

class CGameObject;

class CLift {
public:
	CLift(void);
	~CLift(void);
	void CalculateCliff(void);
	int Process(void);
	void CheckObjects(void);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	int Activate(void);
	void ActivateDeactivate(void);
	void Edit(int nHeight,
			  short nDirection,
			  int nTargetA,
			  int nTargetB,
			  eLiftActivateType activateType,
			  unsigned char nFlags);
	void Set(int x,
			 int y,
			 int z,
			 short nDirection,
			 int nTargetA,
			 int nTargetB,
			 eLiftActivateType activateType,
			 unsigned char nFlags);
	void Set(tCoord3d& start,
			 tCoord3d& end,
			 short nDirection,
			 int nTargetA,
			 int nTargetB,
			 eLiftActivateType activateType,
			 unsigned char nFlags);

private:
	unsigned char m_abReserved00[0x9c];
	AICOORD m_WorldPosition9C;            // 0x9c
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                        // 0xb8
	unsigned char m_abReservedBC[0x7c];
	unsigned short m_nLiftObjectId138;     // 0x138
	tCoord3d m_RangeStart13A;              // 0x13a (x,y,z)
	tCoord3d m_RangeEnd140;                // 0x140 (x,y,z)
	unsigned char m_abReserved146[0x4a];
};

typedef char CLiftSizeCheck[sizeof(CLift) == 0x190 ? 1 : -1];

struct LiftVtableLayout {
	void* m_apSlots00[5];
	int (CLift::*m_pProcess14)(void);
	void* m_apSlots18[10];
};

#endif
