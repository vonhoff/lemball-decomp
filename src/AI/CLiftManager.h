#ifndef LEMBALL_CLIFTMANAGER_H
#define LEMBALL_CLIFTMANAGER_H

class CAI;
class CGameObject;
class CLift;
struct AICOORD;

class CLiftManager {
public:
	CLiftManager(CAI* pAI, int nCapacity);
	~CLiftManager(void);
	void Restart(void);
	void Initialise(int nCapacity);
	void Process(void);
	void StepOn(const AICOORD& coord, CGameObject* pObject);

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
	void (CLiftManager::*m_pProcess1C)(void);
	void* m_apSlots20[2];
};

#endif
