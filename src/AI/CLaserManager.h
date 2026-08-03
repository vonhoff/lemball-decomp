#ifndef LEMBALL_CLASERMANAGER_H
#define LEMBALL_CLASERMANAGER_H

class CAI;
class CLaser;

class CLaserManager {
public:
	CLaserManager(CAI* pAI, int nCapacity);
	void Restart(void);

private:
	unsigned char m_abReserved00[0x30];
	int m_cCapacity30;
	int m_cObjects34;
	CLaser* m_pObjects38;
	CAI* m_pAI3C;
};

#endif
