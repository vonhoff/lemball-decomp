#ifndef LEMBALL_AI_MANAGERS_ICEMANAGER_H
#define LEMBALL_AI_MANAGERS_ICEMANAGER_H

#include "../../Common.h"
#include "BaseObjectManager.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00496a00
class IceManager : public BaseObjectManager {
public:
	IceManager(Ai* p_arg0, int p_arg1);
	bool StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual int GetViewData(ViewData* p_viewData); // vtable+0x24
	virtual void Process();                        // vtable+0x1c
	virtual ~IceManager();                         // vtable+0x14
	void Restart();
	void Add(unsigned short p_id,
			 const Coord3d& p_cornerA,
			 const Coord3d& p_cornerB,
			 int p_velocityX,
			 int p_velocityY,
			 unsigned char p_initialSwitched);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Switch(int p_message, int p_id);

private:
	int m_capacity; // 0x30
	int m_count;    // 0x34
	Ice* m_ice;     // 0x38
	Ai* m_ai;       // 0x3c
};

// SYNTHETIC: LEMBALL 0x0042e4d0
// IceManager::`scalar deleting destructor'

#endif
