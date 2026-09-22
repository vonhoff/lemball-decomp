#ifndef LEMBALL_VISOS_FOUNDATION_CTRANSLATOR_H
#define LEMBALL_VISOS_FOUNDATION_CTRANSLATOR_H

#include "CBaseQueueHandler.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x004988a0
class CTranslator : public CBaseQueueHandler {
public:
	CTranslator() {}
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
};

extern CTranslator* g_pInputTranslator;

// SYNTHETIC: LEMBALL 0x004599d0
// CTranslator::`scalar deleting destructor'

#endif
