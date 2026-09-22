#ifndef LEMBALL_FRONTEND_CONTROLS_CHILITEBUTTONS_H
#define LEMBALL_FRONTEND_CONTROLS_CHILITEBUTTONS_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/CVsRect.h"           // complete type

class CGWnd;
class CGDI;
class CGraphicButton;
class CResANIM;
// SIZE 0x58
// VTABLE: LEMBALL 0x00498150
class CHiliteButtons : public CBaseQueueHandler {
public:
	CHiliteButtons(CGWnd* p_window,
				   CGDI* p_gdi,
				   int p_x,
				   int p_y,
				   unsigned long* p_animIds,
				   unsigned int p_mode,
				   int p_minimum,
				   int p_maximum,
				   int p_arg8,
				   unsigned long p_controlMessage,
				   void* p_binding,
				   unsigned long p_actionMessage);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CHiliteButtons();                  // vtable+0x04
	void Draw(int p_force);
	void MoveCurrentButton(int p_x, int p_y);
	void LoadFaces(unsigned long* p_animIds);
	void UnLoadFaces();
	void UpdateAnimID();

	friend class CHiliteController;

private:
	CVsRect m_bounds;              // 0x10
	int m_x;                       // 0x18
	int m_y;                       // 0x1c
	CGDI* m_gdi;                   // 0x20
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
	CResANIM** m_resources;        // 0x50
	unsigned int m_active;         // 0x54
};

// SYNTHETIC: LEMBALL 0x0044ffc0
// CHiliteButtons::`scalar deleting destructor'

#endif
