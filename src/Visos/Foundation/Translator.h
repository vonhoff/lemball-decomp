#ifndef LEMBALL_VISOS_FOUNDATION_TRANSLATOR_H
#define LEMBALL_VISOS_FOUNDATION_TRANSLATOR_H

#include "../../Common.h"
#include "BaseQueueHandler.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x004988a0
class Translator : public BaseQueueHandler {
public:
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~Translator();                      // vtable+0x04
};

// Confirmed class-scoped globals.
extern Translator* g_pInputTranslator;
#endif
