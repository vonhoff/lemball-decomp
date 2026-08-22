#ifndef LEMBALL_SCAFFOLD_AI_NAVIGATION_AICURSOR_H
#define LEMBALL_SCAFFOLD_AI_NAVIGATION_AICURSOR_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00493c08
class AiCursor : public BaseQueueHandler {
public:
	AiCursor(Ai* p_arg0, int p_arg1, int p_arg2);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~AiCursor();                        // vtable+0x04
	void CheckAndClipCursorBounds();
	void GetCursorSurfaceCoordinates(int& p_x, int& p_y);
	void SetCursorXy(int p_x, int p_y);

private:
	int m_cursorX;      // 0x10
	int m_cursorY;      // 0x14
	int m_maximumX;     // 0x18
	int m_maximumY;     // 0x1c
	Ai* m_ai;           // 0x20
	BaseQueue* m_queue; // 0x24
};

#endif
