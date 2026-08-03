#ifndef LEMBALL_CLASER_H
#define LEMBALL_CLASER_H

#include "AI/AICoord.h"

class CLaser {
public:
	CLaser(void);
	void Restart(void);
	void Initialise(void);
	~CLaser(void);
	void Set(unsigned short nSlotId, const AICOORD& position, int nObjectType);
	int CheckHits(void);
	int Process(void);
};

#endif
