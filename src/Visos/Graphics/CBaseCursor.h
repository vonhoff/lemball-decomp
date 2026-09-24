#ifndef LEMBALL_VISOS_GRAPHICS_CBASECURSOR_H
#define LEMBALL_VISOS_GRAPHICS_CBASECURSOR_H

#include "../Foundation/CBaseQueueHandler.h" // complete type
#include "../Foundation/CVsPoint.h"          // complete type
#include "../Foundation/CVsRect.h"           // complete type
#include "CPopActive.h"                      // complete type
#include "CPushActive.h"                     // complete type

class CGWnd;
class CZRLE;
class CResBase;

enum eCursorDisplayType {
	CURSOR_DISPLAY_NONE = 0,
	CURSOR_DISPLAY_HAND = 1,
	CURSOR_DISPLAY_PAW = 2
};

// SIZE 0x9c
// VTABLE: LEMBALL 0x00499d38
class CBaseCursor : public CBaseQueueHandler {
public:
	CBaseCursor();
	virtual bool InWindow(CGWnd* p_window);     // vtable+0x0c
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void KillSystemCursor() = 0;        // vtable+0x10
	virtual void RestoreSystemCursor() = 0;     // vtable+0x14
	virtual void RefreshPos();                  // vtable+0x18
	virtual ~CBaseCursor();                     // vtable+0x04
	void Draw(CGWnd* p_window);
	void Initialise();
	void Process();
	void SetActive(unsigned int p_active);
	void SetMainID(unsigned int p_resourceId);
	void SetMainID(unsigned int p_resourceId, int p_frame);
	void SetPos(const CVsPoint& p_position);

	friend class CCursor;
	friend class C2D;
	friend void CursorChangeType(eCursorDisplayType p_cursorType, int p_frame);

private:
	CVsPoint m_position;                // 0x10
	CVsPoint m_hotspot;                 // 0x14
	unsigned int m_active;              // 0x18
	CZRLE* m_renderState;               // 0x1c
	CResBase* m_resource;               // 0x20
	int m_frame;                        // 0x24
	unsigned int m_resourceId;          // 0x28
	unsigned int m_mouseInput;          // 0x2c
	unsigned int m_keyboardInput;       // 0x30
	unsigned int m_changingCursor;      // 0x34
	unsigned int m_drawn;               // 0x38
	unsigned int m_systemCursorVisible; // 0x3c
	CPopActive m_popActive;             // 0x40
	CPushActive m_pushActive;           // 0x44
	int m_maxSpeed;                     // 0x4c
	int m_acceleration;                 // 0x50
	int m_fixedX;                       // 0x54
	int m_fixedY;                       // 0x58
	int m_velocityX;                    // 0x5c
	int m_velocityY;                    // 0x60
	int m_directionX;                   // 0x64
	int m_directionY;                   // 0x68
	unsigned int m_lastInputX;          // 0x6c
	unsigned int m_lastInputY;          // 0x70
	CVsRect m_bounds;                   // 0x74
	int m_keys[7];                      // 0x7c
	unsigned int m_reserved98;          // 0x98
};

// SYNTHETIC: LEMBALL 0x0046ba30
// CBaseCursor::`scalar deleting destructor'

#endif
