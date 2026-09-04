#ifndef LEMBALL_AI_MANAGERS_INVISIBLESWITCHMANAGER_H
#define LEMBALL_AI_MANAGERS_INVISIBLESWITCHMANAGER_H

#include "../../Common.h"
#include "BaseObjectManager.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00493298
class InvisibleSwitchManager : public BaseObjectManager {
public:
	InvisibleSwitchManager(Ai* p_arg0, int p_arg1);
	virtual void Process();            // vtable+0x1c
	virtual ~InvisibleSwitchManager(); // vtable+0x14
	void Restart();
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void StepOn(const AiCoord& p_position, GameObject* p_object);

private:
	int m_capacity;              // 0x30
	int m_count;                 // 0x34
	Ai* m_ai;                    // 0x38
	InvisibleSwitch* m_switches; // 0x3c
};

// SYNTHETIC: LEMBALL 0x0040ac30
// InvisibleSwitchManager::`scalar deleting destructor'

#endif
