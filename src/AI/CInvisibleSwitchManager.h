#ifndef LEMBALL_CINVISIBLESWITCHMANAGER_H
#define LEMBALL_CINVISIBLESWITCHMANAGER_H

struct AICOORD;
class CAI;
class CGameObject;

class CInvisibleSwitchManager {
public:
	CInvisibleSwitchManager(CAI* pAI, int nCapacity);
	void Restart(void);
	void Initialise(int nCapacity);
	~CInvisibleSwitchManager(void);
	void StepOn(const AICOORD& position, CGameObject* pObject);
	void Process(void);
};

#endif
