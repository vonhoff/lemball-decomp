#ifndef LEMBALL_VISOS_GRAPHICS_CCURSOR_H
#define LEMBALL_VISOS_GRAPHICS_CCURSOR_H

#include "CBaseCursor.h" // complete type

// SIZE 0xa4
// VTABLE: LEMBALL 0x00499d58
class CCursor : public CBaseCursor {
public:
	CCursor();
	void InitialiseSystemCursor();
	virtual void KillSystemCursor();    // vtable+0x10
	virtual void RestoreSystemCursor(); // vtable+0x14
	virtual void RefreshPos();          // vtable+0x18
	virtual ~CCursor();                 // vtable+0x04

private:
	void* m_systemCursor;      // 0x9c
	unsigned int m_reservedA0; // 0xa0
};

inline CCursor::CCursor()
{
	InitialiseSystemCursor();
}

extern CCursor* g_pCursor;

void CursorChangeType(eCursorDisplayType p_cursorType, int p_frame);

// SYNTHETIC: LEMBALL 0x0046dad0
// CCursor::`scalar deleting destructor'

#endif
