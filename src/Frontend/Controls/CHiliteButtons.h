#ifndef LEMBALL_FRONTEND_CONTROLS_CHILITEBUTTONS_H
#define LEMBALL_FRONTEND_CONTROLS_CHILITEBUTTONS_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/CVsRect.h"           // complete type

class CGWnd;
class CGdi;
class CGraphicButton;
class CResAnim;
// SIZE 0x58
// VTABLE: LEMBALL 0x00498150
class CHiliteButtons : public CBaseQueueHandler {
public:
	CHiliteButtons(CGWnd* p_arg0,
				   CGdi* p_arg1,
				   int p_arg2,
				   int p_arg3,
				   unsigned long* p_arg4,
				   unsigned int p_arg5,
				   int p_arg6,
				   int p_arg7,
				   int p_arg8,
				   unsigned long p_arg9,
				   void* p_arg10,
				   unsigned long p_arg11);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CHiliteButtons();                  // vtable+0x04
	void Draw(int p_force);
	void MoveCurrentButton(int p_x, int p_y);
	void LoadFaces(unsigned long* p_animIds);
	void UnLoadFaces();
	void UpdateAnimId();

	friend class CHiliteController;

private:
	CVsRect m_bounds;              // 0x10
	int m_x;                       // 0x18
	int m_y;                       // 0x1c
	CGdi* m_gdi;                   // 0x20
	CGWnd* m_window;               // 0x24
	unsigned int m_mode;           // 0x28
	unsigned int m_controlMessage; // 0x2c
	unsigned int m_actionMessage;  // 0x30
	int m_valueCount;              // 0x34
	int m_maximum;                 // 0x38
	int m_minimum;                 // 0x3c
	int m_value;                   // 0x40
	int* m_binding;                // 0x44
	unsigned long* m_animIds;      // 0x48
	CGraphicButton* m_button;      // 0x4c
	CResAnim** m_resources;        // 0x50
	unsigned int m_active;         // 0x54
};

// SYNTHETIC: LEMBALL 0x0044ffc0
// CHiliteButtons::`scalar deleting destructor'

#endif
