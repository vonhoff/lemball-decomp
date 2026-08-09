#ifndef LEMBALL_CHANDMANAGER_H
#define LEMBALL_CHANDMANAGER_H

class CAI;
class CHand;
class CGameObject;
class CViewData;
struct AICOORD;

class CHandManager {
public:
	CHandManager(CAI* pAI, int nCapacity);
	void Restart(void);
	~CHandManager(void);
	void Initialise(int nCapacity);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	void Process(void);
	int GetViewData(CViewData* pViewData);
	void Add(unsigned short nSlotId, int x, int y, int z);
	void Remove(CHand* pHand);
	void LoadLevel(unsigned char* pData, int, unsigned char);

private:
	unsigned char m_abReserved00[0x30];
	int m_nCapacity30;
	int m_nObjectCount34;
	CHand* m_pObjects38;
	CAI* m_pAI3C;
};

struct CHandManagerVtable {
	void* m_apBaseSlots00[6];
	void (CHandManager::*m_pRestart18)(void);
	void (CHandManager::*m_pProcess1C)(void);
	void* m_pReserved20;
	int (CHandManager::*m_pGetViewData24)(CViewData* pViewData);
};

extern CHandManagerVtable g_LINKSCF_CaptureChunkManagerVtable;

typedef char CHandManagerSizeMustMatchLayout[sizeof(CHandManager) == 0x40 ? 1 : -1];
typedef char CHandManagerMemberPointerMustFitVtableSlot[sizeof(void (CHandManager::*)(void)) == sizeof(void*) ? 1 : -1];

#endif
