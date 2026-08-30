#ifndef LEMBALL_FRONTEND_PROCESSES_TARGETABOUT_H
#define LEMBALL_FRONTEND_PROCESSES_TARGETABOUT_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseProcess.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00493090
class TargetAbout : public BaseProcess {
public:
	TargetAbout(Game* p_game)
	{
		m_returnState = 0;
		m_processState = 0;
		m_game = p_game;
	}
	virtual ~TargetAbout(); // vtable+0x00
	virtual void Process(); // vtable+0x04

private:
	Game* m_game; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00408010
// TargetAbout::`scalar deleting destructor'

#endif
