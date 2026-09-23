#ifndef LEMBALL_VIEWS_INPUT_CPADTOBUTTON_H
#define LEMBALL_VIEWS_INPUT_CPADTOBUTTON_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type

class CPVButton;
struct PadToButtonEntry;
// SIZE 0x1c
// VTABLE: LEMBALL 0x00497208
class CPadToButton : public CBaseQueueHandler {
public:
	CPadToButton(int p_entryCapacity);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CPadToButton();                    // vtable+0x04
	void AddBinding(CPVButton* p_button, unsigned int p_padCode);

private:
	PadToButtonEntry* m_entries; // 0x10
	int m_entryCount;            // 0x14
	int m_entryCapacity;         // 0x18
};

// SYNTHETIC: LEMBALL 0x0043a490
// CPadToButton::`scalar deleting destructor'

#endif
