#ifndef LEMBALL_AI_CCATAPULT_H
#define LEMBALL_AI_CCATAPULT_H

class CCatapult {
public:
	char m_abReserved00[0x5c];
	void* m_pTarget5C;
	char m_abReserved60[0x34];
	int m_nFrameTime94;
	char m_abReserved98[4];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;
	char m_abReservedA8[0x10];
	int m_nStateB8;
	short m_nVariantBC;
	char m_abReservedBE[0x0e];
	int m_nPhaseDurationCC;
	int m_nPhaseStepD0;
	int m_nDeadlineD4;

	void Restart(void);
	int Activate(void* pTarget);
	void ActivatePosition(int* pPoint);
};

#endif
