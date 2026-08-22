#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_CURSOR_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_CURSOR_H

#include "../../Common.h"
#include "BaseCursor.h" // complete type

// SIZE 0xa4
// VTABLE: LEMBALL 0x00499d58
class Cursor : public BaseCursor {
public:
	virtual void KillSystemCursor();    // vtable+0x10
	virtual void RestoreSystemCursor(); // vtable+0x14
	virtual ~Cursor();                  // vtable+0x04

private:
	void* m_systemCursor;    // 0x9c
	undefined4 m_reservedA0; // 0xa0
};

void CursorChangeType(eCursorDisplayType p_arg0, int p_arg1);
#endif
