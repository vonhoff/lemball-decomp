#ifndef LEMBALL_CINVISIBLESWITCHMANAGER_H
#define LEMBALL_CINVISIBLESWITCHMANAGER_H

struct AICOORD;
class CGameObject;

class CInvisibleSwitchManager {
public:
	void StepOn(const AICOORD& position, CGameObject* pObject);
	void Process(void);
};

#endif
