#ifndef LEMBALL_AI_BASE_CMOVE3D_H
#define LEMBALL_AI_BASE_CMOVE3D_H

#include "AiCoord.h"
#include "CPt3.h"

// SIZE 0x1c
class CMove3d {
public:
	inline CMove3d() : m_start(0, 0, 0) {}
	void Position(CPt3& p_position, int p_time);
	void Set(CPt3& p_start, CPt3& p_end, int p_startTime, int p_speed);

private:
	int m_startTime;    // 0x00
	CPt3 m_start;       // 0x04
	AiCoord m_velocity; // 0x10
};

#endif
