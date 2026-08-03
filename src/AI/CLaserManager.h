#ifndef LEMBALL_CLASERMANAGER_H
#define LEMBALL_CLASERMANAGER_H

class CAI;
class CLaser;
class CGameObject;
class CViewData;
struct AICOORD;

class CLaserManager {
public:
	CLaserManager(CAI* pAI, int nCapacity);
	~CLaserManager(void);
	void Restart(void);
	void Initialise(int nCapacity);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	void Process(void);
	int GetViewData(CViewData* pViewData);

private:
	unsigned char m_abReserved00[0x30];
	int m_cCapacity30;
	int m_cObjects34;
	CLaser* m_pObjects38;
	CAI* m_pAI3C;
};

#endif
