#ifndef LEMBALL_CLIFTMANAGER_H
#define LEMBALL_CLIFTMANAGER_H

class CAI;
class CLift;

class CLiftManager {
public:
	CLiftManager(CAI* pAI, int nCapacity);

private:
	unsigned char m_abReserved00[0x30];
	CAI* m_pAI30;
	int m_cObjects34;
	int m_cCapacity38;
	CLift* m_pObjects3C;
};

#endif
