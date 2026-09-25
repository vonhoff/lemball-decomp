#ifndef LEMBALL_AI_OBJECTS_CROCKET_H
#define LEMBALL_AI_OBJECTS_CROCKET_H

#include "../Base/CGlobalGameObject.h"

class AiCoord;
class CGameObject;
// SIZE 0x144
// VTABLE: LEMBALL 0x00495ed8
class CRocket : public CGlobalGameObject {
public:
	CRocket();
	int StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual bool Process();                          // vtable+0x14
	virtual void DoActivate();                       // vtable+0x10c
	virtual void GetViewData(CViewData& p_viewData); // vtable+0x0c
	virtual void Restart();                          // vtable+0x104
	virtual ~CRocket();                              // vtable+0x00
	void Initialise();
	void Set(unsigned short p_id, const AiCoord& p_position);

	friend class CRocketManager;

private:
	unsigned int m_enabled; // 0x138
	unsigned int m_active;  // 0x13c
	int m_launchBaseZ;      // 0x140
};

// SYNTHETIC: LEMBALL 0x00427520
// CRocket::`vector deleting destructor'

#endif
