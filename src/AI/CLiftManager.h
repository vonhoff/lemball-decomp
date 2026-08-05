#ifndef LEMBALL_CLIFTMANAGER_H
#define LEMBALL_CLIFTMANAGER_H

class CAI;
class CGameObject;
class CViewData;
class CLift;
struct AICOORD;
struct tCoord3d;
typedef int swMessage;

class CLiftManager {
public:
	CLiftManager(CAI* pAI, int nCapacity);
	~CLiftManager(void);
	void Restart(void);
	void Initialise(int nCapacity);
	void Process(void);
	void StepOn(const AICOORD& coord, CGameObject* pObject);
	int GetViewData(CViewData* pViewData);
	void Switch(swMessage message, int nSlotId, int nRangeEnd, int nUnused4);
	unsigned short Id(int iObject);
	void LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion);
	void AppendLiftChunkObjectBetweenEndpoints(unsigned short nSlotId, const tCoord3d& start, const tCoord3d& end);
	void AppendLegacyLiftChunkObject(unsigned short nSlotId, int nParam1, int nParam2, int nParam3);

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
	void* m_pSlot20;
	int (CLiftManager::*m_pGetViewData24)(CViewData*);
};

#endif
