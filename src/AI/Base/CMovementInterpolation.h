#ifndef LEMBALL_AI_BASE_CMOVEMENTINTERPOLATION_H
#define LEMBALL_AI_BASE_CMOVEMENTINTERPOLATION_H

#include "Visos/Math/CVector.h"
#include "Visos/Diagnostics/VsDebug.h"

class CMovementInterpolation {
public:
	CMovementInterpolation() : m_start(DEBUG_SENTINEL, DEBUG_SENTINEL), m_delta(DEBUG_SENTINEL, DEBUG_SENTINEL) {}

	void SetEndpoints(CVector p_start, CVector p_end);
	// FUNCTION: LEMBALL 0x004267a0
	CMovementInterpolation& operator=(const CMovementInterpolation& p_other)
	{
		m_start.m_xFixed = p_other.m_start.m_xFixed;
		m_start.m_yFixed = p_other.m_start.m_yFixed;
		m_delta.m_xFixed = p_other.m_delta.m_xFixed;
		m_delta.m_yFixed = p_other.m_delta.m_yFixed;
		return *this;
	}

private:
	friend class CGameObject;
	friend class CBall;

	CVector m_start; // 0x00
	CVector m_delta; // 0x08
};

#endif
