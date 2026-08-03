#ifndef LEMBALL_CLIFTMANAGER_H
#define LEMBALL_CLIFTMANAGER_H

class CAI;
class CLift;

class CLiftManager {
public:
	CLiftManager(CAI* pAI, int nCapacity);
	~CLiftManager(void);
	void Restart(void);
	void Initialise(int nCapacity);

private:
	unsigned char m_abReserved00[0x30];
	CAI* m_pAI30;
	int m_cObjects34;
	int m_cCapacity38;
	CLift* m_pObjects3C;
};

struct LiftManagerVtableLayout {
	void* m_apSlots00[6];
	void (CLiftManager::*m_pRestart18)(void);
	void* m_apSlots1C[3];
};

#endif
