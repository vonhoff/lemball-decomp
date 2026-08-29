#ifndef LEMBALL_VISOS_FOUNDATION_MASTERINPUT_H
#define LEMBALL_VISOS_FOUNDATION_MASTERINPUT_H

#include "../../Common.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x0049a468
class MasterInput {
public:
	MasterInput(BaseQueue* p_arg0);
	virtual VsOStream& StreamOut(VsOStream& p_arg0); // vtable+0x00
	virtual ~MasterInput();                          // vtable+0x04

	friend bool TargetInputInit();
	friend bool TargetInputQuit();
	friend class HotAreaHandler;
	friend class BaseCursor;

private:
	undefined4 m_unk0x04;     // 0x04
	unsigned int m_itemCount; // 0x08
	BaseQueue* m_queue;       // 0x0c
	void* m_firstItem;        // 0x10
	unsigned int m_state;     // 0x14
};

extern MasterInput* g_pMasterInput;

// SYNTHETIC: LEMBALL 0x00472270
// MasterInput::`scalar deleting destructor'

#endif

