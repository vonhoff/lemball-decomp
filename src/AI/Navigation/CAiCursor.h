#ifndef LEMBALL_AI_NAVIGATION_CAICURSOR_H
#define LEMBALL_AI_NAVIGATION_CAICURSOR_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type

class CAi;
class CBaseQueue;
// SIZE 0x28
// VTABLE: LEMBALL 0x00493c08
class CAiCursor : public CBaseQueueHandler {
public:
	CAiCursor(CAi* p_arg0, int p_arg1, int p_arg2);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CAiCursor();                       // vtable+0x04
	void CheckAndClipCursorBounds();
	void GetCursorSurfaceCoordinates(int& p_x, int& p_y);
	void SetCursorXy(int p_x, int p_y);

private:
	int m_cursorX;       // 0x10
	int m_cursorY;       // 0x14
	int m_maximumX;      // 0x18
	int m_maximumY;      // 0x1c
	CAi* m_ai;           // 0x20
	CBaseQueue* m_queue; // 0x24
};

// SYNTHETIC: LEMBALL 0x00414f00
// CAiCursor::`scalar deleting destructor'

#endif
