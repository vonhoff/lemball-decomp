#ifndef LEMBALL_CINVISIBLESWITCH_H
#define LEMBALL_CINVISIBLESWITCH_H

struct AICOORD {
	int x;
	int y;
	int z;
};

class CGameObject;

class CInvisibleSwitch {
public:
	void VerifyObjects(void);
	void AddObject(CGameObject* pObject);
	void StepOn(const AICOORD& position, CGameObject* pObject);
	void DoActivate(void);
};

#endif
