#ifndef LEMBALL_FRONTEND_PROCESSES_CABOUT_H
#define LEMBALL_FRONTEND_PROCESSES_CABOUT_H

#include "../../Visos/Foundation/CBaseProcess.h" // complete type

class CGame;
// SIZE 0x10
// VTABLE: LEMBALL 0x00493090
class CAbout : public CBaseProcess {
public:
	CAbout(CGame* p_game) { m_game = p_game; }
	virtual ~CAbout() {}    // vtable+0x00
	virtual void Process(); // vtable+0x04

private:
	CGame* m_game; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00408010
// CAbout::`scalar deleting destructor'

#endif
