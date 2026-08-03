#ifndef LEMBALL_CINVISIBLESWITCHMANAGER_H
#define LEMBALL_CINVISIBLESWITCHMANAGER_H

struct AICOORD;
class CAI;
class CGameObject;
class CInvisibleSwitch;

class CInvisibleSwitchManager {
public:
	CInvisibleSwitchManager(CAI* pAI, int nCapacity);
	void Restart(void);
	void Initialise(int nCapacity);
	~CInvisibleSwitchManager(void);
	void StepOn(const AICOORD& position, CGameObject* pObject);
	void Process(void);
	void LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion);

private:
	unsigned char m_abReserved00[0x30];
	int m_cCapacity30;
	int m_cObjects34;
	CAI* m_pAI38;
	CInvisibleSwitch* m_pObjects3C;
};

#endif
