#ifndef LEMBALL_AI_CCRATE_H
#define LEMBALL_AI_CCRATE_H

class CCrate {
public:
	char m_abReserved00[0x94];
	int m_nFrameTime94;
	char m_abReserved98[4];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;
	char m_abReservedA8[0x10];
	int m_nStateB8;
	char m_abReservedBC[0x14];
	int m_nPhaseStepD0;
	int m_nDeadlineD4;
	char m_abReservedD8[0x50];
	int m_nState128;
	char m_abReserved12C[0x1c];
	void* m_pContents148;
	int m_nContentsType14C;

	void Restart(void);
	int Activate(void* pTarget);
	void DoActivate(void);
	void ActivatePosition(int* pPoint);
};

#endif
