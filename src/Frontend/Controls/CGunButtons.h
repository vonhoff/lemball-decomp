#ifndef LEMBALL_FRONTEND_CONTROLS_CGUNBUTTONS_H
#define LEMBALL_FRONTEND_CONTROLS_CGUNBUTTONS_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/CVsRect.h"           // complete type

class CGWnd;
class CGdi;
class CGraphicButton;
class CResAnim;
class CTrackerButton;
// SIZE 0x60
// VTABLE: LEMBALL 0x00497cf8
class CGunButtons : public CBaseQueueHandler {
public:
	CGunButtons(CGWnd* p_window,
				CGdi* p_gdi,
				int p_x,
				int p_y,
				unsigned long* p_animIds,
				unsigned int p_postAction,
				int p_minimum,
				int p_maximum,
				int p_arg8,
				unsigned long p_controlMessage,
				void* p_binding,
				unsigned long p_actionMessage);
	static bool DrawBackBuffer();
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CGunButtons();                     // vtable+0x04
	void Draw(int p_firstState, int p_secondState);
	void LoadFaces(unsigned long* p_animIds);
	void UnLoadFaces();
	void Move(int p_x, int p_y);
	CGunButtons(const CVsRect& p_rect,
				CGWnd* p_window,
				CGdi* p_gdi,
				int p_x,
				int p_y,
				unsigned long* p_animIds,
				unsigned int p_postAction,
				int p_value,
				unsigned int p_controlMessage,
				int* p_binding,
				int p_actionMessage);

	friend class CGunController;

private:
	int m_mode;                      // 0x10
	CVsRect m_trackRect;             // 0x14
	int m_x;                         // 0x1c
	int m_y;                         // 0x20
	CGdi* m_gdi;                     // 0x24
	CGWnd* m_window;                 // 0x28
	unsigned int m_postAction;       // 0x2c
	unsigned int m_controlMessage;   // 0x30
	unsigned int m_actionMessage;    // 0x34
	int m_valueCount;                // 0x38
	int m_maximum;                   // 0x3c
	int m_minimum;                   // 0x40
	int m_value;                     // 0x44
	int* m_binding;                  // 0x48
	unsigned long* m_animIds;        // 0x4c
	CGraphicButton* m_graphicButton; // 0x50
	CTrackerButton* m_trackerButton; // 0x54
	CResAnim** m_resources;          // 0x58
	unsigned int m_active;           // 0x5c
};

extern int g_nGunButtonsRedrawPending;

// SYNTHETIC: LEMBALL 0x0044e620
// CGunButtons::`scalar deleting destructor'

#endif
