#ifndef LEMBALL_AI_MANAGERS_CINVISIBLESWITCHMANAGER_H
#define LEMBALL_AI_MANAGERS_CINVISIBLESWITCHMANAGER_H

#include "CBaseObjectManager.h" // complete type

class CAi;
class AiCoord;
class CGameObject;
class CInvisibleSwitch;
struct Coord3d;
// SIZE 0x40
// VTABLE: LEMBALL 0x00493298
class CInvisibleSwitchManager : public CBaseObjectManager {
public:
	CInvisibleSwitchManager(CAi* p_arg0, int p_arg1);
	virtual void Process();             // vtable+0x1c
	virtual ~CInvisibleSwitchManager(); // vtable+0x14
	void Restart();
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void StepOn(const AiCoord& p_position, CGameObject* p_object);
	void AddSwitch(unsigned short p_id, const Coord3d& p_min, const Coord3d& p_max);

private:
	int m_capacity;               // 0x30
	int m_count;                  // 0x34
	CAi* m_ai;                    // 0x38
	CInvisibleSwitch* m_switches; // 0x3c
};

// SYNTHETIC: LEMBALL 0x0040ac30
// CInvisibleSwitchManager::`scalar deleting destructor'

#endif
