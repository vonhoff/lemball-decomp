#ifndef LEMBALL_CMINEMANAGER_H
#define LEMBALL_CMINEMANAGER_H

class CAI;
class CGameObject;
class CViewData;
class CMine;
struct AICOORD;

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
	void Remove(CMine* pMine);
	void Triggered(CMine* pMine);
	void Trigger(int nIndex, int nDelay);
	void StepOn(const AICOORD& position, CGameObject* pObject);
	void Add(unsigned short nId, AICOORD position);
	void Process(void);
	int GetViewData(CViewData* pViewData);
	void LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion);

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
	void (CMineManager::*m_pProcess1C)(void);
	void* m_pCommand20;
	int (CMineManager::*m_pGetViewData24)(CViewData* pViewData);
};

typedef char CMineManagerSizeCheck[sizeof(CMineManager) == 0x44 ? 1 : -1];

#endif
