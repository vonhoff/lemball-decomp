#ifndef LEMBALL_AI_BASE_CMOVEMENTINTERPOLATION_H
#define LEMBALL_AI_BASE_CMOVEMENTINTERPOLATION_H

#include "../../Visos/Foundation/CVector.h"

class CMovementInterpolation {
public:
	void SetEndpoints(CVector p_start, CVector p_end);
	CMovementInterpolation& operator=(const CMovementInterpolation& p_other);

private:
	int m_startX; // 0x00
	int m_startY; // 0x04
	int m_deltaX; // 0x08
	int m_deltaY; // 0x0c
};

#endif
