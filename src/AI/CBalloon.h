#ifndef LEMBALL_AI_CBALLOON_H
#define LEMBALL_AI_CBALLOON_H

class CBalloon {
public:
	char m_abReserved00[0x94];
	int m_nFrameTime94;
	char m_abReserved98[4];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;

	void Restart(void);
	int* ActivatePosition(int* pPoint);
};

#endif
