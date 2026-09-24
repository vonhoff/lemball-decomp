#ifndef LEMBALL_AI_OBJECTS_CTRAPDOOR_H
#define LEMBALL_AI_OBJECTS_CTRAPDOOR_H

#include "../Base/CBaseGlobalObject.h" // complete type

class AiCoord;
// SIZE 0x150
// VTABLE: LEMBALL 0x00493438
class CTrapDoor : public CBaseGlobalObject {
public:
	CTrapDoor(AiCoord& p_position, unsigned int p_mode);
	void SetPositionFromIntegers(int p_x, int p_y, int p_z);
	virtual bool Process();                          // vtable+0x14
	virtual int Usage();                             // vtable+0x28
	virtual void DoActivate();                       // vtable+0x10c
	virtual void GetViewData(CViewData& p_viewData); // vtable+0x0c
	virtual void Restart();                          // vtable+0x104

	friend class CTrapDoorManager;

private:
	unsigned int m_mode;     // 0x144
	unsigned int m_active;   // 0x148
	unsigned int m_deadline; // 0x14c
};

// SYNTHETIC: LEMBALL 0x0040cea0
// CTrapDoor::`scalar deleting destructor'

#endif
