#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_BASECURSOR_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_BASECURSOR_H

#include "../../Common.h"
#include "../Foundation/BaseQueueHandler.h" // complete type
#include "../Foundation/VsPoint.h"          // complete type
#include "../Foundation/VsRect.h"           // complete type

// SIZE 0x9c
// VTABLE: LEMBALL 0x00499d38
class BaseCursor : public BaseQueueHandler {
public:
	BaseCursor();
	virtual bool InWindow(GWnd* p_window);      // vtable+0x0c
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void RefreshPos();                  // vtable+0x18
	virtual ~BaseCursor();                      // vtable+0x04
	void Draw(GWnd* p_window);
	void Initialise();
	void Process();
	void SetActive(unsigned char p_active);
	void SetMainId(unsigned int p_resourceId);
	void SetMainId(unsigned int p_resourceId, int p_frame);
	void SetPos(const VsPoint& p_position);

private:
	VsPoint m_position;                 // 0x10
	VsPoint m_hotspot;                  // 0x14
	unsigned int m_active;              // 0x18
	void* m_renderState;                // 0x1c
	ResBase* m_resource;                // 0x20
	int m_frame;                        // 0x24
	unsigned int m_resourceId;          // 0x28
	unsigned int m_mouseInput;          // 0x2c
	unsigned int m_keyboardInput;       // 0x30
	unsigned int m_changingCursor;      // 0x34
	unsigned int m_drawn;               // 0x38
	unsigned int m_systemCursorVisible; // 0x3c
	undefined m_primitiveState[0x0c];   // 0x40
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
	VsRect m_bounds;                    // 0x74
	int m_keys[7];                      // 0x7c
	undefined4 m_reserved98;            // 0x98
};

#endif
