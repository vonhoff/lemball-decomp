#ifndef LEMBALL_AI_CTOWER_H
#define LEMBALL_AI_CTOWER_H

class CTower {
public:
	char m_abReserved00[0x9c];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;

	int Activate(void* pTarget);
	void ActivatePosition(int* pPoint);
};

#endif
