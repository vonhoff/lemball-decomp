#ifndef LEMBALL_AI_CBALL_H
#define LEMBALL_AI_CBALL_H

#include "AI/AICoord.h"

// Windows physical size 0x13c (see CBallManager::Initialise allocation).
class CBall {
public:
	char m_abReserved00[0x40];
	int m_nSavedPosX40;
	int m_nSavedPosY44;
	int m_nSavedPosZ48;
	char m_abReserved4C[0x18];
	int m_nObjectType64;
	char m_abReserved68[0x34];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;
	char m_abReservedA8[0x10];
	int m_nStateB8;
	short m_wDirectionBC;
	char m_abReservedBE[0x66];
	int m_nTargetX124;
	int m_nTargetY128;
	int m_nTargetZ12C;
	short m_wSpeed130;
	char m_abReserved132[2];
	int m_nActive134;
	char m_abReserved138[4];

	void Set(AICOORD oCurrent, AICOORD oTarget, int nSpeed);
	void SetHeightCorrect(void);
#ifdef LEMBALL_CBALL_HARDTAIL_METHODS
	void StartMovement(int nUseTarget);
	int Move(void);
	int Process(void);
	void LoadLevel(void* pStreamCursor);
#endif
};

#endif
