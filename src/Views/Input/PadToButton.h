#ifndef LEMBALL_VIEWS_INPUT_PADTOBUTTON_H
#define LEMBALL_VIEWS_INPUT_PADTOBUTTON_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00497208
class PadToButton : public BaseQueueHandler {
public:
	PadToButton(int p_arg0);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~PadToButton();                     // vtable+0x04

private:
	PadToButtonEntry* m_entries; // 0x10
	int m_entryCount;            // 0x14
	int m_entryCapacity;         // 0x18
};

// SYNTHETIC: LEMBALL 0x0043a490
// PadToButton::`scalar deleting destructor'


#endif

