#ifndef LEMBALL_CLIFT_H
#define LEMBALL_CLIFT_H

#include "AI/CInvisibleSwitch.h"

typedef int eLiftActivateType;

class CLift {
public:
	CLift(void);
	~CLift(void);
	void CalculateCliff(void);
	int Process(void);
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
	unsigned char m_abReserved00[0x190];
};

struct LiftVtableLayout {
	void* m_apSlots00[5];
	int (CLift::*m_pProcess14)(void);
	void* m_apSlots18[10];
};

#endif
