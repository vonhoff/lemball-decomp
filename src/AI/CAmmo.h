#ifndef LEMBALL_AI_CAMMO_H
#define LEMBALL_AI_CAMMO_H

struct LevelThreeDwordPosition {
	int m_nX;
	int m_nY;
	int m_nZ;
};

class CAmmo {
public:
	char m_abReserved00[0x5c];
	void* m_pTarget5C;
	char m_abReserved60[4];
	int m_nObjectType64;
	char m_abReserved68[0x2c];
	int m_nFrameTime94;
	char m_abReserved98[4];
	LevelThreeDwordPosition m_CurrentPosition9C;
	char m_abReservedA8[0x10];
	int m_nStateB8;
	char m_abReservedBC[0x10];
	int m_nCycleDeadlineCC;
	char m_abReservedD0[4];
	int m_nDeadlineD4;
	char m_abReservedD8[0x3c];
	int m_fNetworkShadow114;
	char m_abReserved118[0x10];
	int m_nPreviousState128;
	char m_abReserved12C[0x18];
	short m_nLinkedVariant144;

	void Restart(void);
	int Activate(void* pTarget);
	void DoActivate(void);
	LevelThreeDwordPosition* ActivatePosition(LevelThreeDwordPosition* pPoint);
	int Process(void);
};

#endif
