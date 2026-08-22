#ifndef LEMBALL_SCAFFOLD_FRONTEND_CONTROLS_GUNBUTTONS_H
#define LEMBALL_SCAFFOLD_FRONTEND_CONTROLS_GUNBUTTONS_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/VsRect.h"           // complete type

// SIZE 0x60
// VTABLE: LEMBALL 0x00497cf8
class GunButtons : public BaseQueueHandler {
public:
	GunButtons(GWnd* p_arg0,
			   Gdi* p_arg1,
			   int p_arg2,
			   int p_arg3,
			   unsigned long* p_arg4,
			   unsigned char p_arg5,
			   int p_arg6,
			   int p_arg7,
			   int p_arg8,
			   unsigned long p_arg9,
			   void* p_arg10,
			   unsigned long p_arg11);
	bool DrawBackBuffer();
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~GunButtons();                      // vtable+0x04
	void Draw(unsigned char p_firstState, unsigned char p_secondState);
	void LoadFaces(unsigned long* p_animIds);
	void UnLoadFaces();

private:
	int m_mode;                     // 0x10
	VsRect m_trackRect;             // 0x14
	int m_x;                        // 0x1c
	int m_y;                        // 0x20
	Gdi* m_gdi;                     // 0x24
	GWnd* m_window;                 // 0x28
	unsigned int m_postAction;      // 0x2c
	unsigned int m_controlMessage;  // 0x30
	unsigned int m_actionMessage;   // 0x34
	int m_valueCount;               // 0x38
	int m_maximum;                  // 0x3c
	int m_minimum;                  // 0x40
	int m_value;                    // 0x44
	int* m_binding;                 // 0x48
	unsigned long* m_animIds;       // 0x4c
	GraphicButton* m_graphicButton; // 0x50
	TrackerButton* m_trackerButton; // 0x54
	ResAnim** m_resources;          // 0x58
	unsigned int m_active;          // 0x5c
};

#endif
