#ifndef LEMBALL_CMINEMANAGER_H
#define LEMBALL_CMINEMANAGER_H

class CAI;
class CMine;

struct MinePosition {
	short x;
	short y;
	short z;
};

class CMineManager {
public:
	CMineManager(CAI* pAI, int nCapacity);
	~CMineManager(void);
	void Restart(void);
	void Initialise(int nCapacity);

private:
	unsigned char m_abReserved00[0x30];
	CAI* m_pAI30;
	CMine* m_pObjects34;
	MinePosition* m_pPositions38;
	int m_cObjects3C;
	int m_cCapacity40;
};

struct MineManagerVtableLayout {
	void* m_apSlots00[6];
	void (CMineManager::*m_pRestart18)(void);
	void* m_pProcess1C;
	void* m_pCommand20;
	void* m_pGetViewData24;
};

typedef char CMineManagerSizeCheck[sizeof(CMineManager) == 0x44 ? 1 : -1];

#endif
