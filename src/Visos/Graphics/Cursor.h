#ifndef LEMBALL_VISOS_GRAPHICS_CURSOR_H
#define LEMBALL_VISOS_GRAPHICS_CURSOR_H

#include "../../Common.h"
#include "BaseCursor.h" // complete type

// SIZE 0xa4
// VTABLE: LEMBALL 0x00499d58
class Cursor : public BaseCursor {
public:
	Cursor();
	void InitialiseSystemCursor();
	virtual void KillSystemCursor();    // vtable+0x10
	virtual void RestoreSystemCursor(); // vtable+0x14
	virtual void RefreshPos();          // vtable+0x18
	virtual ~Cursor();                  // vtable+0x04

private:
	void* m_systemCursor;    // 0x9c
	undefined4 m_reservedA0; // 0xa0
};

inline Cursor::Cursor()
{
	InitialiseSystemCursor();
}

extern Cursor* g_pCursor;

void CursorChangeType(eCursorDisplayType p_arg0, int p_arg1);

// SYNTHETIC: LEMBALL 0x0046dad0
// Cursor::`scalar deleting destructor'

#endif
