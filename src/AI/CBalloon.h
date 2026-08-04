#ifndef LEMBALL_AI_CBALLOON_H
#define LEMBALL_AI_CBALLOON_H

class CBalloon {
public:
	char m_abReserved00[0x5c];
	void* m_pTarget5C;
	char m_abReserved60[4];
	int m_nObjectType64;
	char m_abReserved68[0x2c];
	int m_nFrameTime94;
	char m_abReserved98[4];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;

	void Restart(void);
	int Activate(void* pTarget);
	int* ActivatePosition(int* pPoint);
};

#endif
