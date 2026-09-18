#ifndef LEMBALL_VISOS_FOUNDATION_TRANSLATOR_H
#define LEMBALL_VISOS_FOUNDATION_TRANSLATOR_H

#include "BaseQueueHandler.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x004988a0
class Translator : public BaseQueueHandler {
public:
	Translator() {}
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
};

extern Translator* g_pInputTranslator;

// SYNTHETIC: LEMBALL 0x004599d0
// Translator::`scalar deleting destructor'

#endif
